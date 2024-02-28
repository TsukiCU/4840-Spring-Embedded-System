/*
 *
 * CSEE 4840 Lab 2 for 2019
 *
 * Hongyu Sun hs3475
 * Yunzhou Li yl5407
 */
#include "fbputchar.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "usbkeyboard.h"
#include <pthread.h>
#include "usb_hid_keys.h"
#include <errno.h>
#include <string.h>
/* Update SERVER_HOST to be the IP address of
 * the chat server you are connecting to
 */
/* arthur.cs.columbia.edu */
#define SERVER_HOST "128.59.19.114"
#define SERVER_PORT 42000

#define BUFFER_SIZE 128

/*
 * References:
 *
 * http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 * http://www.thegeekstuff.com/2011/12/c-socket-programming/
 * https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2#file-usb_hid_keys-h
 *
 */

int sockfd; /* Socket file descriptor */
char keys[6];  /* keys pressed */
char pressed_keys[6];  /* old ones */
char msgbuffer[MESSAGE_SIZE+1];  /* Store the message to be sent. last one is '\0'. */

/* initial position for text msg? */
struct position text_pos = {
  .row = 1,
  .col = 0,
  .buf_idx = 0,
};

/*
 * cursor for message box.
 */
struct position msg_pos = {
  .row = MSG_START_ROW+1,
  .col = 0,
  .buf_idx = 0,
};

struct msg_history text_box_his = {
  .count = 0,
};

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

pthread_t network_thread;
void *network_thread_f(void *);
void *network_thread_s(void *);
void handle_keyboard_input(struct usb_keyboard_packet *packet);
void print_char(char key, struct position *pos, char *msg_buf);
pthread_mutex_t lock;   // lock for keyboard.


int main()
{
  int err, col;

  struct sockaddr_in serv_addr;

  struct usb_keyboard_packet packet;
  int transferred;
  char keystate[12];

  if ((err = fbopen()) != 0) {
    fprintf(stderr, "Error: Could not open framebuffer: %d\n", err);
    exit(1);
  }

  clear_screen();
  /* Draw rows of asterisks across the top and bottom of the screen */
  for (col = 0 ; col < 64 ; col++) {
    fbputchar('*', 0, col);
    fbputchar('*', 23, col);
  }

  /* Split the screen into two parts with a horizontal line. */
  horizontal_line();

  /* Open the keyboard */
  if ( (keyboard = openkeyboard(&endpoint_address)) == NULL ) {
    fprintf(stderr, "Did not find a keyboard\n");
    exit(1);
  }

  /* Allocate message box history buffer */
  /*text_box_his.pages[text_box_his.count] = alloc_new_text_page();
  if(!text_box_his.pages[text_box_his.count]){
    perror(strerror(errno));
    exit(1);
  }*/
  printf("alloc %p\n",text_box_his.pages[text_box_his.count]);
  ++text_box_his.count;

  /* Create a TCP communications socket */
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "Error: Could not create socket\n");
    exit(1);
  }

  /* Get the server address */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVER_PORT);
  if ( inet_pton(AF_INET, SERVER_HOST, &serv_addr.sin_addr) <= 0) {
    fprintf(stderr, "Error: Could not convert host IP \"%s\"\n", SERVER_HOST);
    exit(1);
  }

  /* Connect the socket to the server */
  if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "Error: connect() failed.  Is the server running?\n");
    exit(1);
  }

  /* Start the network thread */
  pthread_create(&network_thread, NULL, network_thread_f, NULL);

  /* Look for and handle keypresses */
  for (;;) {
    libusb_interrupt_transfer(keyboard, endpoint_address,
			      (unsigned char *) &packet, sizeof(packet),
			      &transferred, 0);
    if (transferred == sizeof(packet)) {
      sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0],
	      packet.keycode[1]);
      printf("%s\n", keystate);

    if (packet.keycode[0] == 0x29) { /* ESC pressed? */
	break;
      }

      /* HERE! */
      char key;
      int key_idx = -1;  // if none, just '\0' will do.
      bool old = false;  // if it's still the keycode that's already taken care of
      for(int i=0; i<3; i++) {
        for(int j=0; j<3;j++) // if it's in pressed keys list then it's old.
          if (packet.keycode[i] == pressed_keys[j]) old = true;
        if (!old && packet.keycode[i]!='\0') key_idx = i;
        old = false;
      }
      if (key_idx == -1) key = '\0';
      else key = keycode_to_char(packet.keycode[key_idx], packet.modifiers);

      if (!key) goto out;

      if (key == '\t') {  // if it's a tab
        for (int i=0; i<TAB_SPACE; i++) print_char(' ', &msg_pos, &msgbuffer);
      }

      print_char(key, &msg_pos, &msgbuffer);

out:
  memcpy(pressed_keys, packet.keycode, sizeof(packet.keycode));
    }
  }

  /* Terminate the network thread */
  pthread_cancel(network_thread);

  /* Wait for the network thread to finish */
  pthread_join(network_thread, NULL);

  /* Clean message box buffer*/
  for(int i=0;i<text_box_his.count;++i)
    free(text_box_his.pages[i]);

  return 0;
}

void *network_thread_f(void *ignored)
{
  char recvBuf[BUFFER_SIZE];
  int n;
  int len;
  /* Receive data */
  while ((n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0 ) {
    recvBuf[n] = '\0';
    printf("%s", recvBuf);
    fflush(stdout);
    len = strlen(recvBuf);

	char *p = recvBuf;
    // If exceeds current page
    if(text_pos.row+len/MAX_COLS>=MSG_START_ROW){
		printf("Buf exceed\n");
        // Copy to buffer
        /*memcpy(text_box_his.pages[text_box_his.count-1]+(text_pos.row-1)*MAX_COLS,
          p,
          (MSG_START_ROW-text_pos.row)*MAX_COLS
        );
        // Allocate new page
        text_box_his.pages[text_box_his.count] = alloc_new_text_page();
        ++text_box_his.count;*/
        // Reset message cursor
        text_pos.row = 1;
        text_pos.col = 0;
	p = recvBuf+(MSG_START_ROW-text_pos.row)*MAX_COLS;
	len-=(MSG_START_ROW-text_pos.row)*MAX_COLS;
        clear_txt_box();
    }
	printf("Msg copy text_box_his.count %d, offset %d, len %d\n",text_box_his.count,(text_pos.row-1)*MAX_COLS,len);
	printf("Msg src %p\n",text_box_his.pages[text_box_his.count-1]);
    /*memcpy(text_box_his.pages[text_box_his.count-1]+(text_pos.row-1)*MAX_COLS,
      p,
      len
    );*/
	printf("Msg print\n");
    fbputs_wrap(recvBuf, &text_pos);
	++text_pos.row;
    text_pos.col=0;
  }

  return NULL;
}

void async_send_message(char *msg)
{
  if(msg==NULL)
    return;
  if(!strlen(msg))
    return;

  /* Start the network thread for sending */
  pthread_create(&network_thread, NULL, network_thread_s, (void *)msg);
}

/*
 * Message sending thread
 */
void *network_thread_s(void *msg)
{
  char *p = msg;
  if(write(sockfd,p,strlen(p)+1)){
	char *err = strerror(errno);
    perror(err);
	// change error printing here
	fbputs(err, 2, 0);
  }
  else
    printf("Message sent: %s\n",p);
  memset(msgbuffer,0,BUFFER_SIZE+1);
  return NULL;
}

void handle_keyboard_input(struct usb_keyboard_packet *packet)
{

}

/* The buffer for message box is only 128 bytes in length.
 * Put char at a specific location with all the position modified.
 */
void print_char(char key, struct position *pos, char *msg_buf)
{
  // if it's backspace
  if (key == '\b') {
    fbputchar(' ', pos->row, pos->col-1);
    pos->col--;
    msg_buf[pos->buf_idx--] = ' ';
  }

  // if it's enter, clear everything and send message.
  else if (key == '\n') {
    async_send_message(msgbuffer);
    for (int i=MSG_START_ROW+1; i<MSG_END_ROW; i++) put_line(' ', i);
    pos->row = MSG_START_ROW+1;
    pos->col = 0;
    pos->buf_idx = 0;
  }

// if reach the end of the msg area then need a refresh.
  else if (pos->row == MSG_END_ROW-1 && pos->col == MAX_COLS-1) {
    // copy the second line up and set the second line empty.
    for (int i=0; i<MAX_COLS; i++) fbputchar(msg_buf[pos->buf_idx-MAX_COLS+i], pos->row-1, i);
    put_line(pos->row, ' ');
    pos->col=0;
    pos->buf_idx-=MAX_COLS-1;
    msg_buf[pos->buf_idx] = key;
    fbputchar(key, pos->row, pos->col);
    pos->col++;
  }
// just need to reset column.
  else if (pos->col == MAX_COLS-1) {
  msg_buf[pos->buf_idx] = key;
    pos->col = 0;
    pos->row += 1;
    fbputchar(key, pos->row, pos->col);
    pos->col++;
    pos->buf_idx++;
  }
// nothing special, just put a character here.
  else {
  msg_buf[pos->buf_idx] = key;
    fbputchar(key, pos->row, pos->col);
    pos->col++;
    pos->buf_idx++;
  }
}

