// clk 활성화 및 configuration

#include "stm32f446xx.h"
#include "init_mcu.h"
//참고로 HSI클록 주파수는 내가 세팅가능한게 아니고 내장 RC오실레이터에 의해 결정되는
//16Mhz 고정값이다. HSE는 8Mhz로 고정 PLL을 거쳐서 180Mhz로 올리는 것

void init_mcu(void) {
	FLASH->ACR |= 0x307;

	// 1. HSI, HSE세팅
	RCC->CR |= 0x10001;
	// 2. HSI RDY될 때 까지 블로킹
	while ((RCC->CR & 0x2) == 0)
		;

	/*2. PLL 설정
	 * 0000/ 1001/ 0/1/00 00/00/ 0/011 0110 00/00 1000
	 *
	 * HSE(8Mhz)를 PLL(VCO)의 소스로 사용
	 *
	 * PLLP : 00 (PLL output clock freq=VCO output freq/PLLP, PLLP=2)
	 * PLLN : 11011000 (VCO output freq=VCO input freq*PLLN, PLLN=216)
	 * PLLM : 1000 (VCO input freq=PLL input freq/PLLM, PLLM=8)
	 *
	 * 8Mhz/8=1Mhz, 1Mhz*216=216Mhz(VCO output), 216Mhz/2=108Mhz(PLL output clock)
	 *
	 */
	RCC->PLLCFGR = 0x09403608; //PLL 108Mhz로 출력

	/*
	 * 3. PLL on, HSE clock enable
	 */
	RCC->CR = 0x01010001;
	while ((RCC->CR & 0x02000000) == 0)
		; //wait until pllrdy=1

	/*
	 * 4. 오버드라이브 설정, PWR 패리패럴 클럭을 먼저 넣고
	 */
	RCC->APB1ENR |= 0x10000000;
	PWR->CR |= 0x00010000;
	while ((PWR->CSR & 0x00010000) == 0)
		; //control/status register로 over drive모드가 준비될 때까지 블로킹

	PWR->CR |= 0x00020000; //over drive switching enable
	while ((PWR->CSR & 0x00020000) == 0)
		;

	/*
	 * 5. 패리패럴 클럭 설정
	 * sysclock을 HSI->PLL로
	 * AHB=PLL/1=108Mhz
	 * APB2=AHB/2
	 * APB1=AHB/4
	 *
	 * TIMxCLK=HCLK이고 HCLK는 AHB이고 AHB는108Mhz
	 * clock security system (CSS on)
	 *
	 * system clk(sysclk)=PLL=108Mhz
	 */
	RCC->CFGR = (2 << 0) | (4 << 13) | (4 << 10);
	RCC->DCKCFGR = 0x01000000; //TIMPRE=1이므로 TIMCLK=2*PCLK이다.!!!! 주의하라 따라서 tim clk은 108mhz이다.
	while ((RCC->CFGR & 0x0000000C) != 0x8)
		; //sysclock이 pll이 될 때까지 대기
	RCC->CR |= 0x80000;

	/*
	 * 6. 고속 신호 품질 개선을 위한 IO보상 회로 활성화
	 * 없어도 동작은 함
	 */
	RCC->APB2ENR |= 0x00004000;
	SYSCFG->CMPCR = 0x1;

	/*
	 * 7. PLL 108MHz인지 MCO로 직접 확인(PA8)
	 */
	RCC->AHB1ENR |= (1 << 2); //gpioC에 클럭 인가

	GPIOC->MODER |= (2 << 18);    // PC9 AF 모드

	GPIOC->AFR[1] |= (0 << 4);    // PC9 AF0 = MCO2

	GPIOC->OSPEEDR |= (3 << 18);  // PC9 최고속도

	RCC->CFGR |= (3 << 30);       // MCO2 소스 = PLL
	RCC->CFGR |= (6 << 27);       // MCO2 분주비 ÷4 = 27MHz check it out!!!!

}
