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

#define ABS(X) ((X)>0?(X):-(X))

int vga_ball_fd;

struct arguments{
  vga_ball_arg_t arg;
  vga_ball_dir_t dir;
  vga_ball_color_t c_color;
  vga_ball_color_t b_color;
};
static char args_doc[] = "-x StartX -y StartY -u DirectionX -v DirectionY";
static char doc[] = "A bouncing ball program.";
static struct argp_option options[] = {
  {"startx",'x',"STARTX",0,"start x coordinate of circle"},
  {"starty",'y',"STARTY",0,"start y coordinate of circle"},
  {"dx",'u',"DELTAX",0,"start x direction"},
  {"dy",'v',"DELTAY",0,"start y direction"},
  {"radius",'r',"RADIUS",0,"circle radius"},
  { 0 }
};
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *args = state->input;
    int arr[3] = {0};
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
vga_ball_dir_t dir;

/* Read and print the background color */
void print_background_color() {
  vga_ball_arg_t vla;
  const char *strs[6]={
    "Undefined",
    "Leftwise",
    "Rightwise",
    "Topwise",
    "Bottomwise",
    "Cornerwise"
  };

  if (ioctl(vga_ball_fd, VGA_BALL_READ_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
  printf("%s %02x %02x %02x %d %d %d %d %d\n",
   strs[dir.next_bound],
	 vla.color.red, vla.color.green, vla.color.blue,
   vla.circle.x,vla.circle.y,
   vla.circle.radius,
   dir.dx,dir.dy);
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

void calc_next_bound(vga_ball_circle_t *circle, vga_ball_dir_t *dir)
{
  short a,b;
  a=dir->dx>0?(640-circle->radius-circle->x):(circle->radius-circle->x);
  b=dir->dy>0?(480-circle->radius-circle->y):(circle->radius-circle->y);
  a=ABS(a*dir->dy);
  b=ABS(b*dir->dx);
  if(a>b){// Depends on dy
    if(dir->dy>0)
      dir->next_bound = 4;
    else
      dir->next_bound = 3;
  }
  else if(a<b){
    if(dir->dx>0)
      dir->next_bound = 2;
    else
      dir->next_bound = 1;
  }
  else
    dir->next_bound = 5;
}

void set_radius(vga_ball_circle_t *circle, vga_ball_dir_t *dir, unsigned char r)
{
  circle->radius=r;
  calc_next_bound(circle,dir);
}

void reset_circle(vga_ball_circle_t *circle, vga_ball_dir_t *dir)
{
  if(circle->x<circle->radius)
    circle->x=circle->radius;
  if(circle->x>640-circle->radius)
    circle->x=640-circle->radius;
  if(circle->y<circle->radius)
    circle->y=circle->radius;
  if(circle->y>480-circle->radius)
    circle->y=480-circle->radius;
  dir->next_bound=0;
  calc_next_bound(circle,dir);
}

void update_circle(vga_ball_circle_t *circle, vga_ball_dir_t *dir)
{
  switch(dir->next_bound){
    case 1: // Reach Left
      if((circle->x+dir->dx)<circle->radius){
        // From calc_next_dir, we are sure that dir->dx!=0
        circle->y += (circle->radius-circle->x)/dir->dx*dir->dy;
        circle->x = circle->radius;
        printf("Reach left %d %d.\n",circle->x,circle->y);
        dir->dx = ABS(dir->dx);
        calc_next_bound(circle,dir);
      }else{
        circle->x += dir->dx;
        circle->y += dir->dy;
      }
      break;
    case 2: // Reach Right
      if((circle->x+dir->dx)>(640-circle->radius)){
        circle->y += (640-circle->radius-circle->x)/dir->dx*dir->dy;
        circle->x = 640-circle->radius;
        printf("Reach right %d %d.\n",circle->x,circle->y);
        dir->dx = -ABS(dir->dx);
        calc_next_bound(circle,dir);
      }else{
        circle->x += dir->dx;
        circle->y += dir->dy;
      }
      break;
    case 3: // Reach Top
      if((circle->y+dir->dy)<circle->radius){
        circle->x += (circle->radius-circle->y)/dir->dy*dir->dx;
        circle->y = circle->radius;
        printf("Reach top %d %d.\n",circle->x,circle->y);
        dir->dy = ABS(dir->dy);
        calc_next_bound(circle,dir);
      }else{
        circle->x += dir->dx;
        circle->y += dir->dy;
      }
      break;
    case 4: // Reach Bottom
      if((circle->y+dir->dy)>(480-circle->radius)){
        // From calc_next_dir, we are sure that dir->dx!=0
        circle->x += (480-circle->radius-circle->y)/dir->dy*dir->dx;
        circle->y = 480-circle->radius;
        printf("Reach bottom %d %d.\n",circle->x,circle->y);
        dir->dy = -ABS(dir->dy);
        calc_next_bound(circle,dir);
      }else{
        circle->x += dir->dx;
        circle->y += dir->dy;
      }
      break;
    case 5: // Reach corner
      if((circle->x+dir->dx)<circle->radius){
        // From calc_next_dir, we are sure that dir->dx!=0
        circle->y += (circle->radius-circle->x)/dir->dx*dir->dy;
        circle->x = circle->radius;
        printf("Reach corner %d %d.\n",circle->x,circle->y);
        dir->dx = -dir->dx;
        dir->dy = -dir->dy;
        calc_next_bound(circle,dir);
      }else{
        circle->x += dir->dx;
        circle->y += dir->dy;
      }
      break;
    default:
      break;    
  }
}

int main(int argc,char **argv)
{
  struct arguments args;
  //args.arg.circle.radius=16;
  argp_parse(&argp,argc,argv,0,0,&args);
  vga_ball_arg_t vla = args.arg;
  dir = args.dir;
  vga_ball_circle_t circle = vla.circle;

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
  reset_circle(&circle,&dir);
  vla.color = colors[0];
  vla.circle = circle;
  set_display(&vla);
  printf("initial state: ");
  print_background_color();

  int i=0;
  int dr=5;
  while(1) {
    ++i;
    vla.color = colors[i % COLORS ];
    vla.circle = circle;
    set_display(&vla);
    print_background_color();
    if(circle.radius>100)
      dr=-5;
    if(circle.radius<10)
      dr=5;
    set_radius(&circle,&dir,circle.radius+dr);
    update_circle(&circle,&dir);
    usleep(200000);
  }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
