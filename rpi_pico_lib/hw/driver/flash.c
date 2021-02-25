/*
 * flash.c
 *
 *  Created on: 2020. 12. 14.
 *      Author: baram
 */


#include "flash.h"
#include "cli.h"
#include "hardware/flash.h"


#ifdef _USE_HW_FLASH


#define FLASH_SECTOR_MAX            1



typedef struct
{
  uint32_t addr;
  uint32_t length;
} flash_tbl_t;


static flash_tbl_t flash_tbl[FLASH_SECTOR_MAX] =
    {
        {XIP_BASE + 0x20000,  2*1024*1024 - 128*1024},
    };


static bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length);


#ifdef _USE_HW_CLI
static void cliFlash(cli_args_t *args);
#endif


bool flashInit(void)
{

#ifdef _USE_HW_CLI
  cliAdd("flash", cliFlash);
#endif

  return true;
}

bool flashErase(uint32_t addr, uint32_t length)
{
  bool ret = false;
  uint32_t page_error;

  int16_t  start_sector_num = -1;
  uint32_t sector_count = 0;



  for (int i=0; i<FLASH_SECTOR_MAX; i++)
  {
    if (flashInSector(i, addr, length) == true)
    {
      if (start_sector_num < 0)
      {
        start_sector_num = i;
      }
      sector_count++;
    }
  }


  if (sector_count > 0)
  {
    uint32_t flash_offs;
    uint32_t flash_len;

    flash_offs = addr - XIP_BASE;

    if (flash_offs%FLASH_SECTOR_SIZE != 0)
    {
      uint32_t offset_len;

      offset_len = flash_offs%FLASH_SECTOR_SIZE;

      flash_offs = flash_offs - offset_len;
      length    += offset_len;
    }

    if (length%FLASH_SECTOR_SIZE == 0)
    {
      flash_len = length;
    }
    else
    {
      flash_len = ((length/4096) + 1) * FLASH_SECTOR_SIZE;
    }

#if !PICO_COPY_TO_RAM
    __disable_irq();
#endif
    flash_range_erase(flash_offs, flash_len);
#if !PICO_COPY_TO_RAM
    __enable_irq();
#endif

    ret = true;
  }

  return ret;
}

bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint32_t index;
  uint32_t write_length;
  uint32_t write_addr;
  uint8_t buf[FLASH_PAGE_SIZE];
  uint32_t offset;
  uint32_t sector_count = 0;


  for (int i=0; i<FLASH_SECTOR_MAX; i++)
  {
    if (flashInSector(i, addr, length) == true)
    {
      sector_count++;
    }
  }

  if (sector_count == 0)
  {
    return false;
  }


  index = 0;
  offset = addr%FLASH_PAGE_SIZE;


  if (offset != 0 || length < FLASH_PAGE_SIZE)
  {
    write_addr = addr - offset;
    memcpy(&buf[0], (void *)write_addr, FLASH_PAGE_SIZE);
    memcpy(&buf[offset], &p_data[0], constrain(FLASH_PAGE_SIZE-offset, 0, length));

#if !PICO_COPY_TO_RAM
    __disable_irq();
#endif
    flash_range_program(write_addr - XIP_BASE, (const uint8_t *)buf, FLASH_PAGE_SIZE);
#if !PICO_COPY_TO_RAM
    __enable_irq();
#endif

    if (offset == 0 && length < FLASH_PAGE_SIZE)
    {
      index += length;
    }
    else
    {
      index += (FLASH_PAGE_SIZE - offset);
    }
  }


  while(index < length)
  {
    write_length = constrain(length - index, 0, FLASH_PAGE_SIZE);

#if !PICO_COPY_TO_RAM
    __disable_irq();
#endif
    flash_range_program(addr + index - XIP_BASE, (const uint8_t *)&p_data[index], FLASH_PAGE_SIZE);
#if !PICO_COPY_TO_RAM
    __enable_irq();
#endif

    index += write_length;

    if ((length - index) > 0 && (length - index) < FLASH_PAGE_SIZE)
    {
      offset = length - index;
      write_addr = addr + index;
      memcpy(&buf[0], (void *)write_addr, FLASH_PAGE_SIZE);
      memcpy(&buf[0], &p_data[index], offset);

#if !PICO_COPY_TO_RAM
      __disable_irq();
#endif
      flash_range_program(write_addr - XIP_BASE, (const uint8_t *)buf, FLASH_PAGE_SIZE);
#if !PICO_COPY_TO_RAM
      __enable_irq();
#endif
      break;
    }
  }


  return ret;
}

bool flashRead(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint8_t *p_byte = (uint8_t *)addr;


  for (int i=0; i<length; i++)
  {
    p_data[i] = p_byte[i];
  }

  return ret;
}

bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length)
{
  bool ret = false;

  uint32_t sector_start;
  uint32_t sector_end;
  uint32_t flash_start;
  uint32_t flash_end;


  sector_start = flash_tbl[sector_num].addr;
  sector_end   = flash_tbl[sector_num].addr + flash_tbl[sector_num].length - 1;
  flash_start  = addr;
  flash_end    = addr + length - 1;


  if (sector_start >= flash_start && sector_start <= flash_end)
  {
    ret = true;
  }

  if (sector_end >= flash_start && sector_end <= flash_end)
  {
    ret = true;
  }

  if (flash_start >= sector_start && flash_start <= sector_end)
  {
    ret = true;
  }

  if (flash_end >= sector_start && flash_end <= sector_end)
  {
    ret = true;
  }

  return ret;
}






#ifdef _USE_HW_CLI
void cliFlash(cli_args_t *args)
{
  bool ret = false;



  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
#if 0
    uint8_t id_tbl[FLASH_UNIQUE_ID_SIZE_BYTES];

    cliPrintf("s1\n");
    delay(100);

    __disable_irq();
    flash_get_unique_id(id_tbl);
    __enable_irq();

    cliPrintf("s2\n");
    delay(100);


    for (int i=0; i<FLASH_UNIQUE_ID_SIZE_BYTES; i++)
    {
      cliPrintf("ID[%d] 0x%X\n", i, id_tbl[i]);
    }
#endif

    for (int i=0; i<FLASH_SECTOR_MAX; i++)
    {
      cliPrintf("0x%X : %dKB\n", flash_tbl[i].addr, flash_tbl[i].length/1024);
    }

    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "read") == true)
  {
    uint32_t addr;
    uint32_t length;

    addr   = (uint32_t)args->getData(1);
    length = (uint32_t)args->getData(2);

    for (int i=0; i<length; i++)
    {
      cliPrintf("0x%X : 0x%X\n", addr+i, *((uint8_t *)(addr+i)));
    }

    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "erase") == true)
  {
    uint32_t addr;
    uint32_t length;

    addr   = (uint32_t)args->getData(1);
    length = (uint32_t)args->getData(2);

    if (flashErase(addr, length) == true)
    {
      cliPrintf("Erase OK\n");
    }
    else
    {
      cliPrintf("Erase Fail\n");
    }
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "write") == true)
  {
    uint32_t addr;
    uint32_t data;

    addr   = (uint32_t)args->getData(1);
    data   = (uint32_t)args->getData(2);

    if (flashWrite(addr, (uint8_t *)&data, 4) == true)
    {
      cliPrintf("Write OK\n");
    }
    else
    {
      cliPrintf("Write Fail\n");
    }

    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("flash info\n");
    cliPrintf("flash read  addr length\n");
    cliPrintf("flash erase addr length\n");
    cliPrintf("flash write addr data\n");
  }
}
#endif

#endif
