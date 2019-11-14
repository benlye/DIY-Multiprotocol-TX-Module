#ifndef BOARD_AVR_H
#define BOARD_STM3BOARD_AVR_H2_H
#ifdef AVR_BOARD

//*******************
//***    EEPROM   ***
//*******************
#define EE_ADDR uint8_t*

#ifdef BOARD_ATMEGA328P
#include "board_atmega328p.h
#endif
#ifdef BOARD_ORANGE
#include "board_orange.h"
#endif

#define __TELEMETRY_TX_INTERUPT_DISABLE UCSR0B &= ~_BV(UDRIE0)
#define __TELEMETRY_TX_INTERUPT_ENABLE UCSR0B |= _BV(UDRIE0)

#endif // AVR_BOARD
#endif // !BOARD_AVR_H
