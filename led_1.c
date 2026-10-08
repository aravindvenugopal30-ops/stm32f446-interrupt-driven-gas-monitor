/*
 * led_1.c
 *
 *  Created on: Sep 23, 2026
 *      Author: ARAVIND
 */
#include <stdio.h>
#include "led_1.h"


#define PERIPH_BASE              (0x40000000UL)

#define AHB1_OFFSET              (0x20000UL)
#define AHB1_BASE                (PERIPH_BASE + AHB1_OFFSET)

#define RCC                      (0x40023800UL)

#define GPIOA                    (0x40020000UL)


#define AHB1ENR_OFFEST           (0x30UL)
#define RCC_ABH1                 (*(volatile unsigned int *)(RCC + AHB1ENR_OFFEST))

#define GPIOA_MODER              (*(volatile unsigned int *)(GPIOA +  0x00))


#define ODR_OFFSET               (0x14UL)
#define GPIOA_OD_R                (*(volatile unsigned int *)(GPIOA + ODR_OFFSET))

#define PIN5                (1U<<5)
#define LED_PIN              PIN5

void led_1init(){

	RCC_ABH1 |=(1U<<0);

	GPIOA_MODER |=(1U<<10);
	GPIOA_MODER &=~(1U<<11);

}

void led_1bl(){

	GPIOA_OD_R |=LED_PIN;
			for(int i=0;i<100000;i++){}


			GPIOA_OD_R &=~LED_PIN;

}

