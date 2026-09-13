/*
 * main2.c
 *
 *  Created on: Sep 13, 2026
 *      Author: 1002s
 */

#include "main2.h"
#include "pwm.h"
#include "init_mcu.h"
#include "stm32f446xx.h"

#define TEST_MODE_PWM 1
#define TEST_MODE TEST_MODE_PWM


int main(){
	init_mcu();
	init_pwm();

#if TEST_MODE==TEST_MODE_PWM
	while(1){
		TIM1->CCR1=500;
		TIM1->CCR2 =200;
	}
#endif
}
