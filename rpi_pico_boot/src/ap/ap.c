/*
 * ap.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "ap.h"


static void cliBoot(cli_args_t *args);


void apInit(void)
{
  cliOpen(_DEF_UART1, 57600);
  cliAdd("boot", cliBoot);
}

void apMain(void)
{
  uint32_t pre_time;


  while(1)
  {
    if (millis()-pre_time >= 100)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
    }

    cliMain();
  }
}

void cliBoot(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    cliPrintf("boot loader\n");
    cliPrintf("reset vreg : 0x%X\n", vreg_and_chip_reset_hw->chip_reset);
    cliPrintf("boot mode  : 0x%X\n", resetGetBootMode());

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "reset"))
  {
    resetToBoot(0);
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "jump_fw"))
  {
    void (**jump_func)(void) = (void (**)(void))(FLASH_ADDR_FW + 4);

    cliPrintf("Jump To Fw (0x%X)\n", (int)(*jump_func));
    delay(100);


    __disable_irq();

    bspDeInit();
    __set_MSP(*(uint32_t *)FLASH_ADDR_FW);
    scb_hw->vtor = FLASH_ADDR_FW;

    (*jump_func)();

    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("boot info\n");
    cliPrintf("boot reset\n");
    cliPrintf("boot jump_fw\n");
  }
}
