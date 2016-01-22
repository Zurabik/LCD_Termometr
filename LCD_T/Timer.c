/*
 * Timer.c
 *
 * Created: 31.01.2015 15:19:15
 *  Author: Vadim
 */ 
#include "Timer.h"


//Переменные таймера 0
volatile char i_shift_n = 0;
volatile char i_shift = 0;
volatile char timer0_char_10_ms = 0;
volatile char timer0_char_100_ms = 0;
volatile char timer0_char_1_s = 0;
volatile char timer0_char_10_s = 0;
volatile char timer0_char_1_m = 0;
volatile char timer0_char_1_h = 0;
void Init_Timer0()
{
	TCNT0 = Tic_10mS;        // выставляем начальное значение TCNT1 для переполнения каждые 10мс
	TIMSK0 = (1<<TOIE0); // разрешаем прерывание по переполнению таймера
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00); // настраиваем делитель 1024
	
	
}
ISR( TIMER0_OVF_vect )//сдесь функция 100 раз в 1 сек
{
	cli();
	
	TCNT0 = Tic_10mS; //выставляем начальное значение TCNT0(10mS) 100 раз в секунду
	//send_Uart(timer0);
	//send_Uart(13);
	timer0_char_10_ms++;
	sei();
	//send_message(0);
	
}
void Get_Timer0_100mS()//сдесь функция 10 раз в 1 сек
{
	#if (SHIFT_REGISTER_IS_USED==1)
	//send_int_Uart(timer0_char_100_ms);
	SR_Send_8b_STCP(message_unit[0].CH_LAMP);
	//SR_Send_8b_STCP(i_shift_n++);
	unsigned char input_kk;
	input_kk=SR_Get_8b();
	
	printf("vhod= %u",input_kk );
	send_Uart(13);
	#endif
	/*
	 		send_Uart_str("прошла ");
	 		send_int_Uart(timer0_char_100_ms);
	 		send_Uart_str(" десятая секунды");
	 		send_Uart(13);
			 */
	//#if (ADC_IS_USED==1)
	//ADC_result();
	//#endif
	Check_IR_PressButton();
}
void Get_Timer0_1S()//сдесь функция 1 раз в 1 сек
{
	
	//#if (SHIFT_REGISTER_IS_USED==1)
	
	//SR_Send_8b_STCP(message_unit[0].CH_LAMP);
	//SR_Send_8b_STCP (0xAA);
	/*
	//I32_MESSAGE= (I32_MESSAGE>>1)+0x80000000;
	if (message_unit[0].SETUP_LAMP<=0xfe)
	{
		message_unit[0].SETUP_LAMP=(message_unit[0].SETUP_LAMP<<1)+1;
		SR_Send_8b_STCP(message_unit[0].CH_LAMP);
	}
	else
	{
		
		message_unit[0].SETUP_LAMP=0;
		SR_Send_8b_STCP(message_unit[0].CH_LAMP);
	}
		
	*/
	
	//SR_Send_8b_STCP (0x69);
	//#endif
	
	/*
	send_Uart_str("прошла ");
	send_int_Uart(timer0_int_1_s);
	send_Uart_str(" секунда");
	send_Uart(13);
	*/
//EIMSK=(1<<INT0);
//TCNT2=0;
	//send_Uart_message2(0);
	//send_message(0);
	#if (ADC_IS_USED==1)
	ADC_result();
	#endif
	
}
void Get_Timer0_10S()//сдесь функция 1 раз в 10 сек
{
	
	printf("прошло %u сек.",timer0_char_1_s );
	send_Uart(13);
	
	
	
	//Get_temp_to_message(message_unit[0].GET_TEMP);
	
	
	#if (NRF24L01_IS_USED==1)
	{
		#if (_1WIRE_IS_USED==1)
		{
			//Get_all_Temperatura();
			Get_temp(&message_unit[0].GET_TEMP);
		}
		#endif
	}
	#endif
	
	
}
void Get_Timer0_60S()//сдесь функция 1 раз в 1 мин
{
	printf("прошло %u мин.",timer0_char_1_m );
	send_Uart(13);
}
void Get_Timer0_60M()//сдесь функция 1 раз в 1 час
{
	
	printf("прошло %u час.",timer0_char_1_h );
	send_Uart(13);
	
}
void Check_Timer0()
{
	if (timer0_char_10_ms>=10)//10 Гц
	{
		timer0_char_10_ms=0;
		timer0_char_100_ms++;//(100mS) 10Hz
		Get_Timer0_100mS();
	}
	
	if (timer0_char_100_ms>=10)//1 Гц
	{
		timer0_char_100_ms=0;
		timer0_char_1_s++;
		timer0_char_10_s++;
		Get_Timer0_1S();
	}
	
	if (timer0_char_10_s>=10)//0,1 Гц; 10 сек
	{
		timer0_char_10_s=0;
		Get_Timer0_10S();
	} 
	
	if (timer0_char_1_s>=60)//60 сек
	{
		timer0_char_1_s=0;
		timer0_char_1_m++;
		Get_Timer0_60S();
	}
	if (timer0_char_1_m>=60)//60 min
	{
		timer0_char_1_m=0;
		timer0_char_1_h++;
		Get_Timer0_60M();
	}
	if (timer0_char_1_h>=24)//24 h
	{
		timer0_char_1_h=0;
	}
}

