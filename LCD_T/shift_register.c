/*
 * shift_register.c
 *
 * Created: 31.01.2015 15:43:17
 *  Author: Vadim
 */ 
#include "shift_register.h"


#define SHCP_DDR		DDRB
#define SHCP_PIN		PINB0
#define SHCP_PORT		PORTB
#define SHCP_HIGH()		SHCP_PORT |=  _BV(SHCP_PIN);
#define SHCP_LOW()		SHCP_PORT &= ~_BV(SHCP_PIN);

#define STCP_DDR		DDRD
#define STCP_PIN		PIND6
#define STCP_PORT		PORTD
#define STCP_HIGH()		STCP_PORT |=  _BV(STCP_PIN);
#define STCP_LOW()		STCP_PORT &= ~_BV(STCP_PIN);

#define DS_DDR			DDRD
#define DS_PIN			PIND7
#define DS_PORT			PORTD
#define DS_HIGH()		DS_PORT |=  _BV(DS_PIN);
#define DS_LOW()		DS_PORT &= ~_BV(DS_PIN);

#define DG_DDR			DDRD
#define DG_PIN			PIND5
#define DG_PORT			PORTD


#define SCEPL_DDR		DDRD
#define SCEPL_PIN		PIND4
#define SCEPL_PORT		PORTD
#define SCEPL_HIGH()	SCEPL_PORT |=  _BV(SCEPL_PIN);
#define SCEPL_LOW()		SCEPL_PORT &= ~_BV(SCEPL_PIN);



	void SR_Send_8b (unsigned char data)
	{
		unsigned char counter;
		for (counter = 0; counter < 8; counter++)				// цикл для 8 битов
		{
			
			if (data & 0x80)								// Проверяем крайний левый бит если он равен 1
			{
				//DS_PORT=(1<<DS_PIN);
				DS_HIGH();									//то записываем в линию данных 1
			}
			else
			{
				//DS_PORT=(0<<DS_PIN);
				DS_LOW();									// иначе записываем 0
			}
			
			//SHCP_PORT = (1<<SHCP_PIN);									//Дергаем ногой чтоб пропихнуть бит в регистр
			SHCP_HIGH();
			asm("nop");										//небольшая пауза
			//SHCP_PORT = (0<<SHCP_PIN);
			SHCP_LOW();
			data = data << 1;						// Сдвигаем все биты data влево на один бит
		}
		
	}
	
	unsigned char SR_Get_8b ()
	{
		SCEPL_LOW();//0
		unsigned char data;
		unsigned char counter;
		unsigned char i;
		
		for (counter = 0; counter < 8; counter++)				// цикл для 8 битов
		{
			SHCP_PORT = (1<<SHCP_PIN);									//Дергаем ногой чтоб пропихнуть бит в регистр
			asm("nop");										//небольшая пауза
			SHCP_PORT = (0<<SHCP_PIN); 
			asm("nop");
			i=PIND;
			if ((1<<DG_PIN) & i)								// Проверяем крайний левый бит если он равен 1
			{
				data=data+1;									//то записываем в линию данных 1
			}
			//else
			//{
				//DS_PORT=(0<<DS_PIN);									// иначе записываем 0
			//}
			if (counter < 7)
			{
				data = (data << 1);	// Сдвигаем все биты data влево на один бит
			}
			
								
		}
		 SCEPL_HIGH();
		
		 return data;
	}
	void SR_Send_8b_STCP (unsigned char data)
	{
		//SCEPL_PORT=(0<<SCEPL_PIN);
		//#if (SHIFT_REGISTER_IS_USED==1)
		SR_Send_8b(data);

		//Дергаем ногой и защёлкиваем данные.
		
		//STCP_PORT = (1<<STCP_PIN);
		STCP_HIGH();
		//asm("nop");	//Возможно вместо этого комментария придёться вставить задержку…а может и нет..

		//	STCP_PORT&=~_BV(STCP_PIN);
		//	STCP_PORT = (0<<STCP_PIN);
		//	STCP_PORT = (0<<STCP_PIN);
		//STCP_PORT = (0<<STCP_PIN);
		STCP_LOW();
		//DS_PORT = (0<<DS_PIN);
		DS_LOW();
		//SCEPL_PORT=(1<<SCEPL_PIN);
		//printf("SR=1");
		//send_Uart(13);
		//#else
		//printf("SR=0");
		//send_Uart(13);
		//#endif
	}


	
	void SR_Send_str_STCP (unsigned char* data)
	{
		while (*data!= 0)
		{
			SR_Send_8b(*data++);
		}
		//Дергаем ногой и защёлкиваем данные.
		STCP_PORT = (1<<STCP_PIN);
		//asm("nop");
		//asm("nop");
		//asm("nop");	//Возможно вместо этого комментария придёца вставить задержку…а может и нет..
		STCP_PORT = (0<<STCP_PIN);
		DS_PORT = (0<<DS_PIN);
	}

	void SR_Init()
	{
		//#if (SHIFT_REGISTER_IS_USED==1)
		
		// Пины на выход
		DS_DDR=(1<<DS_PIN)|(1<<STCP_PIN)|(1<<SCEPL_PIN);
		
		SHCP_DDR=(1<<SHCP_PIN);
		
		
		// Пин 165-го на вход
		DG_DDR &=~ (1<<DG_PIN); //0
		
		// начальное условие
		SCEPL_PORT=(1<<SCEPL_PIN);
		DS_PORT=(0<<DS_PIN);
		SHCP_PORT=(0<<SHCP_PIN);
		STCP_PORT=(0<<STCP_PIN);
		
		
		
		//обнулим регистр
		SR_Send_8b_STCP (0x00);
		//#endif
	}
