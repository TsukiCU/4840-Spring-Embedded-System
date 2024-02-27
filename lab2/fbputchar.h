#ifndef _FBPUTCHAR_H
#define _FBPUTCHAR_H

#define FBOPEN_DEV -1          /* Couldn't open the device */
#define FBOPEN_FSCREENINFO -2  /* Couldn't read the fixed info */
#define FBOPEN_VSCREENINFO -3  /* Couldn't read the variable info */
#define FBOPEN_MMAP -4         /* Couldn't mmap the framebuffer memory */
#define FBOPEN_BPP -5          /* Unexpected bits-per-pixel */
#define MAX_ROWS   24          /* Number of rows on the screen */
#define MAX_COLS   64         /* Number of columns on the screen */

// struct position
// {

// };

// struct special_keys
// {

// };

extern int fbopen(void);
extern void fbputchar(char, int, int);
extern void fbputs(const char *, int, int);
extern void put_line(char c, int row);
extern void clear_screen();
extern void horizontal_line();

#endif
