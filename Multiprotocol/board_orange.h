/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 Multiprotocol is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Multiprotocol.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOARD_ORANGE_H
#define BOARD_ORANGE_H
#ifdef BOARD_ORANGE

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

#define yield()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void init()
{
	// this needs to be called before setup() or some functions won't
	// work there

	// Enable external oscillator (16MHz)
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_256CLK_gc ;
	OSC.CTRL |= OSC_XOSCEN_bm ;
	while( ( OSC.STATUS & OSC_XOSCRDY_bm ) == 0 )
		/* wait */ ;
	// Enable PLL (*2 = 32MHz)
	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | 2 ;
	OSC.CTRL |= OSC_PLLEN_bm ;
	while( ( OSC.STATUS & OSC_PLLRDY_bm ) == 0 )
		/* wait */ ;
	// Switch to PLL clock
	CPU_CCP = 0xD8 ;
	CLK.CTRL = CLK_SCLKSEL_RC2M_gc ;
	CPU_CCP = 0xD8 ;
	CLK.CTRL = CLK_SCLKSEL_PLL_gc ;

	PMIC.CTRL = 7 ;		// Enable all interrupt levels
	sei();
	
#if defined(ADCSRA)
	// set a2d prescale factor to 128
	// 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
	// XXX: this will not work properly for other clock speeds, and
	// this code should use F_CPU to determine the prescale factor.
	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);

	// enable a2d conversions
	sbi(ADCSRA, ADEN);
#endif

	// the bootloader connects pins 0 and 1 to the USART; disconnect them
	// here so they can be used as normal digital i/o; they will be
	// reconnected in Serial.begin()
#if defined(UCSRB)
	UCSRB = 0;
#elif defined(UCSR0B)
	UCSR0B = 0;
#endif

// Dip Switch inputs
	PORTA.DIRCLR = 0xFF ;
	PORTA.PIN0CTRL = 0x18 ;
	PORTA.PIN1CTRL = 0x18 ;
	PORTA.PIN2CTRL = 0x18 ;
	PORTA.PIN3CTRL = 0x18 ;
	PORTA.PIN4CTRL = 0x18 ;
	PORTA.PIN5CTRL = 0x18 ;
	PORTA.PIN6CTRL = 0x18 ;
	PORTA.PIN7CTRL = 0x18 ;
}

/* Important notes:
	- Max value is 16000µs
	- delay is not accurate due to interrupts happening */
void delayMicroseconds(unsigned int us)
{
	if (--us == 0)
		return;
	us <<= 2;	// * 4
	us -= 2;		// - 2
	__asm__ __volatile__(
		"1: sbiw %0,1" "\n\t" // 2 cycles
		"nop \n"
		"nop \n"
		"nop \n"
		"nop \n"
		"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

#define __TELEMETRY_RX_HAS_DATA USARTC0.STATUS & USART_RXCIF_bm

#endif // BOARD_ORANGE
#endif // !BOARD_ORANGE_H
