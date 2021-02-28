/*
 * bsp.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "bsp.h"


uint32_t SystemCoreClock = 125*1000000;


void isr_systick(void)
{
  osSystickHandler();
}


void bspInit(void)
{
  SystemCoreClock = clock_get_hz(clk_sys);
  __enable_irq();
}

void delay(uint32_t ms)
{
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    osDelay(ms);
  }
  else
  {
    sleep_ms(ms);
  }
}

uint32_t millis(void)
{
  return to_ms_since_boot(get_absolute_time());
}
