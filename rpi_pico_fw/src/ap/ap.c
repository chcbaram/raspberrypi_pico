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
    if (millis()-pre_time >= 500)
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
    cliPrintf("firmware\n");
    cliPrintf("boot mode  : 0x%X\n", resetGetBootMode());
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "jump_fw"))
  {
    cliPrintf("Jump To Fw\n");
    delay(100);

    resetToRunFw();
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "jump_boot"))
  {
    cliPrintf("Jump To Boot\n");
    delay(100);

    resetToRunBoot();
    ret = true;
  }
  if (ret == false)
  {
    cliPrintf("boot info\n");
    cliPrintf("boot jump_fw\n");
    cliPrintf("boot jump_boot\n");
  }
}
