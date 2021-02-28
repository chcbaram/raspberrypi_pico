/*
 * main.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "main.h"





static void threadMain(void const *argument);


int main(void)
{
  hwInit();
  apInit();

  osThreadDef(threadMain, threadMain, _HW_DEF_RTOS_THREAD_PRI_MAIN, 0, _HW_DEF_RTOS_THREAD_MEM_MAIN);
  if (osThreadCreate(osThread(threadMain), NULL) != NULL)
  {
    logPrintf("threadMain \t\t: OK\r\n");
  }
  else
  {
    logPrintf("threadMain \t\t: Fail\r\n");
    while(1);
  }

  osKernelStart();
  return 0;
}

static void threadMain(void const *argument)
{
  (void)(argument);

  apMain();
}

