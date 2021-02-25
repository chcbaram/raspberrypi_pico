/*
 * hw_def.h
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "bsp.h"


#define _USE_HW_USB
#define _USE_HW_CDC
#define _USE_HW_FLASH
#define _USE_HW_RESET


#define _USE_HW_LED
#define      HW_LED_MAX_CH          1

#define _USE_HW_UART
#define      HW_UART_MAX_CH         1

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_CMD
#define      HW_CMD_MAX_DATA_LENGTH (1024)


#define FLASH_ADDR_TAG              0x10020000
#define FLASH_ADDR_FW               0x10020500
#define FLASH_ADDR_FW_VER           0x10020800

#define FLASH_ADDR_START            0x10020000
#define FLASH_ADDR_END              (FLASH_ADDR_START + (2048-128)*1024)



#endif /* SRC_HW_HW_DEF_H_ */
