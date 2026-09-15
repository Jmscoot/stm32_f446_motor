/*
 * USART TX : PA9
 * USART RX : PA10
 */

#include "stm32f446xx.h"
#include "uart.h"

void uart_init(){
	// 1. alternate function으로 쓸 gpio 클럭 인가
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
	// 2. gpio setting
	GPIOA->MODER |= ((2 << 18) | (2 << 20));
	GPIOA->AFR[1] |= ((7 << 8) | (7 << 4));

	// 3. usart 패리패럴 클럭 인가
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;

	// 4. usart setting
	USART1->CR1=0;
	USART1->CR2=0;
	USART1->CR3=0;


}
