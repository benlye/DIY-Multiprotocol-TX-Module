#ifndef BOARD_AVR_H
#define BOARD_STM3BOARD_AVR_H2_H
#ifdef AVR_BOARD

//*******************
//***    EEPROM   ***
//*******************
#define EE_ADDR uint8_t*

#ifdef BOARD_ATMEGA328P
#include "board_atmega328p.h"
#endif
#ifdef BOARD_ORANGE
#include "board_orange.h"
#endif

#define __TELEMETRY_TX_INTERUPT_DISABLE UCSR0B &= ~_BV(UDRIE0)
#define __TELEMETRY_TX_INTERUPT_ENABLE UCSR0B |= _BV(UDRIE0)

int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
	//  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	long y;
	x -= in_min;
	y = out_max - out_min;
	y *= x;
	x = y / (in_max - in_min);
	return x + out_min;
}

// replacement millis() and micros()
// These work polled, no interrupts
// micros() MUST be called at least once every 32 milliseconds
uint16_t MillisPrecount;
uint16_t lastTimerValue;
uint32_t TotalMicros;
uint32_t TotalMillis;
uint8_t Correction;

uint32_t micros()
{
	uint16_t elapsed;
	uint8_t millisToAdd;
	uint8_t oldSREG = SREG;
	cli();
	uint16_t time = TCNT1;   // Read timer 1
	SREG = oldSREG;

	elapsed = time - lastTimerValue;
	elapsed += Correction;
	Correction = elapsed & 0x01;
	elapsed >>= 1;

	uint32_t ltime = TotalMicros;
	ltime += elapsed;
	cli();
	TotalMicros = ltime;   // Done this way for RPM to work correctly
	lastTimerValue = time;
	SREG = oldSREG;   // Still valid from above

	elapsed += MillisPrecount;
	millisToAdd = 0;

	if (elapsed > 15999)
	{
		millisToAdd = 16;
		elapsed -= 16000;
	}
	if (elapsed > 7999)
	{
		millisToAdd += 8;
		elapsed -= 8000;
	}
	if (elapsed > 3999)
	{
		millisToAdd += 4;
		elapsed -= 4000;
	}
	if (elapsed > 1999)
	{
		millisToAdd += 2;
		elapsed -= 2000;
	}
	if (elapsed > 999)
	{
		millisToAdd += 1;
		elapsed -= 1000;
	}
	TotalMillis += millisToAdd;
	MillisPrecount = elapsed;
	return TotalMicros;
}

uint32_t millis()
{
	micros();
	return TotalMillis;
}

void delayMilliseconds(unsigned long ms)
{
	uint16_t start = (uint16_t)micros();
	uint16_t lms = ms;

	while (lms > 0) {
		if (((uint16_t)micros() - start) >= 1000) {
			lms--;
			start += 1000;
		}
	}
}


#endif // AVR_BOARD
#endif // !BOARD_AVR_H
