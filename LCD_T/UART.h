/*
 * UART.c
 *
 * Created: 02.03.2014 18:33:12
 *  Author: Vadim2
 */ 
//#define F_CPU 18432000UL // CPU Speed
//#define BAUD 115200
#ifndef UART_H_
#define UART_H_
#include "settings.h"
#if (MODE_OF_OPERATION==0)//Передатчик (ведущий)
	#define BAUD 115200
	#define MYUBRR F_CPU/16/BAUD-1 // 18.432 MHz
#endif
#if (MODE_OF_OPERATION==1)//Приемник (ведомый)
	#define BAUD 115200
	#define MYUBRR F_CPU/16/BAUD  // 16 MHz	
#endif
#if (MODE_OF_OPERATION==2)//Уличный датчик
#define BAUD 57600
#define MYUBRR F_CPU/16/BAUD  // 8 MHz
#endif

//#define MYUBRR F_CPU/16/BAUD-1
//#include <stdio.h>
#include "fifo.h"
#include <stdio.h>
#include <avr/interrupt.h>

//#include "Message.h"
//volatile char USART_TX_data[10];
//volatile char USART_RX_data[2];
//volatile int num_b=0;
//volatile int flag_RX=0;
//функция инициализации USART
void USART_Init( unsigned int ubrr);

int uart_putc(  char c, FILE *file );

int uart_getc( FILE* file );

void send_Uart(unsigned char c);//   Отправка байта

void send_Uart_str(unsigned char* s);//  Отправка строки

void send_int_Uart(unsigned int c);//    Отправка числа от 0000 до 9999

#endif
