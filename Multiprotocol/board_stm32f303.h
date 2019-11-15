#ifndef BOARD_STM32F303_H
#define BOARD_STM32F303_H
#ifdef BOARD_STM32F303

//******************
//***   Timers   ***
//******************

// Hardware timers
#define __HARDWARE_TIMER_2 TIM2
#define __HARDWARE_TIMER_3 TIM3

// Timer bases
#define __TIMER2_BASE TIM2
#define __TIMER3_BASE TIM3

// Timer channel numbers
#define __TIMER_CH1 1
#define __TIMER_CH2 2

// Timer flags
#define __TIMER2_CHECK_CC1IF TIM2->SR & TIM_SR_CC1IF						// Check Timer2 Capture/Compare 1 interrupt flag
#define __TIMER2_CLEAR_COMP1 TIM2->SR = 0x1E5F & ~TIM_SR_CC1IF				// Clear Timer2 Capture/Compare 1 interrupt flag
#define __TIMER2_CLEAR_COMP2 TIM2->SR = 0x1E5F & ~TIM_SR_CC2IF				// Clear Timer2 Capture/Compare 2 interrupt flag
#define __TIMER_SR_CC1IF TIM_SR_CC1IF										// Capture/compare 1 interrupt flag
#define __TIMER_SR_CC2IF TIM_SR_CC2IF										// Capture/compare 2 interrupt flag
#define __TIMER_DIER_CC2IE TIM_DIER_CC2IE									// Capture/compare 2 interrupt enable flag
#define __TIMER_SR_UIF TIM_SR_UIF											// Update interrupt flag

#define OCR1A TIM2->CCR1
#define TCNT1 TIM2->CNT
#define TIFR1 TIM2->SR
#define OCF1A_bm TIMER_SR_CC1IF

//******************
//***   Serial   ***
//******************

// Enable/disable serial transmit/receive
#define __USART2_TX_DISABLE USART2->CR1 &= ~USART_CR1_TE					// Disable USART2 serial transmit
#define __USART3_RX_DISABLE USART3->CR1 &= ~USART_CR1_RE					// Disable USART3 serial receive

// Enable/disable serial interrupts
#define __USART2_RX_INTERRUPT_ENABLE USART2->CR1 |= USART_CR1_RXNEIE		// Enable USART2 receive interrupts
#define __USART2_RX_INTERRUPT_DISABLE USART2->CR1 &= ~USART_CR1_RXNEIE		// Disable USART2 receive interrupts
#define __USART3_TX_INTERUPT_ENABLE USART3->CR1 |= USART_CR1_TXEIE			// Enable USART3 transmit interrupts
#define __USART3_TX_INTERUPT_DISABLE USART3->CR1 &= ~USART_CR1_TXEIE		// Disable USART3 transmit interrupts


#define UDR0 USART2->RDR
#define UCSR0B USART2->CR1
#define RXCIE0 USART_CR1_RXNEIE
#define TXCIE0 USART_CR1_TXEIE

#define __USART2_SR USART2->ISR
#define __USART3_SR USART3->ISR

#define __USART3_DR USART3->RDR

#define __USART_SR_TXE USART_ISR_TXE

#define __USART_CR1_PCE USART_CR1_PCE

#define __TELEMETRY_RX_HAS_DATA USART2->ISR & USART_ISR_RXNE


//******************
//***    SPI     ***
//******************
SPIClass SPI_2(SDI_pin, SDO_pin, SCK_pin); 									// Create an instance of the SPI Class called SPI_2 that uses the 2nd SPI Port
#define __SPI2_BASE SPI2
#define __SPI_8BIT SPI_DATASIZE_8BIT
#define __SPI_CLK_DIV_8 SPI_CLOCK_DIV8


//******************
//***   Misc.    ***
//******************
#define __DISABLE_DEBUG_PORT												// Disables debugging pins (not used on F303)
#define __MODE_SELECT 0x0F - (uint8_t)(((GPIOA->IDR) >> 4) & 0x0F)			// Read the 16-pos rotary selector
#define __NVIC_SYS_RESET HAL_NVIC_SystemReset								// Reset the MCU


void usart2_begin(uint32_t baud, uint32_t config)
{
	__HAL_RCC_USART2_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	// Configure PA3 as alternate function USART2_RX (USART2_TX=PA2, USART2_RX=PA3 - only RX (PA3) is used)
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	LED2_output;

	// Set the baud rates and configure the ports
	USART2->BRR = 36000000 / baud;
	USART2->CR1 |= USART_CR1_UE;	// USART Enable
	USART2->CR1 |= USART_CR1_RE;	// Receiver Enable
	USART2->CR1 |= USART_CR1_TE;	// Transmitter Enable
	USART2->CR2 = 0;
	USART2->CR3 = 0;

	USART2->CR1 = (USART3->CR1 & 0B1110000111111111) | ((uint32_t)(config & 0x0F) << 9);
	USART2->CR2 = (USART3->CR2 & 0B1100111111111111) | ((uint32_t)(config & 0x30) << 8);

	USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE);
	USART2->CR1 |= USART_CR1_UE;
}

void usart3_begin(uint32_t baud, uint32_t config)
{
	__HAL_RCC_USART3_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	// Configure PB10 as alternate function USART3_TX (USART3_TX=PB10, USART3_RX=PB11 - only TX (PB10) is used)
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	USART3->BRR = 36000000 / baud;
	USART3->CR1 |= USART_CR1_UE;	// USART Enable
	USART3->CR1 |= USART_CR1_RE;	// Receiver Enable
	USART3->CR1 |= USART_CR1_TE;	// Transmitter Enable
	USART3->CR2 = 0;
	USART3->CR3 = 0;

	USART3->CR1 = (USART3->CR1 & 0B1110000111111111) | ((uint32_t)(config & 0x0F) << 9);
	USART3->CR2 = (USART3->CR2 & 0B1100111111111111) | ((uint32_t)(config & 0x30) << 8);
	
	USART3->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE);
	USART3->CR1 |= USART_CR1_UE;
}

void ISR_COMPB(HardwareTimer *timer) {};

#endif // BOARD_STM32F303
#endif // !BOARD_STM32F303_H
