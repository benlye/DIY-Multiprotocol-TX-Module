#ifndef BOARD_ATMEGA328P_H
#define BOARD_ATMEGA328P_H
#ifdef BOARD_ATMEGA328P

#define __TIMER2_CLEAR TIFR1=OCF1A_bm			// Clear compare A=callback flag
#define __TIMER2_CHECK_CC1IF TIFR1 & OCF1A_bm

#endif // BOARD_ATMEGA328P
#endif // !BOARD_ATMEGA328P_H
