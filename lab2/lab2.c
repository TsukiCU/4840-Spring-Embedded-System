/*
 *
 * CSEE 4840 Lab 2 for 2019
 *
 * Hongyu Sun hs3475
 * Yunzhou Li yl5407
 */
#include "fbputchar.h"
#include <stdio.h>
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
char msgbuffer[MESSAGE_SIZE];  /* Store the message to be sent. */

/* initial position for text msg? */
struct position text_pos = {
  .txt_row = 8,
  .txt_col = 0,
};

/*
 * cursor for message box.
 */
struct position msg_pos = {
  .msg_row = MSG_START_ROW;
  .msg_col = 0;
};

struct msg_history msg_box_his = {
  .count = 0;
};

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

pthread_t network_thread;
void *network_thread_f(void *);
void *network_thread_s(void *);
void handle_keyboard_input(struct usb_keyboard_packet *packet);
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

  fbputs("Hello CSEE 4840 World!", 4, 10);
  /* Split the screen into two parts with a horizontal line. */
  horizontal_line();

  /* Open the keyboard */
  if ( (keyboard = openkeyboard(&endpoint_address)) == NULL ) {
    fprintf(stderr, "Did not find a keyboard\n");
    exit(1);
  }

  /* Allocate message box history buffer */
  msg_box_his.pages[msg_box_his.count] = alloc_new_msg_page();
  ++msg_box_his.count;

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

      /* HERE! */
      // for (uint8_t i=0; i<6; i++) keys[i] = keycode_to_char(packet.keycode[i], packet.modifiers);
      // fbputs(keys, 12, 0);
      char key;
      key = keycode_to_char(packet.keycode[0]);
      print_char(key, &msg_pos);

      //fbputs(keystate, 6, 0);
      if (packet.keycode[0] == 0x29) { /* ESC pressed? */
	break;
      }
    }
  }

  /* Terminate the network thread */
  pthread_cancel(network_thread);

  /* Wait for the network thread to finish */
  pthread_join(network_thread, NULL);

  /* Clean message box buffer*/
  for(int i=0;i<msg_box_his.count;++i)
    free(msg_box_his.pages[i]);

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

    // If exceeds current page
    if(msg_pos.msg_row+len/MAX_COLS>=MSG_START_ROW){
        // Copy to buffer
        memcpy(recvBuf,
          msg_box_his.pages[msg_box_his.count-1]+(msg_pos.msg_row-1)*MAX_COLS,
          (MSG_START_ROW-msg_pos.msg_row)*MAX_COLS
        );
        // Allocate new page
        msg_box_his.pages[msg_box_his.count] = alloc_new_msg_page();
        ++msg_box_his.count;
        recvBuf+=(MSG_START_ROW-msg_pos.msg_row)*MAX_COLS;
        // Reset message cursor
        msg_pos.msg_row = 1;
        msg_pos.msg_col = 0;
    }
    fbputs_wrap(recvBuf, &msg_pos);
    memcpy(recvBuf,
      msg_box_his.pages[msg_box_his.count-1]+(msg_pos.msg_row-1)*MAX_COLS,
      strlen(recvBuf)
    );
	++msg_pos.msg_row;
    msg_pos.msg_col=0;
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

  return NULL;
}

void handle_keyboard_input(struct usb_keyboard_packet *packet)
{

}

