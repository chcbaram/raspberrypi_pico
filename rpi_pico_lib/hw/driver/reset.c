/*
 * reset.c
 *
 *  Created on: 2021. 2. 23.
 *      Author: baram
 */






#include "reset.h"


#ifdef _USE_HW_RESET


#define RESET_BOOT_RAM_ADDR         0x20000000


typedef struct
{
  uint32_t boot_mode;
  uint32_t boot_mode_xor;

  uint32_t reset_src;
  uint32_t reset_src_xor;
} reset_boot_mode_t;



static reset_boot_mode_t *p_boot_mode = (reset_boot_mode_t *)RESET_BOOT_RAM_ADDR;
static uint32_t run_timeout_count = 0;


void resetISR(void)
{
  if (run_timeout_count > 0)
  {
    run_timeout_count--;

    if (run_timeout_count == 0)
    {
      resetToRunBoot();
    }
  }
}

void resetInit(void)
{
}

void resetSetBootMode(uint32_t mode)
{
  p_boot_mode->boot_mode = mode;
  p_boot_mode->boot_mode_xor = mode ^ 0xFFFFFFFF;
}

uint32_t resetGetBootMode(void)
{
  uint32_t boot_mode;

  boot_mode = p_boot_mode->boot_mode_xor ^ 0xFFFFFFFF;

  if (p_boot_mode->boot_mode != boot_mode)
  {
    resetSetBootMode(0);
  }

  return p_boot_mode->boot_mode;
}

void resetToBoot(uint32_t timeout)
{
  if (timeout == 0)
  {
    resetToRunBoot();
  }
  else
  {
    run_timeout_count = timeout;
  }
}

void resetToRunBoot(void)
{
  resetSetBootMode(RESET_MODE_BOOT);
  NVIC_SystemReset();
}

void resetToRunFw(void)
{
  resetSetBootMode(RESET_MODE_FW);
  NVIC_SystemReset();
}


#endif
