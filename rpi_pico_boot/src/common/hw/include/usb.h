/*
 * usb.h
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_USB_H_
#define SRC_COMMON_HW_INCLUDE_USB_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_USB

bool usbInit(void);
void usbDeInit(void);


#endif


#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_USB_H_ */
