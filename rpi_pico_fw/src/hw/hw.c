/*
 * hw.c
 *
 *  Created on: 2021. 2. 21.
 *      Author: baram
 */




#include "hw.h"



extern uint32_t _flash_tag_addr;
extern uint32_t __isr_vector_addr;


__attribute__((section(".tag"))) firm_tag_t firm_tag =
    {
        .magic_number = 0xAAAA5555,

        //-- fw info
        //
        .addr_tag = (uint32_t)&_flash_tag_addr,
        .addr_fw  = (uint32_t)&__isr_vector_addr,
        .size_tag = 1024+256,


        //-- tag info
        //
    };


__attribute__((section(".version"))) firm_version_t firm_ver =
    {
        "V210224R1",
        "Raspberry Pi Pico"
    };



void hwInit(void)
{
  bspInit();

  resetInit();
  cliInit();
  ledInit();
  uartInit();
  usbInit();
  flashInit();
}
