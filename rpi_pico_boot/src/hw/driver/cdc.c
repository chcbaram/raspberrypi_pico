/*
 * cdc.c
 *
 *  Created on: 2020. 12. 11.
 *      Author: baram
 */


#include "cdc.h"
#include "tusb.h"


#ifdef _USE_HW_CDC


static bool is_init = false;


extern void stdio_write(const char *buf, int length);
extern uint32_t stdio_available(void);
extern uint8_t stdio_read(void);



bool cdcInit(void)
{
  bool ret = true;


  is_init = true;

  return ret;
}

bool cdcIsInit(void)
{
  return is_init;
}

uint32_t cdcAvailable(void)
{
  return stdio_available();
}

uint8_t cdcRead(void)
{
  return stdio_read();
}

uint32_t cdcWrite(uint8_t *p_data, uint32_t length)
{
  stdio_write(p_data, length);
  return length;
}

uint32_t cdcGetBaud(void)
{
  cdc_line_coding_t coding;

  tud_cdc_n_get_line_coding (0, &coding);

  return coding.bit_rate;
}

#endif
