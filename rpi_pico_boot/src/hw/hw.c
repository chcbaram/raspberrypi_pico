/*
 * hw.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "hw.h"



__attribute__((section(".version"))) firm_version_t boot_ver =
    {
        "B210227R1",
        "Bootloader"
    };


void hwInit(void)
{
  bspInit();
  resetInit();


  cliInit();
  ledInit();
  uartInit();
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
