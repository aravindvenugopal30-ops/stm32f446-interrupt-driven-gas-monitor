/*
 * i2c_lcd.h
 *
 *  Created on: Sep 17, 2026
 *      Author: ARAVIND
 */

#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include <stdio.h>


void i2c1_init(void);
void lcd_Init(void);
void lcd_Command(char cmd);
void lcd_Data(char data);
void lcd_Send_String(char*str);
void lcd_clear(void);
void lcd_put_cur(int row, int col);


#endif /* I2C_LCD_H_ */
