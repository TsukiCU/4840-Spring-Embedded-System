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
#include <ifaddrs.h>
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
char myAddr[BUFFER_SIZE];

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
  .curr = -1
};

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

pthread_t network_thread;
void *network_thread_f(void *);
void *network_thread_s(void *);
int handle_keyboard_input(struct usb_keyboard_packet *packet);
void print_char(char key, struct position *pos, char *msg_buf);
void debug_save_previous_page(char *page, int lineLen, int lines);
void update_cursor(struct position *new_pos);
void cursor_left(struct position *pos);
void cursor_right(struct position *pos);
void async_send_message(char *msg);
int message_type(char *message);
pthread_mutex_t lock;   // lock for keyboard.

int main()
{
  int err, col;

  struct sockaddr_in serv_addr;

  struct usb_keyboard_packet packet;
  int transferred;
  // char keystate[12];

  memset(myAddr, 0, BUFFER_SIZE);
  memset(msgbuffer, 0, BUFFER_SIZE+1);

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
  alloc_new_text_page(&text_box_his);
  if(!text_box_his.pages[text_box_his.count-1]){
    perror(strerror(errno));
    exit(1);
  }
  printf("alloc %p\n",text_box_his.pages[text_box_his.count-1]);
  text_box_his.curr=0;
  print_page_info(&text_box_his);

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
	  int ret = handle_keyboard_input(&packet);
      if(ret){
		if (ret == 0x29) { /* ESC pressed? */
			clear_screen();
			break;
		}
	  }

      /* HERE! */
    //   char key;
    //   int key_idx = -1;  // if none, just '\0' will do.
    //   bool old = false;  // if it's still the keycode that's already taken care of
	//   struct position new_msg_pos = msg_pos;
    //   for(int i=0; i<3; i++) {
    //     for(int j=0; j<3;j++) // if it's in pressed keys list then it's old.
    //       if (packet.keycode[i] == pressed_keys[j]) old = true;
    //     if (!old && packet.keycode[i]!='\0') key_idx = i;
    //     old = false;
    //   }
    //   if (key_idx == -1) key = '\0';
    //   else key = keycode_to_char(packet.keycode[key_idx], packet.modifiers);

    //   if (!key) goto out;

    //   if (key == '\t') {  // if it's a tab
    //     for (int i=0; i<TAB_SPACE; i++) print_char(' ', &new_msg_pos, &msgbuffer);
    //   }

      //print_char(key, &new_msg_pos, &msgbuffer);
	  //update_cursor(&new_msg_pos);

// out:
//   memcpy(pressed_keys, packet.keycode, sizeof(packet.keycode));
    }
  }

  /* Terminate the network thread */
  pthread_cancel(network_thread);

  /* Wait for the network thread to finish */
  pthread_join(network_thread, NULL);

  /* Clean message box buffer*/
  destroy_pages(&text_box_his);

  return 0;
}


/*
 * Reload text box
 * This function doesn't reload message box
 */
void reload_txt_box()
{
	char *start=text_box_his.pages[text_box_his.curr];
	int offset = 0;
	text_pos.row = 1;
	text_pos.col = 0;

	clear_txt_box();
	for(offset=0;offset<TXT_BOX_LINES*MAX_COLS;offset+=MAX_COLS){
		if(offset>0)
			if(start[offset-1]!=0)
				continue;
		fbputs_wrap(start+offset,&text_pos,message_type(start+offset));
		if(text_pos.col){
			++text_pos.row;
			text_pos.col=0;
		}
	}
	print_page_info(&text_box_his);
}

void page_left()
{
	printf("page left, curr %d\n",text_box_his.curr);
	if(text_box_his.curr){
		--text_box_his.curr;
		reload_txt_box();
	}
}

void page_right()
{
	printf("page right, curr %d\n",text_box_his.curr);
	if(text_box_his.curr<text_box_his.count-1){
		++text_box_his.curr;
		reload_txt_box();
	}
}

/*
 * Key press event handler
 * @param keycode - keycode of the pressed key (NOT ASCII)
 * @param modifiers - modifier status when pressed
 */
int handle_key_press(char keycode, char modifiers)
{
	printf("%02x pressed, modifier %02x\n",keycode,modifiers);
	struct position new_pos = msg_pos;
	switch (keycode) {
	case KEY_ESC:
		return keycode;
	case KEY_LEFT:
		if(modifiers & KEY_MOD_CTRL)
			page_left();
		else
			cursor_left(&new_pos);
		break;
	case KEY_RIGHT:
		if(modifiers & KEY_MOD_CTRL)
			page_right();
		else
			cursor_right(&new_pos);
		break;
	case KEY_UP:
		if(new_pos.row>MSG_START_ROW+1){
			--new_pos.row;
			new_pos.buf_idx-=MAX_COLS;
		}
		break;
	case KEY_DOWN:
		if(new_pos.row<MSG_END_ROW-1){
			int len = strlen(msgbuffer);
			if(new_pos.buf_idx+MAX_COLS>len){
				new_pos.row = MSG_START_ROW+1+len/MAX_COLS;
				new_pos.col = len%MAX_COLS;
				new_pos.buf_idx=len;
			}else{
				++new_pos.row;
				new_pos.buf_idx+=MAX_COLS;
			}
		}
		break;
	case KEY_TAB:
		for (int i=0; i<TAB_SPACE; i++) print_char(' ', &new_pos, msgbuffer);
		break;
	case KEY_ENTER:
		// if it's enter, clear everything and send message.
		async_send_message(msgbuffer);
		// Clear message box
		for (int i=MSG_START_ROW+1; i<MSG_END_ROW; i++) put_line(' ', i);
		new_pos.row = MSG_START_ROW+1;
		new_pos.col = 0;
		new_pos.buf_idx = 0;
		break;
	case KEY_BACKSPACE:
		handle_back_space(keycode, modifiers, &new_pos);
		break;
	default:
		print_char(keycode_to_char(keycode,modifiers), &new_pos, msgbuffer);
		break;
	}
	update_cursor(&new_pos);
	return 0;
}

void handle_back_space(char keycode, char modifiers, struct position *new_pos)
{
	if(!new_pos->buf_idx)
		return;
	// Move cursor
	cursor_left(new_pos);
	msgbuffer[new_pos->buf_idx] = 0;
	int idx = new_pos->buf_idx + 1;
	// Current cursor at the end
	if(msgbuffer[idx]==0)
		fbputchar(' ', new_pos->row, new_pos->col);
	else{
		// Current cursor in the middle
		// Delete the one before cursor
		while(msgbuffer[idx]){
			msgbuffer[idx-1]=msgbuffer[idx];
			++idx;
		}
		msgbuffer[idx-1]=0;
		// Refresh message box
		struct position tmp;
		tmp.col = 0;
		tmp.row = MSG_START_ROW + 1;
		for (int i=MSG_START_ROW+1; i<MSG_END_ROW; i++) put_line(' ', i);
		fbputs_wrap(msgbuffer, &tmp, 0);
	}
}

/*
 * Key release event handler
 * @param keycode - keycode of the released key (NOT ASCII)
 * @param modifiers - modifier status when released
 */
int handle_key_release(char keycode, char modifiers)
{
	printf("%02x released, modifier %02x\n",keycode,modifiers);
	return 0;
}

/*
 * Keyboard interrupt handler
 * We assume that there is only 1 key change at a time.
 */
int handle_keyboard_input(struct usb_keyboard_packet *packet)
{
	// Record last state
	static struct usb_keyboard_packet prev_state = {
		.modifiers = 0,
		.keycode = {0}
	};
	int r=0;
	int i;

	printf("code:");
	for(int i=0;i<6;++i)
		printf("%02x ",packet->keycode[i]);
	printf("\n");

	// Compare last state with current state
	for(i=0;i<6;++i){
		// Last state reaches end first,
		// which means a new key is pressed
		if(prev_state.keycode[i]==0)
			break;
		// Key array is ordered by their pressing order.
		// So the button pressed earlier will always stay
		// at the front of the array, and the array will
		// remain in the same order.
		// So if 2 arrays mismatch, that means a key is
		// released.
		if(prev_state.keycode[i]!=packet->keycode[i]){
			r = handle_key_release(prev_state.keycode[i],packet->modifiers);
			prev_state = *packet;
			return r;
		}
	}
	// If both 2 arrays reach the end
	// and there is no mismatch,
	// that means only modifier changes,
	// just ignore.
	if(i>=6)
		goto ret;
	// Key press
	if(packet->keycode[i]!=0)
		r = handle_key_press(packet->keycode[i],packet->modifiers);
ret:
	// Save last status.
	prev_state = *packet;
	return r;
}

/*
int is_my_address(char *buf)
{
	struct sockaddr_in my_addr;
	struct ifaddrs *ifaddr, *ifa;
	int result = 0;
	char ipAddress[INET_ADDRSTRLEN];
    int port;

	// Get port
    if (sscanf(buf, "%[^:]:%d", ipAddress, &port) != 2) {
        fprintf(stderr, "Error parsing IP address and port, %s\n",buf);
        return 0;
    }
	// Get address
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, ipAddress, &my_addr.sin_addr) <= 0) {
		fprintf(stderr, "Error: Could not convert IP \"%s\"\n", ipAddress);
		return 0;
	}

    // Retrieve the list of interface addresses
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 0;
    }
    // Iterate through the list
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
			printf("%u:%u,%u:%u\n",addr->sin_addr.s_addr,addr->sin_port,my_addr.sin_addr.s_addr,my_addr.sin_port);
            if(addr->sin_addr.s_addr==my_addr.sin_addr.s_addr&&addr->sin_port==my_addr.sin_port){
				result = 1;
				memcpy(myAddr, buf, BUFFER_SIZE);
				break;
			}
        }
    }

	// Free the memory allocated by getifaddrs
    freeifaddrs(ifaddr);

	return result;
}*/

int message_type(char *message)
{
	/*if(message==NULL)
		return 0;
	if(strlen(message)==0)
		return 0;
	if(message[0]!='<')
		return 2;*/

	/*
	char buf[BUFFER_SIZE] = {0};
	++message;
	for(int i=0;i<BUFFER_SIZE-2;++i){
		if(message[i]=='>'||message[i]==0)
			break;
		buf[i]=message[i];
	}
	buf[BUFFER_SIZE-1]=0;

	if(myAddr[0])
		return strcmp(buf,myAddr)!=0;

	return is_my_address(buf);
	*/

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
    printf("%s\n", recvBuf);
    fflush(stdout);
    len = strlen(recvBuf);

    // If exceeds current page
    if(text_box_his.line+len/MAX_COLS>=TXT_BOX_LINES){
		printf("Buf exceed\n");
        // Copy to buffer
        // memcpy(text_box_his.pages[text_box_his.count-1]+(text_box_his.line-1)*MAX_COLS,
        //   recvBuf,
        //   (MSG_START_ROW-text_box_his.line)*MAX_COLS
        // );
		debug_save_previous_page(text_box_his.pages[text_box_his.count-1], MAX_COLS, TXT_BOX_LINES);
        // Allocate new page
        alloc_new_text_page(&text_box_his);
		if(text_box_his.curr<0){
			text_box_his.curr=0;
			reload_txt_box();
		}
		if(text_box_his.count-2==text_box_his.curr){
			// Reset message cursor
			text_pos.row = 1;
			text_pos.col = 0;
			++text_box_his.curr;
			reload_txt_box();
		}
		// If not at current page, do something
		else {
			print_page_info(&text_box_his);
		}
		text_box_his.line = 0;
    }
	// printf("Msg copy text_box_his.count %d, offset %d, len %d\n",text_box_his.count,(text_pos.row-1)*MAX_COLS,len);
	// printf("Msg src %p\n",text_box_his.pages[text_box_his.count-1]);
    memcpy(text_box_his.pages[text_box_his.count-1]+text_box_his.line*MAX_COLS,
      recvBuf,
      len
    );
	// At current page, print new message.
	if(text_box_his.count-1==text_box_his.curr){
		fbputs_wrap(recvBuf, &text_pos, message_type(recvBuf));
		if(text_pos.col){
			++text_pos.row;
			text_pos.col=0;
		}
		text_box_his.line = text_pos.row - 1;
	}
	else
		text_box_his.line+=len/MAX_COLS+1;
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
  if(!write(sockfd,p,strlen(p)+1)){
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

/*
 * Remove cursor at current position (msg_pos)，
 * then update msg_pos using new value，
 * then draw the new cursor.
 * DO NOT USE OTHER METHOD to UPDATE msg_pos,
 * otherwise THE OLD CURSOR MAY NOT BE REMOVED.
 */
void update_cursor(struct position *new_pos)
{
	struct RGB888 black = {0,0,0},
				  white = {255,255,255};
	draw_cursor(&msg_pos, black);
	msg_pos=*new_pos;
	draw_cursor(&msg_pos, white);
}

/*
 * Cursor moves left
 * Will change row,col,buf_idx
 * Will rollback to last row
 * Will stop at first element
 */
void cursor_left(struct position *pos)
{
    // if it's the first row, first col
    if (pos->row == MSG_START_ROW+1 && pos->col == 0) return;
	// second row, first col
    else if (pos->col == 0) {
      pos->row--;
      pos->col = MAX_COLS-1;
	  pos->buf_idx--;
	  return;
    }
	pos->col--;
	pos->buf_idx--;
	return;
}

/*
 * Cursor moves right
 * Will change row,col,buf_idx
 * Will rolldown to next row
 * Will stop at last element or BUFFER_SIZE
 */
void cursor_right(struct position *pos)
{
	// End of the message
	if(pos->buf_idx+1>=BUFFER_SIZE)
		return;
	if(msgbuffer[pos->buf_idx]==0)
		return;
	// first row, last col
	if (pos->row == MSG_START_ROW+1 && pos->col == MAX_COLS - 1){
		++pos->row;
		pos->col=0;
	}
	else
		++pos->col;
	++pos->buf_idx;
}

/* The buffer for message box is only 128 bytes in length.
 * Put char at a specific location with all the position modified.
 */
void print_char(char key, struct position *pos, char *msg_buf)
{
  // if it's backspace
//   if (key == '\b') {
// 	cursor_left(pos);
// 	fbputchar(' ', pos->row, pos->col);
// 	msg_buf[pos->buf_idx--] = ' ';
// 	return;
//   }

  if(key==0)
	return;

  // if reach the end of the msg area then need a refresh.
  if (pos->row == MSG_END_ROW-1 && pos->col == MAX_COLS-1) {
	// Set last value first
	msg_buf[pos->buf_idx] = key;
    // copy the second line up and set the second line empty.
	memcpy(msg_buf, msg_buf+MAX_COLS, MAX_COLS);
	memset(msg_buf+MAX_COLS, 0, MAX_COLS);
	// Print first line
	for (int i=0; i<MAX_COLS; i++) fbputchar(msg_buf[i], pos->row-1, i);
	// Clear second line
	put_line(' ', pos->row);
	// Set position
	pos->row = MSG_START_ROW+1;
	pos->col = MAX_COLS - 1;
	pos->buf_idx = MAX_COLS - 1;
  }
  // TODO: when cursor is in the middle, insert values
  else if (pos->buf_idx<strlen(msg_buf)){
	int i=strlen(msg_buf);
	struct position p = *pos;
	if(i>BUFFER_SIZE-2)
		i=BUFFER_SIZE-2;
	for(;i>pos->buf_idx;--i){
		msg_buf[i]=msg_buf[i-1];
	}
	//cursor_right(&p);
	fbputs_wrap(msg_buf+pos->buf_idx, &p, 0);
  }

  // Set value to buffer
  msg_buf[pos->buf_idx] = key;
  // Display
  fbputchar(key, pos->row, pos->col);
  // Cursor moves right
  cursor_right(pos);

// // just need to reset column.
//   if (pos->col == MAX_COLS-1) {
// 	msg_buf[pos->buf_idx++] = key;
// 	fbputchar(key, pos->row, pos->col);
// 	pos->col = 0;
// 	pos->row += 1;
//   }
// // nothing special, just put a character here.
//   else {
//   msg_buf[pos->buf_idx++] = key;
//     fbputchar(key, pos->row, pos->col);
//     pos->col++;
//   }
}

void debug_save_previous_page(char *page, int lineLen, int lines)
{
	FILE *fp = fopen("test.txt","a");
	char *buf = malloc(lineLen+1);
	fprintf(fp, "PAGE\n");
	for(int i=0;i<lines;++i){
		memcpy(buf, page+lineLen*i, lineLen);
		buf[lineLen]=0;
		if(buf[strlen(buf)-1]=='\n')
			fprintf(fp, "%s",buf);
		else
			fprintf(fp, "%s\n",buf);
	}
	fprintf(fp,"\n");
	fclose(fp);
	free(buf);
}