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

int parse_nums(const char *arg,int *arr,int max)
{
  int *p=arr;
  int last_is_num=0;
  while(*arg){
    if(*arg>='0'&&*arg<='9'){
      if(p==arr&&!last_is_num)
        *p=0;
      *p=(*p)*10+*arg-'0';
      last_is_num=1;
    }
    else{
      if(last_is_num){
        ++p;
        if(p-arr>=max)
          return max;
      }
      last_is_num=0;
    }
    ++arg;
  }
  if(last_is_num)
    ++p;
  return p-arr;
}

struct arguments{
  vga_ball_circle_t circle;
  vga_ball_dir_t dir;
  vga_ball_color_t c_color;
  vga_ball_color_t bg_color;
  int c_random_color;
};
static char args_doc[] = "-x StartX -y StartY -u DirectionX -v DirectionY";
static char doc[] = "A bouncing ball program.";
static struct argp_option options[] = {
  {"startx",'x',"STARTX",0,"start x coordinate of circle"},
  {"starty",'y',"STARTY",0,"start y coordinate of circle"},
  {"dx",'u',"DELTAX",0,"start x direction"},
  {"dy",'v',"DELTAY",0,"start y direction"},
  {"radius",'r',"RADIUS",0,"circle radius"},
  {"speed",'s',"SPEED",0,"circle speed"},
  {"color",'c',"R,G,B",0,"circle color"},
  {"background",'b',"R,G,B",0,"background color"},
  { 0 }
};
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *args = state->input;
    int arr[3] = {0};
    switch (key) {
    case 'x': args->circle.x = atoi(arg); break;
    case 'y': args->circle.y = atoi(arg); break;
    case 'u': args->dir.vx = atoi(arg); break;
    case 'v': args->dir.vy = atoi(arg); break;
    case 'r': args->circle.radius = atoi(arg); break;
    case 's': 
      args->dir.speed = atof(arg);
      if(args->dir.speed<=0){
        argp_failure(state, 1, 0, "invalid argument for -s. See --help for more information");
        exit(ARGP_ERR_UNKNOWN);
      }
      break;
    case 'c':
      if(parse_nums(arg,arr,3)<3){
        argp_failure(state, 1, 0, "invalid argument for -c. See --help for more information");
        exit(ARGP_ERR_UNKNOWN);
      }
      if(arr[0]>255||arr[1]>255||arr[2]>255){
        argp_failure(state, 1, 0, "invalid R,G,B for -c. See --help for more information");
        exit(ARGP_ERR_UNKNOWN);
      }
      args->c_random_color = 0;
      args->c_color.red = arr[0];
      args->c_color.green = arr[1];
      args->c_color.blue = arr[2];
      printf("Circle %u %u %u\n",args->c_color.red,args->c_color.green,args->c_color.blue);
      break;
    case 'b':
      if(parse_nums(arg,arr,3)<3){
        argp_failure(state, 1, 0, "invalid argument for -b. See --help for more information");
        exit(ARGP_ERR_UNKNOWN);
      }
      if(arr[0]>255||arr[1]>255||arr[2]>255){
        argp_failure(state, 1, 0, "invalid R,G,B for -b. See --help for more information");
        exit(ARGP_ERR_UNKNOWN);
      }
      args->bg_color.red = arr[0];
      args->bg_color.green = arr[1];
      args->bg_color.blue = arr[2];
      printf("Background %u %u %u\n",args->bg_color.red,args->bg_color.green,args->bg_color.blue);
      break;
    case ARGP_KEY_END:
      if((2*args->circle.radius+ABS(args->dir.dx)>640)||
         (2*args->circle.radius+ABS(args->dir.dy)>480)){
        argp_failure(state, 1, 0, "invalid argment -u/-v/-r too large. See --help for more information");
        exit(ARGP_ERR_UNKNOWN);
      }
      break;
    default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
struct argp argp = {options, parse_opt, args_doc, doc};
vga_ball_dir_t dir;

/* Read and print the information */
void print_info() {
  vga_ball_arg_t vla;
  const char *strs[6]={
    "Undefined",
    "Leftwise",
    "Rightwise",
    "Topwise",
    "Bottomwise",
    "Cornerwise"
  };

  if (ioctl(vga_ball_fd, VGA_BALL_READ, &vla)) {
      perror("ioctl(VGA_BALL_READ) failed");
      return;
  }
  printf("%s %02x %02x %02x %d %d %d %d %d\n",
   strs[dir.next_bound],
	 vla.c_color.red, vla.c_color.green, vla.c_color.blue,
   vla.circle.x,vla.circle.y,
   dir.dx,dir.dy,
   vla.circle.radius);
   //vla.rect.left,vla.rect.top,vla.rect.right,vla.rect.bottom);
}

void set_display(const vga_ball_arg_t *arg)
{
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE, arg)) {
    perror("ioctl(VGA_BALL_WRITE) failed");
    return;
  }
}

void calc_next_bound(vga_ball_circle_t *circle, vga_ball_dir_t *dir)
{
  int a,b;
  a=dir->vx>0?(640-circle->radius-circle->x):(circle->radius-circle->x);
  b=dir->vy>0?(480-circle->radius-circle->y):(circle->radius-circle->y);
  a=ABS(a*dir->vy);
  b=ABS(b*dir->vx);
  if(a>b){// Depends on dy
    if(dir->vy>0)
      dir->next_bound = 4;
    else
      dir->next_bound = 3;
  }
  else {
    if(dir->vx>0)
      dir->next_bound = 2;
    else
      dir->next_bound = 1;
  }
  dir->corner = (a==b);
}

void set_radius(vga_ball_circle_t *circle, vga_ball_dir_t *dir, unsigned char r)
{
  switch(dir->next_bound){
    case 1: if((circle->x+dir->dx)<r) return;
    case 2: if((circle->x+dir->dx)>(640-r)) return;
    case 3: if((circle->y+dir->dy)<r) return;
    case 4: if((circle->y+dir->dy)>(480-r)) return;
    default: break;
  }
  circle->radius=r;
  calc_next_bound(circle,dir);
}


/*
 * Fast inverse square root function.
 * https://github.com/RuralAnemone/quake-3-fast-inverse-sqrt/blob/main/src/main-og.c
 */
float Q_rsqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5;
	y = number;
	i = * (long *) &y;						// evil floating point bit hack
	i = 0x5f3759df - (i >> 1);				// what the fuck?
	y = * (float *) &i;
	y = y * (threehalfs - (x2 * y * y));	// 1st iteration

	return y;
}

void reset_circle(vga_ball_circle_t *circle, vga_ball_dir_t *dir)
{
  // Left bound
  if(circle->x<circle->radius){
    circle->x=circle->radius;
    dir->dx = ABS(dir->dx);
  }
  // Right bound
  if(circle->x>640-circle->radius){
    circle->x=640-circle->radius;
    dir->dx = -ABS(dir->dx);
  }
  // Up bound
  if(circle->y<circle->radius){
    circle->y=circle->radius;
    dir->dy = ABS(dir->dy);
  }
  // Low bound
  if(circle->y>480-circle->radius){
    circle->y=480-circle->radius;
    dir->dy = -ABS(dir->dy);
  }
  dir->next_bound=0;

  float rlen = Q_rsqrt(dir->vx*dir->vx+dir->vy*dir->vy);
  dir->vx*=rlen*dir->speed;
  dir->vy*=rlen*dir->speed;

  calc_next_bound(circle,dir);
}

void update_circle(vga_ball_circle_t *circle, vga_ball_dir_t *dir)
{
  dir->fx += dir->vx;
  dir->fy += dir->vy;
  dir->dx = dir->fx;
  dir->dy = dir->fy;
  dir->fx -= dir->dx;
  dir->fy -= dir->dy;
  switch(dir->next_bound){
    case 1: // Reach Left
      if((circle->x+dir->vx)<circle->radius){
        // From calc_next_dir, we are sure that dir->dx!=0
        circle->y += (float)(circle->radius-circle->x)/dir->vx*dir->vy;
        circle->x = circle->radius;
        printf("Reach left %d %d.\n",circle->x,circle->y);
        if(dir->corner){
          dir->vx = -dir->vx;
          dir->vy = -dir->vy;
        }
        else
          dir->vx = ABS(dir->vx);
        calc_next_bound(circle,dir);
      }else{
        circle->x += dir->dx;
        circle->y += dir->dy;
      }
      break;
    case 2: // Reach Right
      if((circle->x+dir->vx)>(640-circle->radius)){
        circle->y += (640.0-circle->radius-circle->x)/dir->vx*dir->vy;
        circle->x = 640-circle->radius;
        printf("Reach right %d %d.\n",circle->x,circle->y);
        if(dir->corner){
          dir->vx = -dir->vx;
          dir->vy = -dir->vy;
        }
        else
          dir->vx = -ABS(dir->vx);
        calc_next_bound(circle,dir);
      }else{
        circle->x += dir->dx;
        circle->y += dir->dy;
      }
      break;
    case 3: // Reach Top
      if((circle->y+dir->vy)<circle->radius){
        circle->x += (float)(circle->radius-circle->y)/dir->vy*dir->vx;
        circle->y = circle->radius;
        printf("Reach top %d %d.\n",circle->x,circle->y);
        if(dir->corner){
          dir->vx = -dir->vx;
          dir->vy = -dir->vy;
        }
        else
          dir->vy = ABS(dir->vy);
        calc_next_bound(circle,dir);
      }else{
        circle->x += dir->dx;
        circle->y += dir->dy;
      }
      break;
    case 4: // Reach Bottom
      if((circle->y+dir->vy)>(480-circle->radius)){
        // From calc_next_dir, we are sure that dir->dx!=0
        circle->x += (480.0-circle->radius-circle->y)/dir->vy*dir->vx;
        circle->y = 480-circle->radius;
        printf("Reach bottom %d %d.\n",circle->x,circle->y);
        if(dir->corner){
          dir->vx = -dir->vx;
          dir->vy = -dir->vy;
        }
        else
          dir->vy = -ABS(dir->vy);
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
  args.circle.radius=16;
  args.dir.dx=0;
  args.dir.speed=1.5;
  args.dir.dy=0;
  args.c_random_color = 1;
  args.bg_color.red=args.bg_color.green=args.bg_color.blue=0xff;
  args.c_color.red=0xff;
  args.c_color.green=args.c_color.blue=0;
  argp_parse(&argp,argc,argv,0,0,&args);
  vga_ball_arg_t vla;
  vga_ball_circle_t circle = args.circle;
  dir = args.dir;
  vla.c_color = args.c_color;
  vla.bg_color = args.bg_color;

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
  };

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
  reset_circle(&circle,&dir);
  vla.circle = circle;
  set_display(&vla);
  printf("initial state: ");
  //print_info();

  int i=0;
  int dr=5;
  while(1) {
    ++i;
    if(args.c_random_color)
      vla.c_color = colors[i % COLORS ];
    vla.circle = circle;
    set_display(&vla);
    //print_info();
    if(circle.radius>100)
      dr=-5;
    if(circle.radius<10)
      dr=5;
    //set_radius(&circle,&dir,circle.radius+dr);
    update_circle(&circle,&dir);
    usleep(15000);
  }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
