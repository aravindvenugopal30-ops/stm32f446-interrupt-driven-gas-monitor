/*
 * interrup.c
 *
 *  Created on: Sep 24, 2026
 *      Author: ARAVIND
 */
#include <interrupt.h>
#include <stdio.h>
#include "led_1.h"

#define  PERIPH_BASE                     (0x40000000UL)

#define  RCC                             (0x40023800UL)


#define AHB1PERIPH_OFFSET                (0x00020000UL)
#define AHB1_BASE                        (AHB1PERIPH_OFFSET + PERIPH_BASE)

#define APB2PERIPH_OFFSET                (0x10000UL)
#define APB2_BASE                        (APB2PERIPH_OFFSET + PERIPH_BASE)

#define AHB1ENR_OFFSET                   (0x30UL)
#define RCC_AHB1                         (*(volatile unsigned int *)(RCC + AHB1ENR_OFFSET))

#define APB2ENR_OFFSET                  (0x44UL)
#define RCC_APB2                        (*(volatile unsigned int *)(RCC + APB2ENR_OFFSET))

#define ADC1_BASE                       (APB2_BASE + 0x2000UL)

#define ADC1_SR                         (*(volatile unsigned int *)(ADC1_BASE + 0x00UL))
#define ADC1_CR1                        (*(volatile unsigned int *)(ADC1_BASE + 0x04UL))
#define ADC1_CR2                        (*(volatile unsigned int *)(ADC1_BASE + 0x08UL))
#define ADC1_HTR                        (*(volatile unsigned int *)(ADC1_BASE + 0x24UL))
#define ADC1_LTR                        (*(volatile unsigned int *)(ADC1_BASE + 0x28UL))

#define ADC_DROFFSET                (0x4CUL)
#define ADC1_DR                    (*(volatile unsigned int *)(ADC1_BASE + ADC_DROFFSET))


#define NVIC_ISER0                      (*(volatile unsigned int *)(0xE000E100UL))

volatile uint8_t gas_interput=0;

void adc1_awd_init(uint16_t high_thershold){

	RCC_APB2 |=(1U<<8);

	ADC1_HTR=high_thershold;
	ADC1_LTR=0;

	ADC1_CR1 |=(1U<<6);
	ADC1_CR1 |=(1U<<9);
	ADC1_CR1 |=(1U<<23);


	ADC1_CR1 &=~(0x1FU<<0);
	ADC1_CR1 |=(1U<<0);

	NVIC_ISER0|=(1U<<18);

	ADC1_CR2 |=(1U<<1);
	ADC1_CR2 |=(1U<<0);
	ADC1_CR2 |=(1U<<30);

}

void ADC_IRQHandler(void){

	if(ADC1_SR & (1U<<0)){

			    // led_1bl();
			     gas_interput=1;

			     ADC1_SR =~(1U<<0);

			     ADC1_CR1 &=~(1U<<23);

			     (void)ADC1_DR;

			     *(volatile unsigned int *)(0xE000E200UL)|=(1U<<18);

	}

}








