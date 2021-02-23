/*
 * hw.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "hw.h"





void hwInit(void)
{
  bspInit();
  resetInit();

  if (resetGetBootMode() == RESET_MODE_FW)
  {
    hwJumpFw();
  }

  cliInit();
  ledInit();
  uartInit();
  usbInit();
  flashInit();
}

void hwJumpFw(void)
{
  void (**jump_func)(void) = (void (**)(void))(FLASH_ADDR_FW + 4);

  __disable_irq();

  bspDeInit();
  __set_MSP(*(uint32_t *)FLASH_ADDR_FW);
  scb_hw->vtor = FLASH_ADDR_FW;


  resetSetBootMode(RESET_MODE_FW);

  (*jump_func)();
}
