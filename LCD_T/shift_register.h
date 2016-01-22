/*
 * shift_register.h
 *
 * Created: 21.08.2014 19:56:13
 *  Author: Vadim2
 */ 


#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_
#include <stddef.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>


	//PORTB=(1<<RXCIE0);
	
void SR_Send_8b (unsigned char data);
unsigned char SR_Get_8b ();
void SR_Send_8b_STCP (unsigned char data);
void SR_Get_8b_165 (unsigned char data);	
void SR_Send_str_STCP (unsigned char* data)	;
void SR_Init();

#endif /* SHIFT_REGISTER_H_ */

