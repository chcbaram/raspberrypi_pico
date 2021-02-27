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

//extern uint32_t CDC_Itf_GetBaud(void);
//extern uint32_t CDC_Itf_TxAvailable( void );
extern uint32_t CDC_Itf_RxAvailable( void );
//extern int32_t  CDC_Itf_Write( uint8_t *p_buf, uint32_t length );
//extern uint8_t  CDC_Itf_Getch( void );
extern uint8_t  CDC_Itf_Read( void );
//extern uint32_t CDC_Itf_TxBufLengh( void );
//extern bool CDC_Itf_IsConnected(void);
//extern void CDC_Itf_Flush( void );
extern void CDC_Itf_Init(void);


bool cdcInit(void)
{
  bool ret = true;

  CDC_Itf_Init();

  is_init = true;

  return ret;
}

bool cdcIsInit(void)
{
  return is_init;
}

uint32_t cdcAvailable(void)
{
  //return stdio_available();
  return CDC_Itf_RxAvailable();
}

uint8_t cdcRead(void)
{
  //return stdio_read();
  return CDC_Itf_Read();
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
