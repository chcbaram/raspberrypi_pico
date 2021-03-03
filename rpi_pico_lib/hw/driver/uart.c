/*
 * uart.c
 *
 *  Created on: 2020. 12. 8.
 *      Author: baram
 */


#include "uart.h"
#include "cdc.h"
#include "qbuffer.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"


#ifdef _USE_HW_UART


#define _USE_UART1


typedef struct
{
  bool     is_open;
  uint32_t baud;

#if UART_MAX_CH > 1
  //qbuffer_t qbuffer;
  uart_inst_t *p_uart;
#endif

} uart_tbl_t;

static uart_tbl_t uart_tbl[UART_MAX_CH];


#if UART_MAX_CH > 1
//#define UART_MAX_BUF_SIZE    256
//static uint8_t rx_buf[UART_MAX_CH-1][UART_MAX_BUF_SIZE];
#endif



bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 57600;
  }

  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;


  switch(ch)
  {
    case _DEF_UART1:
      uart_tbl[ch].is_open = true;
      uart_tbl[ch].baud = baud;
      ret = true;
      break;

#if UART_MAX_CH > 1
    case _DEF_UART2:

      uart_tbl[ch].is_open = true;
      uart_tbl[ch].baud    = baud;
      uart_tbl[ch].p_uart  = uart0;

      uart_init(uart_tbl[ch].p_uart, baud);

      gpio_set_function(16, GPIO_FUNC_UART); // TX
      gpio_set_function(17, GPIO_FUNC_UART); // RX


      // Set UART flow control CTS/RTS, we don't want these, so turn them off
      uart_set_hw_flow(uart_tbl[ch].p_uart, false, false);

      // Set our data format
      uart_set_format(uart_tbl[ch].p_uart, 8, 1, UART_PARITY_NONE);

      // Turn off FIFO's - we want to do this character by character
      uart_set_fifo_enabled(uart_tbl[ch].p_uart, true);

      ret = true;
      break;
#endif

#if UART_MAX_CH > 2
    case _DEF_UART3:
      uart_tbl[ch].p_huart   = &huart2;
      uart_tbl[ch].p_hdma_rx = &hdma_usart2_rx;

      huart2.Instance         = USART2;
      huart2.Init.BaudRate    = baud;
      huart2.Init.WordLength  = UART_WORDLENGTH_8B;
      huart2.Init.StopBits    = UART_STOPBITS_1;
      huart2.Init.Parity      = UART_PARITY_NONE;
      huart2.Init.Mode        = UART_MODE_TX_RX;
      huart2.Init.HwFlowCtl   = UART_HWCONTROL_NONE;
      huart2.Init.OverSampling= UART_OVERSAMPLING_16;

      HAL_UART_DeInit(&huart2);

      qbufferCreate(&uart_tbl[ch].qbuffer, &rx_buf[1][0], UART_MAX_BUF_SIZE);

      __HAL_RCC_DMA2_CLK_ENABLE();

      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        ret = false;
      }
      else
      {
        ret = true;
        uart_tbl[ch].is_open = true;

        if(HAL_UART_Receive_DMA(&huart2, (uint8_t *)&rx_buf[1][0], UART_MAX_BUF_SIZE) != HAL_OK)
        {
          ret = false;
        }

        uart_tbl[ch].qbuffer.in  = uart_tbl[ch].qbuffer.len - hdma_usart2_rx.Instance->NDTR;
        uart_tbl[ch].qbuffer.out = uart_tbl[ch].qbuffer.in;
      }
      break;
#endif
  }

  return ret;
}

bool uartClose(uint8_t ch)
{
  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcAvailable();
      break;

#if UART_MAX_CH > 1
    case _DEF_UART2:
      ret = uart_is_readable(uart_tbl[ch].p_uart);
      break;

    case _DEF_UART3:
      break;
#endif
  }

  return ret;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcRead();
      break;

#if UART_MAX_CH > 1
    case _DEF_UART2:
      ret = uart_getc(uart_tbl[ch].p_uart);
      break;

    case _DEF_UART3:
      break;
#endif
  }

  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  uint32_t pre_time;
  uint32_t tx_index;

  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcWrite(p_data, length);
      break;

#if UART_MAX_CH > 1
    case _DEF_UART2:
      tx_index = 0;
      pre_time = millis();
      while(millis()-pre_time < 100)
      {
        if (uart_is_writable(uart_tbl[ch].p_uart))
        {
          uart_get_hw(uart_tbl[ch].p_uart)->dr = p_data[tx_index];
          tx_index++;
          if (tx_index >= length)
          {
            ret = length;
            break;
          }
        }
      }
      break;

    case _DEF_UART3:
      break;
#endif
  }

  return ret;
}

uint32_t uartPrintf(uint8_t ch, char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args);


  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;


  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcGetBaud();
      break;

#if UART_MAX_CH > 1
    case _DEF_UART2:
    case _DEF_UART3:
      ret = uart_tbl[ch].baud;
      break;
#endif
  }

  return ret;
}


#endif
