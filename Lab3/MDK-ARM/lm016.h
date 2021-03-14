
#ifndef _LCD_H
#define _LCD_H

#include "gpio.h"

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(unsigned char row, unsigned char col);
void LCD_PutString(char *str);
void lcd_write(unsigned char data);
void lcd_command(unsigned char command);
void LCD_PutChar(unsigned char data);
void send_4bit(unsigned char data);
void LCD_CreateChar(uint8_t Location,unsigned char data[]);
void LCD_PutCustom(uint8_t Location);
void blin_En(void);
void lcd_data(unsigned char data);

#endif
