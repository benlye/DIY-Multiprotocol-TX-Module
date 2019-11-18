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

#ifndef BOARD_MAPLEF103_H
#define BOARD_MAPLEF103_H
#ifdef BOARD_MAPLE

#define __HARDWARE_TIMER_2 2
#define __HARDWARE_TIMER_3 3

#define __DISABLE_DEBUG_PORT afio_cfg_debug_ports(AFIO_DEBUG_NONE)

#define __MODE_SELECT 0x0F - (uint8_t)(((GPIOA->regs->IDR) >> 4) & 0x0F)

#define __TIMER2_BASE TIMER2_BASE
#define __TIMER3_BASE TIMER3_BASE

#define __TIMER_CH1 TIMER_CH1
#define __TIMER_CH2 TIMER_CH2

#define __TIMER2_CLEAR_COMP1 TIMER2_BASE->SR = 0x1E5F & ~TIMER_SR_CC1IF	// Clear Timer2/Comp1 interrupt flag
#define __TIMER2_CLEAR_COMP2 TIMER2_BASE->SR = 0x1E5F & ~TIMER_SR_CC2IF	// Clear Timer2/Comp2 interrupt flag

#define __TIMER_SR_CC1IF TIMER_SR_CC1IF
#define __TIMER_SR_CC2IF TIMER_SR_CC2IF
#define __TIMER_SR_UIF TIMER_SR_UIF

#define __TIMER_DIER_CC2IE TIMER_DIER_CC2IE

#define __TIMER2_CHECK_CC1IF TIMER2_BASE->SR & TIMER_SR_CC1IF

#define __ISR_COMPB ISR_COMPB

#define __USART3_RX_DISABLE USART3_BASE->CR1 &= ~ USART_CR1_RE	// Disable telemetry receive
#define __USART2_TX_DISABLE USART2_BASE->CR1 &= ~ USART_CR1_TE	// Disable telemetry transmit

#define __TELEMETRY_RX_INTERRUPT_DISABLE USART2_BASE->CR1 &= ~USART_CR1_RXNEIE;

#define __TELEMETRY_TX_INTERUPT_DISABLE USART3_BASE->CR1 &= ~USART_CR1_TXEIE
#define __TELEMETRY_TX_INTERUPT_ENABLE USART3_BASE->CR1 |= USART_CR1_TXEIE

#define OCR1A TIMER2_BASE->CCR1
#define TCNT1 TIMER2_BASE->CNT
#define TIFR1 TIMER2_BASE->SR
#define OCF1A_bm TIMER_SR_CC1IF
#define UDR0 USART2_BASE->DR
#define UCSR0B USART2_BASE->CR1
#define RXCIE0 USART_CR1_RXNEIE_BIT
#define TXCIE0 USART_CR1_TXEIE_BIT

#define __USART2_SR USART2_BASE->SR
#define __USART3_SR USART3_BASE->SR


#define __USART3_DR USART3_BASE->DR

#define __USART_SR_TXE USART_SR_TXE

#define __USART_CR1_PCE USART_CR1_PCE_BIT

#define __TELEMETRY_RX_HAS_DATA USART2_BASE->SR & USART_SR_RXNE

#define __NVIC_SYS_RESET nvic_sys_reset

SPIClass SPI_2(2); 								//Create an instance of the SPI Class called SPI_2 that uses the 2nd SPI Port

#define __SPI2_BASE SPI2_BASE
#define __SPI_8BIT SPI_CR1_DFF_8_BIT
#define __SPI_CLK_DIV_8 SPI_CR1_BR_PCLK_DIV_8

#define __SPI_BEGIN SPI_2.begin()
#define __SPI_END SPI_2.end()

void ISR_COMPB();

void usart2_begin(uint32_t baud, uint32_t config)
{
	usart_init(USART2);
	usart_config_gpios_async(USART2, GPIOA, PIN_MAP[PA3].gpio_bit, GPIOA, PIN_MAP[PA2].gpio_bit, config);
	LED2_output;
	usart_set_baud_rate(USART2, STM32_PCLK1, baud);
	usart_enable(USART2);
}
void usart3_begin(uint32_t baud, uint32_t config)
{
	usart_init(USART3);
	usart_config_gpios_async(USART3, GPIOB, PIN_MAP[PB11].gpio_bit, GPIOB, PIN_MAP[PB10].gpio_bit, config);
	usart_set_baud_rate(USART3, STM32_PCLK1, baud);
	usart_enable(USART3);
}

#endif // BOARD_MAPLE
#endif // !BOARD_MAPLEF103_H
