//***************************************************************************
//  File........: lcd_lib.h
//
//  Author(s)...: Pashgan    chipenable.ru
//
//  Target(s)...: ATMega...
//
//  Compiler....: IAR, CodeVision, GNU GCC
//
//  Description.: ������� ������������������� �� �������
//
//  Data........: 20.07.13  
//
//***************************************************************************
#ifndef LCD_LIB_H
#define LCD_LIB_H

#include "compilers_4.h"

//����� � ��������� ����������� ����

//���� ������ LCD
#define LCD_PORT  LCD_DATA, F, _VIRT

#define LCD_DATA_0  D, 0, _NONE
#define LCD_DATA_1  D, 1, _NONE
#define LCD_DATA_2  D, 2, _NONE
#define LCD_DATA_3  C, 5, _NONE
#define LCD_DATA_4  D, 6, _HI
#define LCD_DATA_5  D, 7, _HI
#define LCD_DATA_6  B, 0, _HI
#define LCD_DATA_7  B, 1, _HI

//����������� ������ LCD
#define LCD_RS  D, 2, _HI
#define LCD_RW  D, 3, _HI
#define LCD_EN  D, 5, _HI  

/*____________________________________________________________________*/

//���������� ��������� ��������
#define LCD_CHECK_FL_BF             0
#define LCD_BUS_4_8_BIT             0

//��������� ������������� �������
#define LCD_ONE_TWO_LINE            1
#define LCD_FONT58_FONT511          0
#define LCD_DEC_INC_DDRAM           1
#define LCD_SHIFT_RIGHT_LEFT        0
#define LCD_DISPLAY_OFF_ON          1
#define LCD_CURSOR_OFF_ON           1
#define LCD_CURSOR_BLINK_OFF_ON     1
#define LCD_CURSOR_DISPLAY_SHIFT    0

/*_____________________����� �������________________________________*/

//�������
#define LCD_CLEAR_DISPLAY  0x01
#define LCD_RETURN_HOME    0x02

//������� �������
#define LCD_Clear() do{LCD_WriteCom(LCD_CLEAR_DISPLAY); _delay_ms(2);}while(0)   

//������� ������� � ��������� ���������
#define LCD_ReturnHome()  do{LCD_WriteCom(LCD_RETURN_HOME);}while(0)

//���������������� �������(������,����� � ������)
//#define LCD_Goto(x, y)    LCD_WriteCom(((((y)& 1)*0x40)+((x)& 15))|128)  
#define LCD_Goto(x, y)	LCD_WriteCom((( ((y)&1)*0x40)+( (((y)&3)>>1)*0x14)+(x))|128)//��� 20�4

/*___________________���������������� �������_______________________*/

void LCD_Init(void);                                  //������������� ������ � ��� �� ���������
void LCD_WriteCom(uint8_t data);                      //�������� ������� ���
void LCD_WriteData(char data);                        //������� ������ �� ���
void LCD_SendStr(char *str);                          //������� ������ �� ���

#ifdef __GNUC__
   void LCD_SendStrFl(char *str);                           //������� ������ �� ���� ������     
   void LCD_SetUserChar(uint8_t const *sym, uint8_t adr);   //��������� ���������������� ������
#else
   void LCD_SendStrFl(char __flash *str);                   //������� ������ �� ���� ������
   void LCD_SetUserChar(uint8_t __flash *sym, uint8_t adr); //��������� ���������������� ������
#endif

#endif