/*
 * bsp.h
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"

#define _USE_LOG_PRINT    1

#if _USE_LOG_PRINT
#define logPrintf(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#else
#define logPrintf(fmt, ...)
#endif

#include "pico/time.h"


void bspInit(void);

void delay(uint32_t ms);
uint32_t millis(void);

#endif /* SRC_BSP_BSP_H_ */
