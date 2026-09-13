/*
 * pwm.c
 *
 *  Created on: Sep 13, 2026
 *      Author: 1002s
 */
#include "stm32f446xx.h"
/*
 * Center Align PWM
 * TIM1 ch1,2,3 3개 사용
 *
 *PHASE1
 * PA7 : TIM1_CH1N / AF1
 * PA8 : TIM1_CH1 / AF1
 *
 *PHASE2
 * PB0 : TIM1_CH2N / AF1
 * PA9 : TIM1_CH2 / AF1
 *
 *PHASE3
 * PB1 : TIM1_CH3N / AF1
 * PA10 : TIM1_CH3 / AF1
 *
 *
 * TIM1 : APB2(54Mhz)
 * f_pwm : 20Khz
 *
 */
void init_pwm(void) {
	// 0. timer1 클럭 활성화
	RCC->APB2ENR |= 1;

	// 1. GPIOA,B configuration
	RCC->AHB1ENR |= (1 << 1) | (1 << 0);
	GPIOA->MODER |= (2 << 20) | (2 << 18) | (2 << 16) | (2 << 14);
	GPIOB->MODER |= (2 << 2) | (2 << 0);
	GPIOA->OSPEEDR |= (3 << 20) | (3 << 18) | (3 << 16) | (3 << 14);
	GPIOB->OSPEEDR |= (3 << 2) | (3 << 0);

	GPIOA->AFR[0] = (1 << 28); //PA7
	GPIOB->AFR[0] = (1 << 0) | (1 << 4);		   //PB0, PB1
	GPIOA->AFR[1] = (1 << 0) | (1 << 4) | (1 << 8);		   //PA8, PA9, PA10

	// TIM param configuartion
	TIM1->PSC=1; //TIM1CLK 54Mhz
	TIM1->ARR=1349;
	TIM1->CCMR1=0x7878;//ch1,2 OC1M은 PWM2모드 이다.
	TIM1->CCMR2=0x78;//ch3,4중 ch3만 config(3상인버터니까)
	TIM1->CCER=0x555;
	TIM1->BDTR|=TIM_BDTR_MOE|180;
	TIM1->DIER=0x1;
	TIM1->CR1=0x65;

}

