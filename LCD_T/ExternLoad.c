/*
 * ExternLoad.c
 *
 * Created: 31.01.2015 15:35:25
 *  Author: Vadim
 */ 
#include "ExternLoad.h"

void Check_IR_PressButton()
{
	#if (SHIFT_REGISTER_IS_USED==1)
	
	if (message_unit[0].IR_ST != 0)											//была нажата кнопка на пульте NEC
	{
		if (ADDR0==DEVICE_ADDRESS)												// если пульт наш
		{
			switch (CMD0)
			{
				case BT_RED:													//красная кнопка
				message_unit[0].OUT_220_1=0;
				message_unit[0].OUT_220_2=0;
				message_unit[0].OUT_220_3=0;
				message_unit[0].LOAD_ST=1;										//поставим флаг что нужна запись в сдвиговый регистр
				//send_Uart('R');
				break;
				
				case BT_GREEN:													//зеленая кнопка
				//message_unit[0].OUT_220_1=~message_unit[0].OUT_220_1;
				message_unit[0].OUT_220_1 ^= 1;
				message_unit[0].LOAD_ST=1;
				//send_Uart('G');
				break;
				
				case BT_YELLOW:													//желтая кнопка
				message_unit[0].OUT_220_2^=1;
				message_unit[0].LOAD_ST=1;
				//send_Uart('Y');
				break;
				
				case BT_BLUE:													//голубая кнопка
				message_unit[0].OUT_220_3^=1;
				message_unit[0].LOAD_ST=1;
				//send_Uart('B');
				break;
				
				default:
				//send_Uart(13);
				//printf("you put:  %X", c);
				//send_Uart_str("you put: ");
				//send_Uart(c);
				// 		send_Uart_str(" но увы!");
				//send_Uart(13);
				//LCD_SendStr("Hi");
				break;
				
			}
			if (message_unit[0].LOAD_ST == 1)									//если требуется запись
			{
				//send_Uart(message_unit[0].CH_LAMP);
				SR_Send_8b_STCP(&message_unit[0].CH_LAMP);						//запишем в сдвиговй регистр новые данные(включим лампочки)
				message_unit[0].LOAD_ST = 0;
			}
		}
		message_unit[0].IR_ST=0;												//сбросим флаг нажатой кнопки
	}
	#endif
}