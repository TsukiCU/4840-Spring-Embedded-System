#include <stdbool.h>

#ifndef _FBPUTCHAR_H
#define _FBPUTCHAR_H

#define FBOPEN_DEV -1          /* Couldn't open the device */
#define FBOPEN_FSCREENINFO -2  /* Couldn't read the fixed info */
#define FBOPEN_VSCREENINFO -3  /* Couldn't read the variable info */
#define FBOPEN_MMAP -4         /* Couldn't mmap the framebuffer memory */
#define FBOPEN_BPP -5          /* Unexpected bits-per-pixel */
#define MAX_ROWS   24          /* Number of rows on the screen */
#define MAX_COLS   64          /* Number of columns on the screen */

struct position
{
    int row;
    int col;
};

/* Some special keys. */
struct special_keys
{
    bool left_arrow;
    bool right_arrow;
    bool up_arrow;
    bool down_arrow;
    bool caps_lock;
    bool backspace;
    bool insert;
    bool esc;
    bool delete_;
};

extern int fbopen(void);
extern void fbputchar(char, int, int);
extern void fbputs(const char *, int, int);
extern void put_line(char, int);
extern void clear_screen();
extern void horizontal_line();
extern void fbputs_wrap(const char *, struct position *);

#endif
