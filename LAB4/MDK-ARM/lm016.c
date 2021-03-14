/*

Labels of LCD pins must be as follows.

LCD_D7
LCD_D6
LCD_D5
LCD_D4
LCD_RS
LCD_E


In LCD_SetCursor first argument is row's number and second argument is columns number. (they start from 1)

In LCD_CreateChar, first argument is Char's location and second argument is an array containing data for every row of character.

For printing custom char you should use PutCustom function that its argument is location of char.


****** Library works fine when using LCD Practicaly but in Proteus we must use LCD_Clear after Using LCD_Init **********


below is some code to check functionality of Library.

unsigned char y[8] = {0x1f,0x13,0x13,0x14,0x14,0x12,0x11,0x00};
LCD_Init();
LCD_Clear();
LCD_PutString("Salam bar to");
HAL_Delay(1000);
LCD_Clear();
HAL_Delay(1000);
LCD_CreateChar(0x00,y);
LCD_PutCustom(0x00);
LCD_PutChar('B');

*/

#include "lm016.h"
int counter = 0; //this variable is used to save number of characters that we have written on lcd.
int row_cursor = 1; //this variable is used to save location of cursor. ( number of row).
int col_cursor = 1; //this variable is used to save location of cursor. ( number of column).


void blink_En(void)
{
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin,GPIO_PIN_RESET);
}

void send_4bit(unsigned char data)
{
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port,LCD_D7_Pin,(GPIO_PinState) (( (data & 0x08) == 0x08))); //writing lsb or msb parts of data to lcd ports.
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port,LCD_D6_Pin,(GPIO_PinState) (( (data & 0x04) == 0x04)));
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port,LCD_D5_Pin,(GPIO_PinState) (( (data & 0x02) == 0x02)));
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port,LCD_D4_Pin,(GPIO_PinState) (( (data & 0x01) == 0x01)));
}

void lcd_write(unsigned char data) //if Rs = High  ==> writes data , Rs = Low ==> writes command.
{
	unsigned char LSB = data & 0x0F;
	unsigned char MSB = (data>>4) & 0x0F; //shift four bits to replace MSB part with LSB.
	
	HAL_Delay(1);

  send_4bit(MSB); //writing MSB part first.
	
 	blink_En();
	
  send_4bit(LSB); //now writing LSB part
  
  blink_En();
}


void lcd_command(unsigned char command)
{
   HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_RESET); //resetting rs so that lcd knows that we are sending command.
   lcd_write(command);
}

void lcd_data(unsigned char data)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET); //resetting rs so that lcd knows that we are sending data.
  lcd_write(data);
}

void LCD_Init(void)
{
	LCD_Clear();
  lcd_command(0x28);  //function test : 4 bit mode,2 lines , 5*8 dot.
  lcd_command(0x06);  //Entry mode set : Increamental, shifts the entire lcd to the right.
  lcd_command(0x0c);  //Display control : display on, cursor off,blink cursor off.
	HAL_Delay(10);
}

void LCD_PutString(char *str)
{
  while(*str != '\0') //while pointer hasn't reached end of string.
  {
    LCD_PutChar(*str); //use putchar to send to lcd.
    str++;
  }
}

void LCD_SetCursor(unsigned char row, unsigned char col)
{
    switch(row){
    case 1:
		{
      lcd_command(0x80 + col-1 ); //setting DDRAM address according to first address of each line. for line 1 address is 0x00 and for line 2 address is 0x40;
			counter = col-1;
			row_cursor = row;
			col_cursor = col;
			break;
		}
    case 2:
		{
			lcd_command( 0xC0 + col-1 ); //second line address + column
			counter = 16 + col-1 ;
			row_cursor = row;
			col_cursor = col;
      break;
		}
  }
}

void LCD_Clear(void)
{
	row_cursor = 1;
	col_cursor = 1;
	counter=0;
  lcd_command(0x01); //clearing display	
	lcd_command(0x02); //return home
	HAL_Delay(10);
}

void LCD_PutChar(unsigned char data)
{
		counter++;
		//HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET); //setting rs so that lcd know that we are sending data.
		if(counter < 16) //line 1 is not full yet.
		{
				lcd_data(data);
				col_cursor++;
		}
		else if(counter==16) //line 1 is full and we have to go to second line.
		{
				lcd_data(data);
				col_cursor=1;
				LCD_SetCursor(2,1);
			  row_cursor = 2;
		}
			else if ( counter > 16 && counter < 32) //line 1 is not full yet.
			{
				lcd_data(data);
				col_cursor++;
			}
			else //line 2 is full and we have to go to first line.
			{
				lcd_data(data);
				col_cursor=1;
				LCD_SetCursor(1,1);
				counter = 0;
				row_cursor=1;
			}
	}

	
void LCD_CreateChar(uint8_t Location,unsigned char data[])
{
	lcd_command(0x40 | Location<<3);
    for(int i = 0; i< 8 ; i++)
	{	
		lcd_data(data[i]);
	}
}

void LCD_PutCustom(uint8_t Location)
{
	LCD_SetCursor(row_cursor,col_cursor);
	lcd_data(Location);
	counter++;
}