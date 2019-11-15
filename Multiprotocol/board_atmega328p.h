#ifndef BOARD_ATMEGA328P_H
#define BOARD_ATMEGA328P_H
#ifdef BOARD_ATMEGA328P

#define __TIMER2_CLEAR_COMP1 TIFR1=OCF1A_bm			// Clear compare A=callback flag
#define __TIMER2_CHECK_CC1IF TIFR1 & OCF1A_bm

#define __TELEMETRY_RX_HAS_DATA UCSR0A & ( 1 << RXC0 )

#define __USART2_RX_INTERRUPT_ENABLE UCSR0B |= _BV(RXCIE0)
#define __USART2_RX_INTERRUPT_DISABLE UCSR0B &= ~_BV(RXCIE0)

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
		"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

void init()
{
	// this needs to be called before setup() or some functions won't work there
	sei();
}

#endif // BOARD_ATMEGA328P
#endif // !BOARD_ATMEGA328P_H
