/*
 * ap.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "ap.h"

static void threadLed(void const *argument);
static void cliBoot(cli_args_t *args);


void apInit(void)
{
  cliOpen(_DEF_UART1, 57600);
  cliAdd("boot", cliBoot);

  osThreadDef(threadLed, threadLed, _HW_DEF_RTOS_THREAD_PRI_LED, 0, _HW_DEF_RTOS_THREAD_MEM_LED);
  if (osThreadCreate(osThread(threadLed), NULL) != NULL)
  {
    logPrintf("threadLed \t\t: OK\r\n");
  }
  else
  {
    logPrintf("threadLed \t\t: Fail\r\n");
    while(1);
  }
}

void apMain(void)
{


  while(1)
  {
    cliMain();
    delay(1);
  }
}

static void threadLed(void const *argument)
{
  (void)(argument);


  while(1)
  {
    ledToggle(_DEF_LED1);
    delay(500);
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
