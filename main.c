#include <interrupt.h>
#include<stdio.h>
#include<stdint.h>

#include "i2c_lcd.h"
#include "led_1.h"

#define PERIPH_BASE                (0x40000000UL)

#define RCC                        (0x40023800UL)


#define AHB1PERIPH_OFFSET          (0x00020000UL)
#define AHB1PERIPH_BASE            (AHB1PERIPH_OFFSET + PERIPH_BASE)

#define APB2_OFFSET                (0x10000UL)
#define APB2_BASE                  (PERIPH_BASE + APB2_OFFSET)

#define APB2_ENROFFSET             (0x44UL)
#define RCC_APB2ENR                (*(volatile unsigned int*)(RCC + APB2_ENROFFSET))

#define GPIOA_OFFSET               (0x0000)
#define GPIOA_BASE                 (AHB1PERIPH_BASE + GPIOA_OFFSET)

#define MODER_OFFSET               (0x00UL)
#define GPIOA_MODER                (*(volatile unsigned int *)(GPIOA_BASE + MODER_OFFSET))

#define AHB1ENR_OFFEST             (0x30UL)
#define RCC_AHB1ENR                (*(volatile unsigned int *)(RCC + AHB1ENR_OFFEST))

#define ADC1                       (0x40012000UL)

#define SQR3_OFFSET                (0x34UL)
#define ADC1_SQR3                  (*(volatile unsigned int *)(ADC1 + SQR3_OFFSET))

#define SQR1_OFFSET                (0x2CUL)
#define ADC1_SQR1                  (*(volatile unsigned int *)(ADC1 + SQR1_OFFSET))

#define CR2_OFFSET                 (0x08UL)
#define ADC_CR2                    (*(volatile unsigned int *)(ADC1 + CR2_OFFSET))

#define ADC_SROFFSET               (0x00UL)
#define ADC1_SR                     (*(volatile unsigned int *)(ADC1 + ADC_SROFFSET))

#define ADC_DROFFSET                (0x4CUL)
#define ADC1_DR                    (*(volatile unsigned int *)(ADC1 + ADC_DROFFSET))

#define ADC1_CR1                    (*(volatile unsigned int *)(ADC1 + 0x04))


#define AFRL_OFFSET                (0x20UL)
#define GPIOA_AFRL                 (*(volatile unsigned int *)(GPIOA_BASE + AFRL_OFFSET))


#define ADC_CH1                    (1U)
#define CH_LEN                     (0x00)
#define SR_EOC                     (1U<<1)


                                  //UART2-TX-PA2
#define UART2                     (APB1_BASE + 0x4400UL)

#define CR1_OFFSET                 (0x0CUL)
#define UART2_CR1                  (*(volatile uint32_t *)(UART2 + CR1_OFFSET))

#define BRR_OFFSET                  (0x08UL)
#define UART2_BRR                   (*(volatile unsigned int *)(UART2 + BRR_OFFSET ))

#define SR_OFFSET                   (0x00UL)
#define UART2_SR                    (*(volatile unsigned int *)(UART2 + SR_OFFSET))

#define DR_OFFSET                   (0x04UL)
#define UART2_DR                    (*(volatile unsigned int *)(UART2 + DR_OFFSET ))

#define APB1_OFFSET                (0x0000UL)
#define APB1_BASE                  (PERIPH_BASE + APB1_OFFSET)

#define APB1ENR_OFFSET             (0x40UL)
#define RCC_APB1ENR                (*(volatile unsigned int *)(RCC + APB1ENR_OFFSET))

#define UARTENR                   (1U<<4)
#define SYS_FR                      16000000
#define APB1_CLK                    SYS_FR
#define UART_BAUDRATE               115200



uint32_t  adc_read(void);
void start_conversion();

static void urat_set_baudrate(uint32_t periphclk, uint32_t BaudRate);
uint32_t static compute_uart_div(uint32_t periph_clk,uint32_t baurdrate);
void uart2_write(int ch);
void uart_writ_string(char *str);
void uart1_tx_init(void);

void pa1_adc_init(void){

	RCC_AHB1ENR|=(1U<<0);

	GPIOA_MODER &=~(3U<<2);

	GPIOA_MODER |=(3U<<2);


	RCC_APB2ENR |=(1U<<8);

	ADC1_SQR3 =ADC_CH1;  //chnnel1 setting

	ADC1_SQR1 =CH_LEN; // length  of channel 1channel=0x00;

	ADC_CR2 |=(1U<<0);
}

void start_conversion(){

	ADC_CR2|=(1U<<30);

}

uint32_t  adc_read(void){

	while(!(ADC1_SR & SR_EOC)){}
	return ADC1_DR;
}


int main(){


	    uint32_t raw_adc = 0;
	    char buffer[50];
	    char vdata[50];
	    uint32_t voltage_mv;

	    uart1_tx_init();

	    led_1init();


	    i2c1_init();
	    lcd_Init();
	    pa1_adc_init();

		adc1_awd_init(1000);

	    lcd_clear();
	    lcd_put_cur(0,0);
	    lcd_Send_String("SYSTEM STARING");

        for(volatile int i = 0; i < 500000; i++);
        lcd_clear();

	    uart_writ_string("ADC INIT OK\r\n");

	    start_conversion();

	    while(1)
	    {
	    	 raw_adc = ADC1_DR;
		        voltage_mv = ((raw_adc * 3300U) / 4095U);

	        if(gas_interput==1||raw_adc>1000){

	        	  lcd_put_cur(0,0);
	        	 lcd_Send_String("Interrupt Occur!");

	       sprintf(buffer, " CO2 VAL: %lu  ", (unsigned long)raw_adc);

	        			   lcd_put_cur(1,0);
	        			  lcd_Send_String(buffer);

	        	led_1bl();
			    gas_interput=0;
	        }

	        else{
	        	if(!(ADC1_CR1 &(1U<<23))){
	        	ADC1_CR1 |=(1U<<23);
	        	ADC_CR2|=(1U<<30);
	        	}
	        sprintf(buffer, "CO2 VAL: %lu ", (unsigned long)raw_adc );

	        sprintf(vdata,
	        		"Voltage: %lu mV",
					(unsigned long)voltage_mv);

	        lcd_put_cur(0,0);
	        lcd_Send_String(buffer);


	        lcd_put_cur(1,0);
	        lcd_Send_String(vdata);

	        uart_writ_string(buffer);
	        uart_writ_string(vdata);
	        for(volatile int i=0;i<20000;i++);
	    	}

	    }
	}






void uart1_tx_init(void){


	RCC_AHB1ENR|=(1U<<0);

	GPIOA_MODER &=~(3U<<4);


	GPIOA_MODER |=(2U<<4);

	GPIOA_AFRL  &=~(0XFU<<8);

	GPIOA_AFRL |=(7U<<8);

	RCC_APB1ENR |=(1U<<17);
	urat_set_baudrate(APB1_CLK, UART_BAUDRATE);

	UART2_CR1|=(1U<<13)|(1U<<3);

}


static void urat_set_baudrate(uint32_t periphclk, uint32_t BaudRate){

	UART2_BRR =compute_uart_div(periphclk,BaudRate);
}

uint32_t static compute_uart_div(uint32_t periph_clk,uint32_t baurdrate){

	return ((periph_clk + (baurdrate/2U))/baurdrate);


}
void uart2_write(int ch){
	while(!(UART2_SR&(1U<<7))){

	}
	UART2_DR=(ch&0xFF);
}

void uart_writ_string(char *str){
	while(*str){
		uart2_write(*str);
		str++;
	}
}





