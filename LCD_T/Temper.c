/*
 * Temper.c
 *
 * Created: 20.08.2014 10:39:00
 *  Author: Vadim2
 */ 
#include "Temper.h"
#include "settings.h"
#if LCD_TYPE==0
#include "lcd_lib_2.h"
#endif
#if LCD_TYPE==1
#include "n3310.h"
#endif


//#include "Message.h"
//#if _1WIRE_IS_USED==1
//массив структур для хранения адресов
OWI_device allDevices[MAX_DEVICES];
volatile char scratchpad[MAX_DEVICES][2];
volatile char num_device = 0;//количество найденных устройств

uint8_t Get_Temp_int()
{
	char scratchpad_int[MAX_DEVICES][2];
	scratchpad_int[0][0]=scratchpad[0][0];
	scratchpad_int[0][1]=scratchpad[0][1];
	uint8_t temperature;
	uint8_t znak_temper=0;// 0- минус 1-плюс
	unsigned int tmp = 0;
	/*выводим знак и преобразуем число, если оно отрицательное*/
	
	if ((scratchpad_int[0][1]&128) == 0) /*если 7-ой бит равен 0, значит число положительное*/
	{
		znak_temper=1;
		//нарисуем плюс "+" :
	}
	else
	{
		znak_temper=0;
		//нарисуем минус "-" :
		tmp = ((unsigned int)scratchpad_int[0][1]<<8)|scratchpad_int[0][0];
		tmp = ~tmp + 1;
		scratchpad_int[0][0] = tmp;
		scratchpad_int[0][1] = tmp>>8;
	}
	//LcdGotoXYFont(4,n+1);
	/*выводим значение целое знач. температуры*/
	temperature = (scratchpad_int[0][0]>>4)|((scratchpad_int[0][1]&7)<<4);
	if (znak_temper == 0) /*если -*/
	{
		temperature=128-temperature;
		//нарисуем плюс "+" :
	}
	else
	{
		temperature=128+temperature;
	}
	return temperature;
}

void Init_Temperatura()
{
	OWI_Init(BUS);
	//unsigned char num= 0;
	uint8_t num = 0;
	unsigned char i= 0;
	//num = 0;
	
	
	//Получение адресов 1-Wire устройств.
	//Посылаем команду SEARCH ROM, получаем адреса нескольких 1-Wire устройств,
	// сохраняет их и проверяет контрольную сумму каждого адреса.
	//printf("поиск 1-Wire");
	//send_Uart(13);
	OWI_SearchDevices(allDevices, MAX_DEVICES, BUS, &num);
	//сохраним сколько нашли устройств
	num_device = num; 
	//printf("поиск закончен 1-Wire");
	//send_Uart(13);
	//напишем сколько нашли устройств
	//unsigned char* id1;
	//printf("Найдено %u устройств",num);
	//send_Uart(13);
	unsigned char* id1;
	#if UART_IS_USED==1
	//unsigned char* id1;
	printf("Найдено %u устройств",num);
	send_Uart(13);
	//Для каждого из найденных устройств:
 	while(i<=num_device-1)
 	{
		//Отправим в UART данные найденных датчиков
		send_Uart_str("Датчик № ");
 		printf("%u ",i ); 
		send_Uart_str(" ROM CODE: ");
		id1=allDevices[i].id;
		printf("%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X", id1[7],id1[6],id1[5],id1[4],id1[3],id1[2],id1[1],id1[0]);
 		send_Uart(13);
		i++;
	}
	#endif
	#if LCD_IS_USED==1
		#if LCD_TYPE==1
		char str[14];
		//unsigned char* id1;
		//printf("LCD!");
		//send_Uart(13);
		LcdClear();
		LcdGotoXYFont(0,0);
		//LcdFStr(FONT_1X,(unsigned char*)PSTR("Найдено "));
		sprintf(str, "Найдено %u дт.", num);
		LcdStr(FONT_1X, &str);
		LcdUpdate();		
		//printf("Найдено %u устройств",num);
		//send_Uart(13);
		//Для каждого из найденных устройств:
		i=0;
		while(i<=num_device-1)
		{
			//Отправим в LCD данные найденных датчиков
			LcdGotoXYFont(0,i*2+1);
			sprintf(str, "ROM датчика %u:", i);
			LcdStr(FONT_1X, &str);
			LcdGotoXYFont(0,i*2+2);
			//LcdFStr(FONT_1X,(unsigned char*)PSTR("Найдено "));
			//send_Uart_str("Датчик № ");
			//printf("%u ",i );
			//send_Uart_str(" ROM CODE: ");
			id1=allDevices[i].id;
			sprintf(str, "%.2X%.2X%.2X%.2X%.2X%.2X%.2X", id1[6],id1[5],id1[4], id1[3],id1[2],id1[1],id1[0]);
			LcdStr(FONT_1X, &str);
			LcdUpdate();
			//send_Uart(13);
			i++;
			_delay_ms(1000);
		}
		#endif
		#if LCD_TYPE==0
			char str[20];
			LCD_Clear();
			LCD_Goto(0,0);
			sprintf(str, "Naydeno %u dt.", num);
			//LcdStr(FONT_1X, &str);
			//LcdUpdate();
			LCD_SendStr(&str);
			_delay_ms(1000);
			//printf("Найдено %u устройств",num);
			//send_Uart(13);
			//Для каждого из найденных устройств:
			i=0;
			while(i<=num_device-1)
			{
				//Отправим в LCD данные найденных датчиков
				//LcdGotoXYFont(0,i*2+1);
				LCD_Goto(0,1);
				sprintf(str, "ROM датчика %u:", i);
				LCD_SendStr(&str);
				_delay_ms(1000);
				LCD_Goto(0,2);
				//LcdGotoXYFont(0,i*2+2);//
				//LcdFStr(FONT_1X,(unsigned char*)PSTR("Найдено "));
				//send_Uart_str("Датчик № ");
				//printf("%u ",i );
				//send_Uart_str(" ROM CODE: ");
				id1=allDevices[i].id;
				sprintf(str, "%.2X%.2X%.2X%.2X%.2X%.2X%.2X", id1[6],id1[5],id1[4], id1[3],id1[2],id1[1],id1[0]);
				LCD_SendStr(&str);
				//LcdUpdate();//
				//send_Uart(13);
				i++;
				//_delay_ms(1000);
			}
		#endif
		
	#endif
	//подаем сигнал сброса
	OWI_DetectPresence(BUS);
	//команду для адресации всех устройств на шине
	OWI_SkipRom(BUS);
	//подаем команду - запук преобразования 
	OWI_SendByte(DS18B20_CONVERT_T ,BUS);
	//для будущих считываний
}

void Get_all_Temperatura()
{
	//unsigned int tmp = 0;
	//unsigned char temperature;
	unsigned char n=0;
// 
//      while (!OWI_ReadBit(BUS))
// 	{
// 	
// 	};

//Для каждого из найденных устройств:
	while (n<=num_device-1)
	{
		/*подаем сигнал сброса
		команду для адресации n-го устройства на шине
		команду - чтение внутренней памяти
		затем считываем внутреннюю память датчика в массив
		*/
		OWI_DetectPresence(BUS);
		OWI_MatchRom(allDevices[n].id, BUS);
		OWI_SendByte(DS18B20_READ_SCRATCHPAD, BUS);
		scratchpad[n][0] = OWI_ReceiveByte(BUS);
		//message_unit[0].GET_TEMP[0]=scratchpad[0];
		scratchpad[n][1] = OWI_ReceiveByte(BUS);
		//	message_unit[0].GET_TEMP[1]=scratchpad[1];
		n++;
	}
//подаем сигнал сброса
//команду для адресации всех устройств на шине
//подаем команду - запук преобразования *
OWI_DetectPresence(BUS);
OWI_SkipRom(BUS);
OWI_SendByte(DS18B20_CONVERT_T ,BUS);
}

void Set_Temp_to_LCD()
{
	#if LCD_IS_USED==1
		//KEY_V_BAT_ON();
		Get_all_Temperatura();//(10 ms)
		//KEY_V_BAT_OFF();
		unsigned int tmp = 0;
		uint8_t znak_temper=0;// 0- минус 1-плюс
		uint8_t temperature;
		uint8_t temperature_i;
		char scratchpad_lcd[MAX_DEVICES][2];
		
		unsigned char n=0;
		#if LCD_TYPE==1
			char str[14];
			//LcdClear();
			while(n<=num_device-1)
			{
				scratchpad_lcd[n][0]=scratchpad[n][0];
				scratchpad_lcd[n][1]=scratchpad[n][1];
					//LcdGotoXYFont(0,n+1);
					//sprintf(str, "t");
					//LcdStr(FONT_2X, &str);
					//LcdFStr(FONT_1X,(unsigned char*)PSTR("т-ра "));
				//LcdGotoXYFont(1,1);
				//LcdSingleBar(0,31,31,83,PIXEL_OFF);//очистим полоску "координаты нижнего левого угла"(Y,X,высота,ширина, стереть или закрасить) полоски
				/*выводим знак и преобразуем число, если оно отрицательное*/
				//LCD_Goto(5,0);
				if ((scratchpad_lcd[n][1]&128) == 0) /*если 7-ой бит равен 0, значит число положительное*/
				{
					znak_temper=1;
					//нарисуем плюс "+" :
					//LcdSingleBar(0,17,4,12,PIXEL_ON);//- горизонтальная линия
					//LcdSingleBar(4,21,12,4,PIXEL_ON);//| вертикальная линия
					//LcdFStr(FONT_2X,(unsigned char*)PSTR("+"));
					
					////send_Uart_str('+');
					////LCD_WriteData('+');
				}
				else
				{
					znak_temper=0;
					//LcdSingleBar(0,17,4,12,PIXEL_ON);//- горизонтальная линия (минус)
					//sprintf(str, "-");
					//LcdStr(FONT_2X, &str);
					
					////LcdFStr(FONT_BIG_I,(unsigned char*)PSTR("-"));
					////send_Uart_str("-");
					////LCD_WriteData('-');
					tmp = ((unsigned int)scratchpad_lcd[n][1]<<8)|scratchpad_lcd[n][0];
					tmp = ~tmp + 1;
					scratchpad_lcd[n][0] = tmp;
					scratchpad_lcd[n][1] = tmp>>8;
				}
				//LcdGotoXYFont(4,n+1);
				/*выводим значение целое знач. температуры*/
				temperature = (scratchpad_lcd[n][0]>>4)|((scratchpad_lcd[n][1]&7)<<4);
				//Извращаемся с выводом цыфр на экран :(
				if (temperature < 100)
				{ 
					//если температура двузначное число, рисуем ее крупно
					//KEY_V_BAT_ON();
					//LcdSingleBar(0,31,32,17,PIXEL_OFF);//очистим полоску "координаты нижнего левого угла"(Y,X,высота,ширина, стереть или закрасить) полоски
					//KEY_V_BAT_OFF();
					if (znak_temper==0)
					{
						//если минус
						LcdSingleBar(0,17,4,12,PIXEL_ON);//- горизонтальная линия (минус)
					} 
					else
					{
						//если плюс
						LcdSingleBar(0,17,4,12,PIXEL_ON);//- горизонтальная линия
						LcdSingleBar(4,21,12,4,PIXEL_ON);//| вертикальная линия
					}
					if (temperature<10)
					{
						LcdBarsFontNumber(temperature,1);
					}
					if (temperature>=10)
					{
						/*выводим десятки*/
						//KEY_V_BAT_ON();
						LcdBarsFontNumber(temperature/10,0);
						//KEY_V_BAT_OFF();
						temperature_i=temperature/10;
						/*выводим единицы*/
						LcdBarsFontNumber((temperature-(temperature_i*10)),1);
					}
					/*выводим дробную часть знач. температуры*/
					temperature = (scratchpad[n][0]&15);
					temperature = (temperature<<1) + (temperature<<3);
					temperature = (temperature>>4);
					
					LcdSingleBar(54,31,4,4,PIXEL_ON);
					LcdBarsFontNumber(temperature,2);
					
				}
				else
				{
					//если температура больше 99 градусов, рисуем ее мельче
					LcdGotoXYFont(0,1);
					if (znak_temper==0)
					{
						//если минус
						LcdFStr(FONT_2X,(unsigned char*)PSTR("-"));
					}
					else
					{
						//если плюс
						LcdFStr(FONT_2X,(unsigned char*)PSTR("+"));
					}
					
					sprintf(str, "%3u", temperature);
					LcdStr(FONT_2X, &str);
					
					/*выводим дробную часть знач. температуры*/
					temperature = (scratchpad[n][0]&15);
					temperature = (temperature<<1) + (temperature<<3);
					temperature = (temperature>>4);
					
					LcdGotoXYFont(8,1);
					sprintf(str, ".%u", temperature);
					LcdStr(FONT_2X, &str);
				}
				
				//LcdBarsFontNumber(0,1);

				//printf(" %u",  temperature);
				/*выводим дробную часть знач. температуры*/
				//temperature = (scratchpad[n][0]&15);
				//temperature = (temperature<<1) + (temperature<<3);
				//temperature = (temperature>>4);
				
				//printf(".%u",  temperature);
				//sprintf(str, ".%u", temperature);
				//LcdStr(FONT_2X, &str);
				
				//send_Uart(13);
				n++;
			}
			//LcdUpdate();
		#endif
		#if LCD_TYPE==0
			char str[14];
			//LcdClear();
			while(n<=num_device-1)
			{
				
				LCD_Goto(0,n+1);
				//LcdGotoXYFont(0,n+1);
				sprintf(str, "t");
				LCD_SendStr(&str);
				//LcdStr(FONT_2X, &str);
				//LcdFStr(FONT_1X,(unsigned char*)PSTR("т-ра "));
				//LcdGotoXYFont(4,n+1);
				LCD_Goto(4,n+1);
				/*выводим знак и преобразуем число, если оно отрицательное*/
				//LCD_Goto(5,0);
				if ((scratchpad[n][1]&128) == 0) /*если 7-ой бит равен 0, значит число положительное*/
				{
					//LCD_SendStr("+");
					//LcdFStr(FONT_2X,(unsigned char*)PSTR("+"));
					//send_Uart_str('+');
					LCD_WriteData('+');
				}
				else
				{
					//LcdFStr(FONT_2X,(unsigned char*)PSTR("-"));
					//send_Uart_str("-");
					LCD_WriteData('-');
					tmp = ((unsigned int)scratchpad[n][1]<<8)|scratchpad[n][0];
					tmp = ~tmp + 1;
					scratchpad[n][0] = tmp;
					scratchpad[n][1] = tmp>>8;
				}
				LCD_Goto(6,n+1);
				//LcdGotoXYFont(6,n+1);
				/*выводим значение целое знач. температуры*/
				temperature = (scratchpad[n][0]>>4)|((scratchpad[n][1]&7)<<4);
				//send_int_Uart(temperature);
				sprintf(str, "%2u", temperature);
				LCD_SendStr(&str);
				//LcdStr(FONT_2X, &str);
				//LcdGotoXYFont(10,n+1);
				LCD_Goto(10,n+1);
				//printf(" %u",  temperature);
				/*выводим дробную часть знач. температуры*/
				temperature = (scratchpad[n][0]&15);
				temperature = (temperature<<1) + (temperature<<3);
				temperature = (temperature>>4);
				//printf(".%u",  temperature);
				sprintf(str, ".%u", temperature);
				LCD_SendStr(&str);
				//LcdStr(FONT_2X, &str);
	
				//send_Uart(13);
				n++;
			}
			//LcdUpdate();
		#endif
	#endif

}

//void Set_Temp_to_UART()
//{
			//
			//#if UART_IS_USED==1
			//unsigned char n=0;
			//unsigned int tmp = 0;
			//unsigned char temperature;
			//send_Uart_str("считали инфу ");
			//send_int_Uart(n);
			//send_Uart_str("-го датчика");
			//send_Uart(13);
			//printf("scratchpad[0]: %X", scratchpad[n][0]);
			//send_Uart(13);
			//printf("scratchpad[1]: %X", scratchpad[n][1]);
			//send_Uart(13);
			//
			///*выводим знак и преобразуем число, если оно отрицательное*/
			////LCD_Goto(5,0);
			//if ((scratchpad[n][1]&128) == 0) /*если 7-ой бит равен 0, значит число положительное*/
			//{
				//send_Uart_str('+');
				////LCD_WriteData('+');
			//}
			//else
			//{
				//send_Uart_str("-");
				////LCD_WriteData('-');
				//tmp = ((unsigned int)scratchpad[n][1]<<8)|scratchpad[n][0];
				//tmp = ~tmp + 1;
				//scratchpad[n][0] = tmp;
				//scratchpad[n][1] = tmp>>8;
			//}
			///*выводим значение целое знач. температуры*/
			//temperature = (scratchpad[n][0]>>4)|((scratchpad[n][1]&7)<<4);
			////send_int_Uart(temperature);
			//printf(" %u",  temperature);
			///*выводим дробную часть знач. температуры*/
			//temperature = (scratchpad[n][0]&15);
			//temperature = (temperature<<1) + (temperature<<3);
			//temperature = (temperature>>4);
			//printf(".%u",  temperature);
			//send_Uart(13);
			//#endif
			//#if LCD_IS_USED==1
			//
			//send_Uart_str("считали инфу ");
			//send_int_Uart(n);
			//send_Uart_str("-го датчика");
			//send_Uart(13);
			//printf("scratchpad[0]: %X", scratchpad[n][0]);
			//send_Uart(13);
			//printf("scratchpad[1]: %X", scratchpad[n][1]);
			//send_Uart(13);
			//
			///*выводим знак и преобразуем число, если оно отрицательное*/
			////LCD_Goto(5,0);
			//if ((scratchpad[n][1]&128) == 0) /*если 7-ой бит равен 0, значит число положительное*/
			//{
				//send_Uart_str('+');
				////LCD_WriteData('+');
			//}
			//else
			//{
				//send_Uart_str("-");
				////LCD_WriteData('-');
				//tmp = ((unsigned int)scratchpad[n][1]<<8)|scratchpad[n][0];
				//tmp = ~tmp + 1;
				//scratchpad[n][0] = tmp;
				//scratchpad[n][1] = tmp>>8;
			//}
			///*выводим значение целое знач. температуры*/
			//temperature = (scratchpad[n][0]>>4)|((scratchpad[n][1]&7)<<4);
			////send_int_Uart(temperature);
			//printf(" %u",  temperature);
			///*выводим дробную часть знач. температуры*/
			//temperature = (scratchpad[n][0]&15);
			//temperature = (temperature<<1) + (temperature<<3);
			//temperature = (temperature>>4);
			//printf(".%u",  temperature);
			//send_Uart(13);
			//#endif
			//n++;
//}

void Get_temp(unsigned char *temp[])
{
	//если есть одно устройство на шине
if (num_device==1)
	{
	//подаем сигнал сброса
    OWI_DetectPresence(BUS);
    //OWI_MatchRom(allDevices[0].id, BUS);
	//пропуск адреса
	OWI_SkipRom(BUS);
	//команду - чтение внутренней памяти
    OWI_SendByte(DS18B20_READ_SCRATCHPAD, BUS);
    //scratchpad[0] = OWI_ReceiveByte(BUS);
	//message_unit.GET_TEMP[0]= OWI_ReceiveByte(BUS);
	*temp[0]=OWI_ReceiveByte(BUS);
    //scratchpad[1] = OWI_ReceiveByte(BUS);
	//message_unit.GET_TEMP[1]=OWI_ReceiveByte(BUS);
	*temp[1]=OWI_ReceiveByte(BUS);
	// 		 /*подаем сигнал сброса
	 //     команду для адресации всех устройств на шине
	 //     подаем команду - запук преобразования */
	 OWI_DetectPresence(BUS);
	 OWI_SkipRom(BUS);
	 OWI_SendByte(DS18B20_CONVERT_T ,BUS);
	 }
}

//#endif