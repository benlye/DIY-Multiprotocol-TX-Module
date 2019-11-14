#ifndef BOARD_STM32_H
#define BOARD_STM32_H
#ifdef STM32_BOARD

//*******************
//***    EEPROM   ***
//*******************
#define EE_ADDR uint16_t
#define eeprom_write_byte EEPROM.write
#define eeprom_read_byte EEPROM.read

#ifdef BOARD_MAPLE
#include <libmaple/usart.h>
#include <libmaple/timer.h>
#include "board_maplef103.h"
#endif

#ifdef BOARD_STM32F303
#include "board_stm32f303.h"
#endif

#endif // STM32_BOARD
#endif // !BOARD_STM32_H
