#include "usbkeyboard.h"

#include <stdio.h>
#include <stdlib.h>

/* References on libusb 1.0 and the USB HID/keyboard protocol
 *
 * http://libusb.org
 * http://www.dreamincode.net/forums/topic/148707-introduction-to-using-libusb-10/
 * http://www.usb.org/developers/devclass_docs/HID1_11.pdf
 * http://www.usb.org/developers/devclass_docs/Hut1_11.pdf
 */

char ascii_kb_table[];
char ascii_kb_table_caps[];

/*
 * Find and return a USB keyboard device or NULL if not found
 * The argument con
 *
 */
struct libusb_device_handle *openkeyboard(uint8_t *endpoint_address) {
  libusb_device **devs;
  struct libusb_device_handle *keyboard = NULL;
  struct libusb_device_descriptor desc;
  ssize_t num_devs, d;
  uint8_t i, k;

  /* Start the library */
  if ( libusb_init(NULL) < 0 ) {
    fprintf(stderr, "Error: libusb_init failed\n");
    exit(1);
  }

  /* Enumerate all the attached USB devices */
  if ( (num_devs = libusb_get_device_list(NULL, &devs)) < 0 ) {
    fprintf(stderr, "Error: libusb_get_device_list failed\n");
    exit(1);
  }

  /* Look at each device, remembering the first HID device that speaks
     the keyboard protocol */

  for (d = 0 ; d < num_devs ; d++) {
    libusb_device *dev = devs[d];
    if ( libusb_get_device_descriptor(dev, &desc) < 0 ) {
      fprintf(stderr, "Error: libusb_get_device_descriptor failed\n");
      exit(1);
    }

    if (desc.bDeviceClass == LIBUSB_CLASS_PER_INTERFACE) {
      struct libusb_config_descriptor *config;
      libusb_get_config_descriptor(dev, 0, &config);
      for (i = 0 ; i < config->bNumInterfaces ; i++)
	for ( k = 0 ; k < config->interface[i].num_altsetting ; k++ ) {
	  const struct libusb_interface_descriptor *inter =
	    config->interface[i].altsetting + k ;
	  if ( inter->bInterfaceClass == LIBUSB_CLASS_HID &&
	       inter->bInterfaceProtocol == USB_HID_KEYBOARD_PROTOCOL) {
	    int r;
	    if ((r = libusb_open(dev, &keyboard)) != 0) {
	      fprintf(stderr, "Error: libusb_open failed: %d\n", r);
	      exit(1);
	    }
	    if (libusb_kernel_driver_active(keyboard,i))
	      libusb_detach_kernel_driver(keyboard, i);
	    libusb_set_auto_detach_kernel_driver(keyboard, i);
	    if ((r = libusb_claim_interface(keyboard, i)) != 0) {
	      fprintf(stderr, "Error: libusb_claim_interface failed: %d\n", r);
	      exit(1);
	    }
	    *endpoint_address = inter->endpoint[0].bEndpointAddress;
	    goto found;
	  }
	}
    }
  }

 found:
  libusb_free_device_list(devs, 1);

  return keyboard;
}

char keycode_to_char(uint8_t keycode, uint8_t modifier)
{
	if(keycode > 0x67)
		return 0;
	return SHIFT_PRESSED(modifier)?ascii_kb_table_caps[keycode]:ascii_kb_table[keycode];
}

char ascii_kb_table[] = {
0  , 0  , 0  , 0  , 'a', 'b', 'c', 'd',
'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
'u', 'v', 'w', 'x', 'y', 'z', '1', '2',
'3', '4', '5', '6', '7', '8', '9', '0',
0  , 0  , 0  , 0  , 0  , '-', '=', '[',
']','\\', 0  , ';','\'', '`', ',', '.',
'/', 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
0  , '1', '2', '3', '4', '5', '6', '7',
'8', '9', '0', '.', 0  , 0  , 0  , '='
};

char ascii_kb_table_caps[] = {
0  , 0  , 0  , 0  , 'A', 'B', 'C', 'D',
'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@',
'#', '$', '%', '^', '&', '*', '(', ')',
0  , 0  , 0  , 0  , 0  , '_', '+', '{',
'}', '|', 0  , ':','\"', '~', '<', '>',
'?', 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
0  , '1', '2', '3', '4', '5', '6', '7',
'8', '9', '0', '.', 0  , 0  , 0  , '='
};

char keycode_to_char(uint8_t modifier, uint8_t keycode)
{
/* a->z : 0x04 -> 0x1d */
  if (keycode >= 0x04 && keycode <= 0x1d) {
    if (SHIFT_PRESSED(modifier)) return 'A' + (keycode - 0x04);
    else return 'a' + (keycode - 0x04);
  }
}
