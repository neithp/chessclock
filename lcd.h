/*
 * lcd.h
 *
 *  Created on: Apr 23, 2022
 *      Author: truon
 */

#ifndef LCD_H_
#define LCD_H_

#define LCDPORT         GPIO_PORTB_BASE
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOB
#define RS              GPIO_PIN_0
#define E               GPIO_PIN_1
#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7


void LCD_init(void);                                //LCD initialization
void LCD_Command(unsigned char c);                  //Send command
void LCD_Show(unsigned char d);                     //Show a char

void LCD_Clear(void);
void LCD_Print(char x, char y, char *s);//Clear the screen
void LCD_Print2Ln(char *s, char *d);                   //Print 2 lines
void LCD_PrintLn(char i, char *s);                  //Print specific line
void LCD_PrintJustify(char i, char *s, char *d);    //Print specific line floated left and floated right text
void LCD_Cursor(char x, char y);                    //Set cursor
void LCD_Yaz(char* s);
void LCD_Printa(char x, char y, int a);
void LCD_Prints(char x, char y, char *s);
#endif /* LCD_H_ */
