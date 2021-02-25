/*
 * bsp.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "bsp.h"
#include "reset.h"

static volatile uint32_t systick_counter = 0;

extern void clock_init(void);


void SysTick_Handler(void)
{
  systick_counter++;
}


void bspInit(void)
{
  uint32_t freq;

  //clock_init();

  //freq = CLK_GetHCLKFreq();
  //CLK_EnableSysTick(CLK_CLKSEL0_STCLKSEL_HCLK, CLK_GetHCLKFreq()/1000);

  __enable_irq();
}

void bspDeInit(void)
{

}

void delay(uint32_t ms)
{
  sleep_ms(ms);
}

uint32_t millis(void)
{
  return to_ms_since_boot(get_absolute_time());
}
