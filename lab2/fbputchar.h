#include <stdbool.h>
#include <stdint.h>

#ifndef _FBPUTCHAR_H
#define _FBPUTCHAR_H

#define FBOPEN_DEV -1          /* Couldn't open the device */
#define FBOPEN_FSCREENINFO -2  /* Couldn't read the fixed info */
#define FBOPEN_VSCREENINFO -3  /* Couldn't read the variable info */
#define FBOPEN_MMAP -4         /* Couldn't mmap the framebuffer memory */
#define FBOPEN_BPP -5          /* Unexpected bits-per-pixel */
#define MAX_ROWS   24          /* Number of rows on the screen */
#define MAX_COLS   64          /* Number of columns on the screen */
#define MSG_START_ROW 20       /* Starting row of message box */
#define MSG_END_ROW   23       /* End row of message box */
#define TXT_BOX_LINES (MSG_START_ROW-1)
#define TAB_SPACE   4          /* Space for TAB (4) */
#define MESSAGE_SIZE 128       /* Set it 128. may nd modify */
#define DELETE_INTERVAL 100000 /* Usleep interval */


struct position
{
    uint8_t row;
    uint8_t col;
    uint8_t buf_idx;
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

struct RGB888
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

struct msg_history
{
    char *pages[256];
    int count;
	int curr;
	int line;
};

extern int fbopen(void);
extern void fbputchar(char, int, int);
extern void fbputcharColor(char, int, int, struct RGB888);
extern void fbputs(const char *, int, int);
extern void fbputsColor(const char *, int, int, struct RGB888);
extern void put_line(char, int);
extern void clear_screen();
extern void clear_txt_box();
extern void horizontal_line();
extern void fbputs_wrap(const char *, struct position *, int);
extern char *alloc_new_text_page(struct msg_history *);
void destroy_pages(struct msg_history *);
extern void draw_cursor(struct position *, struct RGB888);
extern void print_page_info(struct msg_history *);
extern void handle_back_space(char, char, struct position*);
void *bs_continuous(void *arg);

#endif
