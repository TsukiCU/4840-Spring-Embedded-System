#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>
#include <stddef.h>

typedef struct {
	unsigned char red, green, blue;
} vga_ball_color_t;

typedef struct {
  unsigned char left,top,right,bottom;
} vga_ball_rect_t;

typedef struct {
  unsigned short x,y;
  unsigned char radius;
} vga_ball_circle_t;

typedef struct {
  short dx,dy;
} vga_ball_dir_t;

typedef struct {
  vga_ball_color_t color;
  vga_ball_circle_t circle;
} vga_ball_arg_t;

#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_WRITE_BACKGROUND _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t *)
#define VGA_BALL_READ_BACKGROUND  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t *)

#endif
