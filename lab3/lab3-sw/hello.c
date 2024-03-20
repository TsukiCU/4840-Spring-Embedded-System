/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <argp.h>
#include <stdlib.h>

#define ABS(X) (X>0?X:-X)

int vga_ball_fd;

struct arguments{
  vga_ball_arg_t arg;
  vga_ball_dir_t dir;
};

static char args_doc[] = "-x StartX -y StartY -u DirectionX -v DirectionY";
static char doc[] = "A bouncing ball program.";
static struct argp_option options[] = {
  // TODO: Add color settings?
  {"startx",'x',"STARTX",0,"start x coordinate of circle"},
  {"starty",'y',"STARTY",0,"start y coordinate of circle"},
  {"dx",'u',"DELTAX",0,"start x direction"},
  {"dy",'v',"DELTAY",0,"start y direction"},
  {"radius",'r',"RADIUS",0,"circle radius"},
  { 0 }
};
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *args = state->input;
    switch (key) {
    case 'x': args->arg.circle.x = atoi(arg); break;
    case 'y': args->arg.circle.y = atoi(arg); break;
    case 'u': args->dir.dx = atoi(arg); break;
    case 'v': args->dir.dy = atoi(arg); break;
    case 'r': args->arg.circle.radius = atoi(arg); break;
    case ARGP_KEY_END:
      if(!args->dir.dx||!args->dir.dy){
        argp_failure(state, 1, 0, "required -u and -v. See --help for more information");
        exit(ARGP_ERR_UNKNOWN);
      }
    default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
struct argp argp = {options, parse_opt, args_doc, doc};

/* Read and print the background color */
void print_background_color() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
  printf("%02x %02x %02x %02x %02x %02x %02x %d %d %d\n",
	 vla.color.red, vla.color.green, vla.color.blue,
   (unsigned char)(vla.circle.x>>8),(unsigned char)(vla.circle.x),
   (unsigned char)(vla.circle.y>>8),(unsigned char)(vla.circle.y),
   vla.circle.x,vla.circle.y,
   vla.circle.radius);
   //vla.rect.left,vla.rect.top,vla.rect.right,vla.rect.bottom);
}

/* Set the background color */
void set_background_color(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.color = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}

void set_display(const vga_ball_arg_t *arg)
{
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, arg)) {
    perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
    return;
  }
}

void update_circle(vga_ball_circle_t *circle, vga_ball_dir_t *dir)
{
  short remain_x = dir->dx>0?(640.0-circle->radius)-circle->x:circle->radius-circle->x;
  short remain_y = dir->dy>0?(480.0-circle->radius)-circle->y:circle->radius-circle->y;
  //printf("d %d %d remain %d %d\n",dir->dx,dir->dy,remain_x,remain_y);
  // TODO: FIX dx==0||dy==0 float exception
  if(!(remain_x/dir->dx)||!(remain_y/dir->dy)){
    // x is faster
    if((float)ABS(remain_x/dir->dx)<(float)ABS(remain_y/dir->dy)){
      remain_y = (float)remain_x/dir->dx*dir->dy;
      dir->dx=-dir->dx;
      //printf("dx change %d %d\n",remain_x,remain_y);
    }
    else{
      remain_x = (float)remain_y/dir->dy*dir->dx;
      dir->dy=-dir->dy;
      //printf("dy change %d %d\n",remain_x,remain_y);
    }
    if(!remain_x&&!remain_y){
      //printf("dx==dy==0\n");
      circle->x+=dir->dx;
      circle->y+=dir->dy;
    }else{
      circle->x+=remain_x;
      circle->y+=remain_y;
    }
  }
  else {
    circle->x+=dir->dx;
    circle->y+=dir->dy;
  }
}

int main(int argc,char **argv)
{
  struct arguments args;
  //args.arg.circle.radius=16;
  argp_parse(&argp,argc,argv,0,0,&args);
  vga_ball_arg_t vla = args.arg;
  vga_ball_dir_t dir = args.dir;

  int i;
  static const char filename[] = "/dev/vga_ball";

  static const vga_ball_color_t colors[] = {
    { 0xff, 0x00, 0x00 }, /* Red */
    { 0x00, 0xff, 0x00 }, /* Green */
    { 0x00, 0x00, 0xff }, /* Blue */
    { 0xff, 0xff, 0x00 }, /* Yellow */
    { 0x00, 0xff, 0xff }, /* Cyan */
    { 0xff, 0x00, 0xff }, /* Magenta */
    { 0x80, 0x80, 0x80 }, /* Gray */
    { 0x00, 0x00, 0x00 }, /* Black */
    { 0xff, 0xff, 0xff }  /* White */
  };

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
  printf("initial state: ");
  print_background_color();

  vga_ball_circle_t circle = vla.circle;
  if(circle.x<circle.radius)
    circle.x=circle.radius;
  if(circle.x>640-circle.radius)
    circle.x=640-circle.radius;
  if(circle.y<circle.radius)
    circle.y=circle.radius;
  if(circle.y>480-circle.radius)
    circle.y=480-circle.radius;
  vla.circle = circle;

  while(1) {
    vla.color = colors[i % COLORS ];
    set_display(&vla);

    update_circle(&vla.circle,&dir);
    print_background_color();
    usleep(200000);
  }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
