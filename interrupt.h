/*
 * interrput.h
 *
 *  Created on: Sep 24, 2026
 *      Author: ARAVIND
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_


#include <stdio.h>
#include <stdint.h>

void adc1_awd_init(uint16_t high_thershold);
void ADC_IRQHandler(void);
extern volatile uint8_t gas_interput;

#endif /* INTERRUPT_H_ */
