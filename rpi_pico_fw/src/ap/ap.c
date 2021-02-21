/*
 * ap.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "ap.h"





void apInit(void)
{
  cliOpen(_DEF_UART1, 57600);
}

void apMain(void)
{
  uint32_t pre_time;


  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
    }

    cliMain();
  }
}
