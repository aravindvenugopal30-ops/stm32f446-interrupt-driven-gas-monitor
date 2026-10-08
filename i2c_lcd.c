/*
 * i2c_lcd.c
 *
 *  Created on: Sep 17, 2026
 *      Author: ARAVIND
 */

//PB8--SCL
//PB9--SDA
#include <stdint.h>
#include "i2c_lcd.h"

#define PERIPH_BASE          (0x40000000UL)
#define APB1_OFFSET          (0x0000UL)
#define APB1_BASE            (PERIPH_BASE + APB1_OFFSET)

#define RCC_BASE             (0x40023800UL)
#define AHB1_BASE            (0x40020000UL)

#define GPIOB_OFFSET         (0x0400UL)
#define GPIOB_BASE           (GPIOB_OFFSET + AHB1_BASE)

#define AHB1ENR_OFFSET       (0x30UL)
#define RCC_AHB1ENR          (*(volatile unsigned int *)(RCC_BASE + AHB1ENR_OFFSET))

#define APB1ENR_OFFSET       (0x40UL)
#define RCC_APB1ENR          (*(volatile unsigned int *)(RCC_BASE + APB1ENR_OFFSET))

#define MODER_OFFSET         (0x00UL)
#define GPIOB_MODER          (*(volatile unsigned int *)(MODER_OFFSET + GPIOB_BASE))

#define OTYPER_OFFSET        (0x04UL)
#define GPIOB_OTYPER         (*(volatile unsigned int *)(GPIOB_BASE + OTYPER_OFFSET))

#define PUPDR_OFFSET         (0x0CUL)
#define GPIOB_PUPDR          (*(volatile unsigned int *)(GPIOB_BASE + PUPDR_OFFSET))

#define AFRH_OFFSET          (0x24UL)
#define GPIOB_AFRH           (*(volatile unsigned int *)(GPIOB_BASE + AFRH_OFFSET))

#define I2CC1                (0x40005400UL)
#define I2C_CR1              (*(volatile unsigned int *)(I2CC1 + 0x00))

#define I2C_CR2              (*(volatile unsigned int *)(I2CC1 + 0x04))

#define CCR_OFFSET           (0x1CUL)
#define I2CC1_CCR            (*(volatile unsigned int *)( CCR_OFFSET+ I2CC1))

#define TRISE_OFFSET         (0x20UL)
#define I2C_TRISE            (*(volatile unsigned int * )(I2CC1 +TRISE_OFFSET))

#define SR1_OFFSET           (0x14UL)
#define I2C_SR1              (*(volatile unsigned int *)(I2CC1 +SR1_OFFSET))

#define  DR_OFFSET           (0x10UL)
#define I2C_DR               (*(volatile unsigned int *)(I2CC1 + DR_OFFSET))

#define SR2_OFFSET           (0x18UL)
#define I2C_SR2              (*(volatile unsigned int *)(I2CC1 + SR2_OFFSET ))

#define GPIOBEN              (1U<<1)
#define I2C1EN               (1U<<21)
#define I2C_100KHZ           100
#define TRISE_VALUE          17


static void I2C_Start(void);
static void I2C_Address(uint8_t address);
static void I2C_Write(uint8_t data);
static void I2C_Stop(void);
static void LCD_Send_Internal(uint8_t data,uint8_t flags);




void i2c1_init(void){
	RCC_AHB1ENR |=GPIOBEN;
	RCC_APB1ENR |=I2C1EN;

	GPIOB_MODER &=~((3U<<16)|(3U<<18));
	GPIOB_MODER |=((2U<<16)|(2U<<18));

	GPIOB_OTYPER|=((1U<<8)|(1U<<9));

	GPIOB_PUPDR &=~((3U<<16)|(3U<<18));
	GPIOB_PUPDR |=((1U<<16)|(1U<<18));



	GPIOB_AFRH &=~((0xFU<<0)|(0xFU<<4));
	GPIOB_AFRH |=((4U<<0)|(4U<<4));

	I2C_CR1 |=(1U<<15);
	I2C_CR1 &=~(1U<<15);

	I2C_CR2 |=16;//16 MHZ
	I2CC1_CCR =I2C_100KHZ;
	I2C_TRISE=TRISE_VALUE;

	I2C_CR1|=(1U<<0);

}


static void I2C_Start(void){
	I2C_CR1 |=(1U<<10);
	I2C_CR1 |=(1U<<8);
	while(!(I2C_SR1&(1U<<0)));
}

static void I2C_Address(uint8_t address){

	I2C_DR=address;
	while(!(I2C_SR1& (1U<<1)));
	volatile uint32_t temp;
	 temp = I2C_SR1;
	 temp = I2C_SR2;
	(void)temp;
}

static void I2C_Write(uint8_t data){

	while(!(I2C_SR1 & (1U<<7)));
	I2C_DR=data;
	while(!(I2C_SR1 &(1U<<2)));
}
static void I2C_Stop(void){

	I2C_CR1|=(1U<<9);
}

static void LCD_Send_Internal(uint8_t data,uint8_t flags){

	uint8_t up=(data & 0xF0)|flags|(1U<<3);
	uint8_t low=((data<<4)&0xF0)|flags|(1U<<3);

	I2C_Start();
	I2C_Address(0x27<<1);

	I2C_Write(up |(1U<<2));
	for(volatile int i=0;i<500;i++);
	I2C_Write(up &~(1U<<2));

	I2C_Write(low |(1U<<2));
	for(volatile int i=0;i<500;i++);
	I2C_Write(low &~(1U<<2));

	I2C_Stop();
}
 void lcd_Command(char cmd){
	LCD_Send_Internal(cmd,0);
}

 void lcd_Data(char data){
	LCD_Send_Internal(data,1);
}

void lcd_Init(void){
	for(volatile int i=0;i<800000;i++);
	lcd_Command(0x30);

	for(volatile int i=0;i<1000;i++);
	lcd_Command(0x30);
	for(volatile int i=0;i<1000;i++);
	lcd_Command(0x30);
	for(volatile int i=0;i<1000;i++);
	lcd_Command(0x20);
	for(volatile int i=0;i<1000;i++);

	lcd_Command(0x33);
	lcd_Command(0x32);
	lcd_Command(0x28);
	lcd_Command(0x0C);
	lcd_Command(0X01);
	for(volatile int i=0;i<50000;i++);
}
void lcd_Send_String(char*str){

	while(*str)lcd_Data(*str++);
}
void lcd_put_cur(int row, int col) {
    uint32_t pos = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_Command(pos);
}

void lcd_clear(void) {
	lcd_Command(0x01);
}


