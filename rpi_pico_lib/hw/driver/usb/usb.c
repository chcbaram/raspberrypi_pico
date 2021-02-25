/*
 * usb.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "usb.h"
#include "cdc.h"
#include "tusb.h"
#include "pico/stdio_usb.h"


#ifdef _USE_HW_USB

bool usbInit(void)
{
  bool ret;


  ret = stdio_usb_init();

  cdcInit();

  return ret;
}

void usbDeInit(void)
{
}


#endif
