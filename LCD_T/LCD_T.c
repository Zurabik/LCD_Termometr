/*
 * LCD_T.c
 *
 * Created: 22.07.2014 21:52:50
 *  Author: Vadim2
 */
//тут у нас настройки для подключаемых модулей
#include "settings.h" 
#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t start_number; //сетчик запусков для температуры

uint8_t adc_start_number; //сетчик запусков для АЦП



//Настройки и инициализиция:
#if 1==1
/*=====================SLEEP_MODE===================================*/
#if SLEEP_MODE_USED==1
#include <avr/io.h>

#include <avr/sleep.h> // здесь описаны режимы сна
#if SLEEP_MODE_WD==1
#include <avr/wdt.h> // здесь организована работа с ватчдогом
ISR (WDT_vect)
{
	//_delay_us(100);
	
	//if (start_number<_1WIRE_START_CYCLES_NUMBER)
	//{
		//#if LCD_IS_USED==1 
		//LcdClear();
		//PORTB ^= _BV(PB4); // переключаем светодиод
		//#if ADC_IS_USED==1
		//ADC_result();
		//#endif
		//Set_Temp_to_LCD();
		
		//#endif
	//	start_number++;
	//} 
	//else
	//{
	//	start_number=0;
		//#if LCD_IS_USED==1
		//LcdClear();
		//PORTB ^= _BV(PB4); // переключаем светодиод
		//#if ADC_IS_USED==1
		//ADC_result();
		//#endif
		//Set_Temp_to_LCD();
		//#endif
	//}
	
	//LcdUpdate();
	WDTCSR |= _BV(WDIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
}
#endif
#endif
/*=====================SLEEP_MODE===================================*/
/*=====================UART================================*/
#if (UART_IS_USED==1)
#include "UART.h"
#endif
/*=====================UART================================*/

/*=====================LCD================================*/
#if (LCD_IS_USED==1)//Если LCD дисплей используется

	#if LCD_TYPE==0 //Если LCD символьный
	#include "compilers_4.h"
	#include "lcd_lib_2.h"
//#include "bcd.h"
//здесь определены строки, чтобы
//знать в каком режиме работет LCD
//какой используется порт и компилятор
//строки определны в ОЗУ для переносимости проекта

	#if (LCD_BUS_4_8_BIT == 0)
		char textBus[] = "4 bit ";
	#else
		char textBus[] = "8 bit ";
	#endif

	#if (LCD_CHECK_FL_BF == 0)
		char textBF[] = "delay ";
	#else
		char textBF[] = "bf_flag ";
	#endif

	#ifdef VIRT_PORT
		char textTypePort[] = "virtual port ";
	#else
		char textTypePort[] = "real port ";
	#endif

	#ifdef __ICCAVR__
		char textCompiler[] = "IAR ";
	#elif __GNUC__
		char textCompiler[] = "GNUC ";
	#elif __CODEVISIONAVR__
		char textCompiler[] = "CV ";
	#else
		char textCompiler[] = "xz ";
	#endif

	const uint8_t quarterNote[] PROGMEM   = {4,4,4,4,4,4,28,28};
	const uint8_t eighthNote[]  PROGMEM   = {4,6,5,4,4,4,28,28};
	const uint8_t sixteenthNote[] PROGMEM = {4,6,5,6,5,4,28,28};
	#endif
	#if LCD_TYPE==1 //Если LCD графический
	#include "n3310.h"
	#include "picture.h"
	#endif
#endif
/*=====================LCD================================*/

/*=====================NRF24L01+================================*/
#if (NRF24L01_IS_USED==1)
#include "Message.h"
#include "TX_RX_command.h"
#include "packet.h"
#include "radio.h"
uint8_t i;						//счетчик устройств
uint8_t tx_addr[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };		// Адрес приемника Receiver address
uint8_t rx_addr[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };		// Адрес приемника Receiver address
volatile char tx_delay = 2;
#endif
/*=====================NRF24L01+================================*/

/*=====================1-Wire===================================*/
#if _1WIRE_IS_USED==1

	#include "Temper.h"

#endif
/*=====================1-Wire===================================*/

/*=====================Timer===================================*/
#if (TIMER0_IS_USED==1)
#include "Timer.h"
#endif
/*=====================Timer===================================*/

/*=====================ADC===================================*/
#if (ADC_IS_USED==1)
#include "adc.h"
#endif
/*=====================ADC===================================*/

/*=====================74HC595===================================*/
#if (SHIFT_REGISTER_IS_USED==1)
#include "shift_register.h"
#endif
/*=====================74HC595===================================*/

/*=====================IR-reciver===================================*/
#if (IR_IS_USED==1)
#include "nec.h"
#endif
/*=====================IR-reciver===================================*/

/*===================== DS3231 ===================================*/
#if DS3231_IS_USED==1
#include "DS3231.h"
//#define _1WIRE_START_CYCLES_NUMBER 75	//Количество запусков МК без проверки температуры8*75=600 сек (10 мин.)
#endif
/*===================== DS3231 ===================================*/

/*ISR (USART_RX_vect)
{
	cli(); //запрещаем прерывания
	//send_Uart_str("you put! ");
	char c=UDR0;
	switch (c)
	{
		case 0x00:
		send_Uart(13);
		send_Uart_str("you put 0x00!");
		send_Uart(13);
		break;
		
		case 0x01:
		send_Uart(13);
		send_Uart_str("you put 0x01!");
		send_Uart(13);
		break;
		
		// 		case 0x61:
		// 		LOAD_1!=LOAD_1;
		// 		break;
		
		
		case 0x41:// "A"
		tx_delay=tx_delay-1;
		printf("tx_delay : %u", tx_delay);
		send_Uart(13);
		//Load_get();
		//Load_set(!load[0],1,1);
		//printf("load[0] %X", load[0]);
		//send_Uart(13);
		//printf("load[1] %X", load[1]);
		//send_Uart(13);
		//printf("load[2] %X", load[2]);
		//send_Uart(13);
		//LOAD_1!=LOAD_1;
		break;
		
		case 0x44:// "D"
		tx_delay=tx_delay+1;
		printf("tx_delay : %u", tx_delay);
		send_Uart(13);
		//IrPulseThershold--;// 1024/8000 * 9 = 1.152 msec
		//printf("IrPulseThershold : %u", IrPulseThershold);
		//send_Uart(13);
		break;
		
		case 0x57:// "W"
		tx_delay=tx_delay+10;
		printf("tx_delay : %u", tx_delay);
		send_Uart(13);
		// определяет таймаут приема посылки
		// и ограничивает максимальную длину импульса и паузы
		//TimerReloadValue++;
		//printf("TimerReloadValue : %u", TimerReloadValue);
		//send_Uart(13);
		break;
		
		case 0x53:// "S"
		tx_delay=tx_delay-10;
		printf("tx_delay : %u", tx_delay);
		send_Uart(13);
		// определяет таймаут приема посылки
		// и ограничивает максимальную длину импульса и паузы
		//TimerReloadValue--;
		//printf("TimerReloadValue : %u", TimerReloadValue);
		//send_Uart(13);
		break;
		
		case 0x75:
		send_Uart(13);
		send_Uart_str("you put 0x75!");
		send_Uart(13);
		break;
		case 0xE5:
		send_Uart(13);
		send_Uart_str("Температура ");
		send_Uart(13);
		//Get_Temperatura();
		send_Uart(13);
		send_Uart_str("Температура готова");
		break;
		default:
		send_Uart(13);
		printf("you put:  %X", c);
		//send_Uart_str("you put: ");
		//send_Uart(c);
		// 		send_Uart_str(" но увы!");
		send_Uart(13);
		//LCD_SendStr("Hi");
		break;
	}
	// 	send_Uart(13);
	// 	send_Uart_str("разрешаем прерывание");
	sei(); //разрешаем прерывания
	
	
	//текст программы обработки прерывания при получении данных
	//flag_RX=1;
	// 	if (flag_RX==0x00)
	// 	{
	// 		USART_RX_data[0]=0;
	// 		USART_RX_data[1]=0;
	// 		num_b=0;
	// 	}
	// 	else
	// 	{
	// 		USART_RX_data[num_b]=flag_RX;
	// 		num_b++;
	// 	}
	//USART_RX_data[num_b]=UDR0;
	//		num_b++;
	//	sei();
}
*/



#endif



int main(void)
{
	//Передатчик (ведущий)
	#if MODE_OF_OPERATION==0
	unsigned char tx_command[6];
	_delay_ms(1);
	//stdout = &usart_str;
	//set_message_default(1);
	/*====инициализируем UART=====*/
	//volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	//stdout=stdin=&uart_stream;		// указываем, куда будет выводить printf
	#if (UART_IS_USED==1)
	volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	stdout=stdin=&uart_stream;		// указываем, куда будет выводить printf
	//stdin =&uart_stream;		// указываем, куда будет выводить printf
	USART_Init(MYUBRR);
	#endif
	sei();
	
	#if (ADC_IS_USED==1)
	ADC_init();
	#endif
	//Shift register:
	#if (SHIFT_REGISTER_IS_USED==1)
	SR_Init();
	SR_Send_8b_STCP("2");
	#else
	#if (UART_IS_USED==1)
	printf("SR=0");
	send_Uart(13);
	#endif
	#endif
	//Используем LCD дисплей
	#if (LCD_IS_USED==1)
	//инициализируем дисплей
	// LCD_Init();
	// LCD_Goto(0,0);
	// LCD_SendStr("Load...");
	// загружаем новые символы
	LCD_SetUserChar(quarterNote, 0);
	LCD_SetUserChar(eighthNote,  1);
	LCD_SetUserChar(sixteenthNote, 2);
	
	//позиционируем курсор и выводим строки
	LCD_Goto(0,0);
	LCD_SendStr(textCompiler);
	LCD_SendStr(textTypePort);
	
	LCD_Goto(0,1);
	LCD_SendStr(textBus);
	LCD_SendStr(textBF);
	
	//выводим новый символ
	LCD_WriteData(2);
	_delay_ms(1000);
	LCD_Clear();
	LCD_Goto(0,0);
	LCD_SendStr("Ok");
	#endif
	/*===========================*/
	//если используется Timer0
	#if (TIMER0_IS_USED==1)
	Init_Timer0();
	#endif

	#if (IR_IS_USED==1)
	IR_Init();
	#endif
	
	#if (NRF24L01_IS_USED==1)			//если используется трансивер
	#if (NRF24L01_TX_RX==0)		//если используется режим передатчика
	set_all_message_default(MAX_SENSOR_UNIT);
	setup_NRF24L01_Transmitter(tx_addr);	//установим в режим передатчика
	#endif
	#if (NRF24L01_TX_RX==1)
	//set_all_message_default(1);
	set_message_default();
	rx_addr[4]=message_unit[0].SENSOR_ADDRESS;
	setup_NRF24L01_Receiver(rx_addr);		//установим в режим приемника
	#endif
	#endif
	
	#if (_1WIRE_IS_USED==1)				//если используется 1-Wire
	//		unsigned int tmp = 0;
	//		unsigned char temperature;
	//инициализируем 1-Wire
	OWI_Init(BUS);
	Init_Temperatura();
	//puts( "Init_Temperatura\r\n" );
	//_delay_ms(1000);
	// Get_Temperatura();
	
	#endif
	#endif
	
	//Приемник (ведомый)
	#if MODE_OF_OPERATION==1
	unsigned char tx_command[6];
	_delay_ms(1);
	//stdout = &usart_str;
	//set_message_default(1);
	/*====инициализируем UART=====*/
	//volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	//stdout=stdin=&uart_stream;		// указываем, куда будет выводить printf
	#if (UART_IS_USED==1)
	volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	stdout=stdin=&uart_stream;		// указываем, куда будет выводить printf
	//stdin =&uart_stream;		// указываем, куда будет выводить printf
	USART_Init(MYUBRR);
	#endif
	sei();
	
	#if (ADC_IS_USED==1)
	ADC_init();
	#endif
	//Shift register:
	#if (SHIFT_REGISTER_IS_USED==1)
	SR_Init();
	SR_Send_8b_STCP("2");
	#else
	#if (UART_IS_USED==1)
	printf("SR=0");
	send_Uart(13);
	#endif
	#endif
	//Используем LCD дисплей
	#if (LCD_IS_USED==1)
	//инициализируем дисплей
	// LCD_Init();
	// LCD_Goto(0,0);
	// LCD_SendStr("Load...");
	// загружаем новые символы
	LCD_SetUserChar(quarterNote, 0);
	LCD_SetUserChar(eighthNote,  1);
	LCD_SetUserChar(sixteenthNote, 2);
	
	//позиционируем курсор и выводим строки
	LCD_Goto(0,0);
	LCD_SendStr(textCompiler);
	LCD_SendStr(textTypePort);
	
	LCD_Goto(0,1);
	LCD_SendStr(textBus);
	LCD_SendStr(textBF);
	
	//выводим новый символ
	LCD_WriteData(2);
	_delay_ms(1000);
	LCD_Clear();
	LCD_Goto(0,0);
	LCD_SendStr("Ok");
	#endif
	/*===========================*/
	//если используется Timer0
	#if (TIMER0_IS_USED==1)
	Init_Timer0();
	#endif

	#if (IR_IS_USED==1)
	IR_Init();
	#endif
	
	#if (NRF24L01_IS_USED==1)			//если используется трансивер
	#if (NRF24L01_TX_RX==0)		//если используется режим передатчика
	set_all_message_default(MAX_SENSOR_UNIT);
	setup_NRF24L01_Transmitter(tx_addr);	//установим в режим передатчика
	#endif
	#if (NRF24L01_TX_RX==1)
	//set_all_message_default(1);
	set_message_default();
	rx_addr[4]=message_unit[0].SENSOR_ADDRESS;
	setup_NRF24L01_Receiver(rx_addr);		//установим в режим приемника
	#endif
	#endif
	
	#if (_1WIRE_IS_USED==1)				//если используется 1-Wire
	//		unsigned int tmp = 0;
	//		unsigned char temperature;
	//инициализируем 1-Wire
	OWI_Init(BUS);
	Init_Temperatura();
	//puts( "Init_Temperatura\r\n" );
	//_delay_ms(1000);
	// Get_Temperatura();
	
	#endif
	#endif
	
	//Уличный датчик №1
	#if MODE_OF_OPERATION==2
	//перенеси потом на место!!
	uint8_t lcd_temp_correct_number_on=0;
	uint8_t lcd_temp_correct_number_off=0;
	// Пины на выход
	KEY_V_BAT_DDR=(1<<KEY_V_BAT_PIN);
	KEY_V_BAT_OFF();
	//CLKPR = (1 << CLKPCE); // Enable change of CLKPS bits
	//CLKPR = (1 << CLKPS1); // Set prescaler to 4, and system clock to 2 MHz
	MCUCR = (1 << BODS); 
	uint16_t _1wire_stop_i=0;
	//unsigned char tx_command[6];
	//_delay_ms(1);
	/*====инициализируем UART=====*/
	#if (UART_IS_USED==1)
	volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	stdout=stdin=&uart_stream;		// указываем, куда будет выводить printf
	//stdin =&uart_stream;		// указываем, куда будет выводить printf
	USART_Init(MYUBRR);
	printf("Загрузка");
	send_Uart(13);
	#endif
	//sei();
	
	#if SLEEP_MODE_USED==1
		#if SLEEP_MODE_WD==1
		//инициализация ватчдога
		wdt_reset(); // сбрасываем
		wdt_enable(WDTO_8S); // разрешаем ватчдог 8 сек
		WDTCSR |= _BV(WDIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
		sei(); // разрешаем прерывания
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную
		#endif
	#endif
	
	
	#if (ADC_IS_USED==1)
	ADC_init();
	#endif
	//Shift register:
	#if (SHIFT_REGISTER_IS_USED==1)
	SR_Init();
	SR_Send_8b_STCP("2");
	#if (UART_IS_USED==1)
	printf("SR=0");
	send_Uart(13);
	#endif
	#endif
	//Используем LCD дисплей
	#if (LCD_IS_USED==1)
		#if LCD_TYPE==0
		 //инициализируем дисплей
		 LCD_Init();
		 LCD_Goto(0,0);
		 LCD_SendStr("Load...");
		 //_delay_ms(1000);
		// загружаем новые символы
	 	 LCD_SetUserChar(quarterNote, 0);
	 	 LCD_SetUserChar(eighthNote,  1);
	 	 LCD_SetUserChar(sixteenthNote, 2);
	 
	 	 //позиционируем курсор и выводим строки
	 	 LCD_Goto(0,0);
	 	 LCD_SendStr(textCompiler);
	 	 LCD_SendStr(textTypePort);
	 	 
	 	 LCD_Goto(0,1);
	 	 LCD_SendStr(textBus);
	 	 LCD_SendStr(textBF);
	 
	 	 //выводим новый символ
	 	 LCD_WriteData(2);
		_delay_ms(1000);
	 	 LCD_Clear();
	 	 LCD_Goto(0,0);
	 	 LCD_SendStr("Ok");
		#endif 
		#if LCD_TYPE==1
		unsigned char lcd_power=1;
			LcdInit();
			LcdClear();
			LcdGotoXYFont(0,2);
			LcdFStr(FONT_2X,(unsigned char*)PSTR("Прывет!"));
			LcdUpdate();
		#endif
	#endif
	/*===========================*/
	//если используется Timer0
	#if (TIMER0_IS_USED==1)				
		Init_Timer0();
	#endif

	#if (IR_IS_USED==1)
		IR_Init();
	#endif
	
	#if (NRF24L01_IS_USED==1)			//если используется трансивер
		 #if (NRF24L01_TX_RX==0)		//если используется режим передатчика
			 set_all_message_default(MAX_SENSOR_UNIT);
			 setup_NRF24L01_Transmitter(tx_addr);	//установим в режим передатчика 
		 #endif
		 #if (NRF24L01_TX_RX==1)
			 //set_all_message_default(1);
			 set_message_default();
			  rx_addr[4]=message_unit[0].SENSOR_ADDRESS;
			 setup_NRF24L01_Receiver(rx_addr);		//установим в режим приемника
		 #endif
	#endif
	
	#if (_1WIRE_IS_USED==1)				//если используется 1-Wire
//		unsigned int tmp = 0;
//		unsigned char temperature;
     //инициализируем 1-Wire
	 //printf("инициализируем 1-Wire");
	 //send_Uart(13);
	 OWI_Init(BUS);
     Init_Temperatura();
	 //puts( "Init_Temperatura\r\n" );
	 //_delay_ms(1000);
	// Get_Temperatura();
	
	#endif 
	#endif

	//Уличный датчик №2
	#if MODE_OF_OPERATION==4
	
	// Пины на выход
	#if (KEY_V_BAT_USED==1)
		KEY_V_BAT_DDR |=  _BV(KEY_V_BAT_PIN);
		//KEY_V_BAT_DDR=(1<<KEY_V_BAT_PIN);
		KEY_V_BAT_OFF();
	#else
	#endif
	//KEY_V_BAT_DDR |=  _BV(KEY_V_BAT_PIN);
	//KEY_V_BAT_DDR=(1<<KEY_V_BAT_PIN);
	//KEY_V_BAT_OFF();
	KEY_LIGHT_DDR=(1<<KEY_LIGHT_PIN);
	KEY_LIGHT_OFF(); //отключаем подсветку
	//CLKPR = (1 << CLKPCE); // Enable change of CLKPS bits
	//CLKPR = (1 << CLKPS1); // Set prescaler to 4, and system clock to 2 MHz
	MCUCR = (1 << BODS);
	uint16_t _1wire_stop_i=0;
	//unsigned char tx_command[6];
	//_delay_ms(1);
	/*====инициализируем UART=====*/
	#if (UART_IS_USED==1)
	volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	stdout=stdin=&uart_stream;		// указываем, куда будет выводить printf
	//stdin =&uart_stream;		// указываем, куда будет выводить printf
	USART_Init(MYUBRR);
	printf("Загрузка");
	send_Uart(13);
	#endif
	//sei();
	#if SLEEP_MODE_USED==1
		#if SLEEP_MODE_WD==1
			//инициализация ватчдога
			wdt_reset(); // сбрасываем
			wdt_enable(WDTO_8S); // разрешаем ватчдог 8 сек
			WDTCSR |= _BV(WDIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
			sei(); // разрешаем прерывания
			set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную
		#else
		sei(); // разрешаем прерывания
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную
		#endif
	#endif
	#if (ADC_IS_USED==1)
	ADC_init();
	#endif
	/*Shift register:*/
	#if (SHIFT_REGISTER_IS_USED==1)
	SR_Init();
	SR_Send_8b_STCP("2");
	#if (UART_IS_USED==1)
	printf("SR=0");
	send_Uart(13);
	#endif
	#endif
	//Используем LCD дисплей
	#if (LCD_IS_USED==1)
	#if LCD_TYPE==0
	//инициализируем дисплей
	LCD_Init();
	LCD_Goto(0,0);
	LCD_SendStr("Load...");
	//_delay_ms(1000);
	// загружаем новые символы
	LCD_SetUserChar(quarterNote, 0);
	LCD_SetUserChar(eighthNote,  1);
	LCD_SetUserChar(sixteenthNote, 2);
	
	//позиционируем курсор и выводим строки
	LCD_Goto(0,0);
	LCD_SendStr(textCompiler);
	LCD_SendStr(textTypePort);
	
	LCD_Goto(0,1);
	LCD_SendStr(textBus);
	LCD_SendStr(textBF);
	
	//выводим новый символ
	LCD_WriteData(2);
	_delay_ms(100);
	LCD_Clear();
	LCD_Goto(0,0);
	LCD_SendStr("Ok");
	#endif
	#if LCD_TYPE==1
	unsigned char lcd_power=1;
	LcdInit();
	LcdClear();
	LcdGotoXYFont(0,2);
	LcdFStr(FONT_2X,(unsigned char*)PSTR("Прывет!"));
	LcdUpdate();
	_delay_ms(2000);
	#endif
	#endif
	/*===========================*/
	//если используется Timer0
	#if (TIMER0_IS_USED==1)
	Init_Timer0();
	#endif

	#if (IR_IS_USED==1)
	IR_Init();
	#endif
	
	#if (NRF24L01_IS_USED==1)			//если используется трансивер
	#if (NRF24L01_TX_RX==0)		//если используется режим передатчика
	set_all_message_default(MAX_SENSOR_UNIT);
	setup_NRF24L01_Transmitter(tx_addr);	//установим в режим передатчика
	#endif
	#if (NRF24L01_TX_RX==1)
	//set_all_message_default(1);
	set_message_default();
	rx_addr[4]=message_unit[0].SENSOR_ADDRESS;
	setup_NRF24L01_Receiver(rx_addr);		//установим в режим приемника
	#endif
	#endif
	
	#if _1WIRE_IS_USED==1				//если используется 1-Wire
	uint8_t start_1wire=0; //сетчик запусков для температуры
	//		unsigned int tmp = 0;
	//		unsigned char temperature;
	//инициализируем 1-Wire
	//printf("инициализируем 1-Wire");
	//send_Uart(13);
	OWI_Init(BUS);
	Init_Temperatura();
	//puts( "Init_Temperatura\r\n" );
	//_delay_ms(1000);
	// Get_Temperatura();
	
	#endif
	
	#if (DS3231_IS_USED==1)
		// Пины на вход
		uint16_t twi_max_delay=0;
		DS3231_INT_DDR &= ~_BV(DS3231_INT_PIN);
		//PORTD |= 1<<DS3231_INT_PIN;
		//DS3231_INT_DDR=(0<<DS3231_INT_PIN);
		//разрешаем прерывание (int0 по умолчанию, при низком уровне на PD2)
		EIMSK |= _BV(INT0);
		TWI_MasterInit(TWI_FREQ);
		sei();
		//DS3231_Set_One_Register(128,DS_REG_STATUS);
		//_delay_ms(1);
		
		//KEY_V_BAT_ON();
		DS3231_Set_Time(&ds3231,0,30,19);
		_delay_ms(1);
		//DS3231_Set_One_Register(128,DS_REG_A1SEC);//установим будильник на каждую секунду
		//DS3231_Set_One_Register(128,DS_REG_A1MIN);
		//DS3231_Set_One_Register(128,DS_REG_A1HOUR);
		//DS3231_Set_One_Register(128,DS_REG_A1DYDT);
		//DS3231_Set_One_Register(31,DS_REG_CONTROL);
		//
		//DS3231_Set_One_Register(128,DS_REG_STATUS);
		//KEY_V_BAT_OFF();
		DS3231_Set_Alarm1(&ds3231,DS3231_ALARM1_DELAY_S);
		_delay_ms(1);
		
		//_delay_ms(1200);
	#endif
	
	#endif


    while(1)
    {
		#if MODE_OF_OPERATION==0//Передатчик (ведущий)
		#if (NRF24L01_IS_USED==1)		//если используется трансивер
		
		#if (NRF24L01_TX_RX==0)		//в режиме передатчика
		
		tx_command[0]=SETUP_ALL_SETTING;
		tx_command[1]++;
		tx_command[2]=1;
		tx_command[3]=1;
		tx_command[4]=1;
		tx_command[5]=1;
		tx_command[6]=1;
		//printf("режим передатчика. \n\r");
		for (uint8_t n=0; n < MAX_SENSOR_UNIT; n++)
		{
			printf(n);
			tx_addr[4]=message_unit[n].SENSOR_ADDRESS;

			//SR_Send_8b_STCP(i_shift);
			
			Get_Transmit_NRF24L01(tx_command, tx_addr);	//отправляем сообщение
			//tx_Delay(tx_delay);			//пауза в мс
			//i++;
			_delay_us(500);
		}

		_delay_ms(100);
		
		#endif
		
		#if (NRF24L01_TX_RX==1)		//в режиме приемника
		Check_Receive_NRF24L01();		//проверяем пришло ли чтонибудь
		#endif
		
		#endif
		
		#if (TIMER0_IS_USED==1)			//если используется Timer0
		Check_Timer0();
		#endif
		#endif
		
		#if MODE_OF_OPERATION==1//Приемник (ведомый)
		#if (NRF24L01_IS_USED==1)		//если используется трансивер
		
		#if (NRF24L01_TX_RX==0)		//в режиме передатчика
		
		tx_command[0]=SETUP_ALL_SETTING;
		tx_command[1]++;
		tx_command[2]=1;
		tx_command[3]=1;
		tx_command[4]=1;
		tx_command[5]=1;
		tx_command[6]=1;
		//printf("режим передатчика. \n\r");
		for (uint8_t n=0; n < MAX_SENSOR_UNIT; n++)
		{
			printf(n);
			tx_addr[4]=message_unit[n].SENSOR_ADDRESS;

			//SR_Send_8b_STCP(i_shift);
			
			Get_Transmit_NRF24L01(tx_command, tx_addr);	//отправляем сообщение
			//tx_Delay(tx_delay);			//пауза в мс
			//i++;
			_delay_us(500);
		}

		_delay_ms(100);
		
		#endif
		
		#if (NRF24L01_TX_RX==1)		//в режиме приемника
		Check_Receive_NRF24L01();		//проверяем пришло ли чтонибудь
		#endif
		
		#endif
		
		#if (TIMER0_IS_USED==1)			//если используется Timer0
		Check_Timer0();
		#endif
		#endif
		
		#if MODE_OF_OPERATION==2//Уличный датчик №1
		//Get_all_Temperatura();
		//Set_Temp_to_LCD();
		//ADC_result();
		//_delay_ms(2000);
		//KEY_ON();
		if (load_number_i<1000000000)
		{
			load_number_i=load_number_i+1;//увеличим счетчик запусков
		}
		else
		{
			load_number_i=0;
		}
		//adc_i++;
		//_delay_ms(10);
		
		
		if (start_number>=_1WIRE_START_CYCLES_NUMBER)// редко проверяем температуру, раз в 10 минут
		{
			start_number=0; // обнулим сетчик запусков для температуры
			#if LCD_IS_USED==1
				if (ADC_BAT_RESULT_mV>V_1WIRE_MIN)//если напряжение позволит получить температуру с датчика 
				{
					_1wire_stop_i=0;//обнулим счетчик неудачных попыток измерить температуру
					#if LCD_TYPE==1
						LcdClear();//очищаем буфер экрана в памяти МК
					#endif
					#if LCD_TYPE==0
						LCD_Clear();//очищаем экран
					#endif
					#if ADC_IS_USED==1
						ADC_V_batt();//запускаем АЦП
					#endif
					#if _1WIRE_IS_USED==1
						Set_Temp_to_LCD();//получим температуру с датчика и сформируем данные для дисплея
					#endif
				}
				else //если напряжение не позволяет получить температуру с датчика
				{
					_1wire_stop_i++;//увеличим счетчик неудачных попыток измерить температуру
					#if LCD_TYPE==1
						sprintf(str2, "T fail: %6u", _1wire_stop_i);//укажем количество неудачных попыток измерить температуру
						LcdGotoXYFont(0,2);
						LcdStr(FONT_1X, &str2);
					#endif
				}
				#if LCD_TYPE==1
					if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN)//если напряжение позволит отобразить данные на дисплее, отправим их на него
					{
						/*выводим на экран напряжение*/
						ADC_V_bat_to_LCD();
						LcdUpdate();
					}
					#endif
			#endif
		}
		else // часто пытаемся проверить напряжение
		{
			start_number++;
			//start_number=0;
			if (adc_start_number<ADC_START_CYCLES_NUMBER)// пока вообще ничего не делаем, только счетчик увеличим и затем уснем
			{
				adc_start_number++;
			} 
			else//вот теперь проверяем напряжение 
			{
				adc_start_number=0;
				#if LCD_IS_USED==1
					
						#if ADC_IS_USED==1
						ADC_V_batt();//запускаем АЦП 
						/*выводим на экран напряжение*/
						ADC_V_bat_to_LCD();
						#endif
						#if LCD_TYPE==1
						if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN)
							{
								
								LcdUpdate();//если напряжение позволит отобразить данные на дисплее, отправим их на него
							}
						#endif
				#endif
			}
		}
		#if LCD_IS_USED==1

		#if LCD_TYPE==1


		// поочереди включаем и выключем экран для экономии энергии каждые 8 с,
		// если напряжение ниже 5 вольт
		
		if ((lcd_power==1) && (ADC_BAT_RESULT_mV<V_BAT_ECONOM))//если экран включен а напряжение на емкости требует экономии
		{
			if ((Get_Temp_int())<=108)// если очень холодно -20
			{
				if (lcd_temp_correct_number_off<LCD_TEMPERATURE_CORRECTION_NUMBER)// если выключать экран еще рано
				{
					lcd_temp_correct_number_off++;//подождем еще
				}
				else// если выключать экран уже пора
				{
					lcd_temp_correct_number_off=0;//сбросим счетчик
					Lcd_off();// выключем экран для экономии энергии
					KEY_V_BAT_OFF();// выключем подсветку для экономии энергии
					lcd_power=0;//экран был погашен
				}
			}
			else //если не очень холодно
			{
				lcd_temp_correct_number_off=0;//сбросим счетчик
				Lcd_off();// выключем экран для экономии энергии
				KEY_V_BAT_OFF();// выключем подсветку для экономии энергии
				lcd_power=0;//экран был погашен
			}
			
		}
		else// если экономить ненужно
		{
			
			if (Get_Temp_int()<=108)// если очень холодно -20
			{
				if (lcd_temp_correct_number_on<LCD_TEMPERATURE_CORRECTION_NUMBER)// если включать экран еще рано
				{
					lcd_temp_correct_number_on++;//подождем еще
				}
				else// если включать экран уже пора
				{
					lcd_temp_correct_number_on=0;//сбросим счетчик
					
					if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN) // включаем экран если напряжение позволит чтонить на нем увидеть
					{
						if (ADC_LIGHT_RESULT>=200)// если стало темно
						{
							KEY_V_BAT_ON();// то включаем подсветку
						}
						else //светло у нас
						{
							KEY_V_BAT_OFF();// выключем подсветку для экономии энергии
						}
						Lcd_on();// включаем экран
						lcd_power=1;//экран был включен
						
					}
				}
			}
		
			else //если не очень холодно
				{
					lcd_temp_correct_number_on=0;//сбросим счетчик
					
					if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN) // включаем экран если напряжение позволит чтонить на нем увидеть
					{
						
						if (ADC_LIGHT_RESULT>=200)// если стало темно
						{
							KEY_V_BAT_ON();// то включаем подсветку
						}
						else
						{
							KEY_V_BAT_OFF();// выключем подсветку для экономии энергии
						}
						Lcd_on();// включаем экран
						lcd_power=1;//экран был включен
						
					}
				}
		
			
		}
		#endif
		#if LCD_TYPE==0
		
		#endif
		#endif
		
		#if SLEEP_MODE_USED==1
		
		sleep_enable(); // разрешаем сон
		sleep_cpu(); // спать!
		#endif
		#if SLEEP_MODE_USED==0
		_delay_ms(SLEEP_MODE_DELAY);
		#endif
		
		//KEY_OFF();
		
		
		#endif
		
		#if MODE_OF_OPERATION==4//Уличный датчик №2		
			#if DS3231_IS_USED==1
				//сбросим флаг прерывания в DS3231
				/*устанавливаем указатель DS1307 на нужный адрес*/
				buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
				buf[1] = DS_REG_STATUS;      //адрес регистра   
				buf[2] &= ~_BV(DS_BIT_A1F);	//значение регистра  
				TWI_SendData(buf, 3);		//запись 
				/*Начинаем получать данные с часов*/
				DS3231_Get_All_reg_before();
				//_delay_ms(2);
				//DS3231_Get_Time_1();
				//KEY_V_BAT_ON();
				//DS3231_Get_Time_2();
				//KEY_V_BAT_OFF();
			#endif
			
			/*Если счетчик не переполнен*/
			if (load_number_i<1000000000)
			{
				load_number_i=load_number_i+1;//увеличим счетчик запусков
			}
			else//иначе обнулим его
			{
				load_number_i=0;
			}
			/* редко проверяем температуру, раз в N минут/секунд*/
			if (start_1wire >= _1WIRE_START_CYCLES_NUMBER-1)
			{
				// обнулим сетчик запусков для температуры
				start_1wire=0; 
				/*Если используем LCD*/
				#if LCD_IS_USED==1
					/*Проверим напряжение*/
					//#if ADC_IS_USED==1
					KEY_V_BAT_ON();
					ADC_V_batt();//запускаем АЦП
					KEY_V_BAT_OFF();
					//#endif
// 					KEY_V_BAT_ON();
// 					_delay_us(10);
// 					KEY_V_BAT_OFF();
					#if DS3231_IS_USED==1
					/*Получим данные из часов*/
					DS3231_Get_All_reg_after(&ds3231);
					/*Выставим будильник */
					//KEY_V_BAT_ON();
					DS3231_Set_Alarm1(&ds3231,DS3231_ALARM1_DELAY_S);
					//KEY_V_BAT_OFF();
					//_delay_us(100);
					//KEY_V_BAT_ON();
					#endif
					/*если напряжение позволит получить температуру с датчика*/
					if (ADC_BAT_RESULT_mV>V_1WIRE_MIN)
					{
						_1wire_stop_i=0;//обнулим счетчик неудачных попыток измерить температуру
						/*Если используем LCD 5110*/
						#if LCD_TYPE==1
							LcdClear();//очищаем буфер экрана в памяти МК (230 us)
						#endif
						#if LCD_TYPE==0
							LCD_Clear();//очищаем экран
						#endif
						//#if _1WIRE_IS_USED==1
						//KEY_V_BAT_OFF();
						
						#if DS3231_IS_USED==1
						//подождем завершения TWI операций
						TWI_Delay();
						#endif
						//KEY_V_BAT_ON();
							//sprintf(str2, "st_1w: %03i", start_1wire);//укажем количество неудачных попыток измерить температуру
							//LcdGotoXYFont(0,2);
							//LcdStr(FONT_1X, &str2);
						Set_Temp_to_LCD();//получим температуру с датчика температуры и сформируем данные для дисплея
						//KEY_V_BAT_OFF();	
						
						//KEY_V_BAT_OFF();						
					//#endif
					
					}
					else //если напряжение не позволяет получить температуру с датчика
					{
						_1wire_stop_i++;//увеличим счетчик неудачных попыток измерить температуру
						#if LCD_TYPE==1
							sprintf(str2, "T fail: %6u", _1wire_stop_i);//укажем количество неудачных попыток измерить температуру
							LcdGotoXYFont(0,2);
							LcdStr(FONT_1X, &str2);
						#endif
					}

				#endif
			}
			else // часто пытаемся проверить напряжение
			{

				//KEY_V_BAT_ON();
				start_1wire=start_1wire+1;
				//_delay_us(5);
				//KEY_V_BAT_OFF();
				//start_number=0;
				if (adc_start_number<ADC_START_CYCLES_NUMBER)// пока вообще ничего не делаем, только счетчик АЦП увеличим и затем уснем
				{
					adc_start_number++;
					/*Перед сном нужно установить будильник но мы не знаем текущее время*/
					/*нужно его вычислить на основании данных предидущего опроса часов и уставки будильника*/
				#if (DS3231_IS_USED==1)
					DS3231_Set_Alarm1_unknow(&ds3231,DS3231_ALARM1_DELAY_S);
					//_delay_ms(2);
				#endif
				}
				else//вот теперь проверяем напряжение
				{
				//обнулим счетчик циклов АЦП
					adc_start_number=0;
					#if LCD_IS_USED==1
						#if LCD_TYPE==1
 							//KEY_V_BAT_ON();
	// 						_delay_us(10);
	// 						KEY_V_BAT_OFF();
							/*Проверим напряжение*/
							//#if ADC_IS_USED==1
							ADC_V_batt();//запускаем АЦП
							//#endif
		// 					KEY_V_BAT_ON();
		// 					_delay_us(10);
	 						//KEY_V_BAT_OFF();
							#if DS3231_IS_USED==1
								/*Получим данные из часов*/
								DS3231_Get_All_reg_after(&ds3231);
								/*Заведем будильник */
								DS3231_Set_Alarm1(&ds3231,DS3231_ALARM1_DELAY_S);
								//_delay_ms(2);
								/*Укажем на экране текущее время*/
								//sprintf(str2, "Время:%02i:%02i:%02i", ((ds3231.ds3231_data.time.hours.hour_10*10)+ds3231.ds3231_data.time.hours.hour_1),(ds3231.ds3231_data.time.minutes.minutes_10*10+ds3231.ds3231_data.time.minutes.minutes_1),(ds3231.ds3231_data.time.seconds.seconds_10*10+ds3231.ds3231_data.time.seconds.seconds_1));//укажем текущее среднее значение АЦП
								//LcdGotoXYFont(0,4);
								//LcdStr(FONT_1X, &str2);
							#endif			
						#endif
					#endif
				}
			}
			//если напряжение позволит отобразить данные на дисплее 5110, отправим их на него
			#if LCD_IS_USED==1
				#if LCD_TYPE==1
					if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN)
					{
						//KEY_V_BAT_ON();
						/*выводим на экран напряжение*/
						ADC_V_bat_to_LCD();
						//KEY_V_BAT_OFF();
						#if DS3231_IS_USED==1
						/*Укажем на экране текущее время*/
						DS3231_time_to_LCD(&ds3231);
						#endif
						//KEY_V_BAT_ON();
						LcdUpdate();
						//KEY_V_BAT_OFF();
					}
					// поочереди включаем и выключем экран для экономии энергии каждые 8 с,
					// если напряжение ниже 5 вольт
					if ((lcd_power==0) && (ADC_BAT_RESULT_mV<V_BAT_ECONOM))// если напряжение ниже 5 вольт
					{
						lcd_power=1;
						Lcd_off();// выключем экран для экономии энергии
						KEY_LIGHT_OFF();// выключем подсветку для экономии энергии
					}
					else// если напряжение выше 5 вольт
					{
						lcd_power=0;
						if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN) // включаем экран если напряжение позволит чтонить на нем увидеть
						{
							if (ADC_LIGHT_RESULT>=ADC_LIGHT_ON_VALUE)// если стало темно
							{
								KEY_LIGHT_ON();// то включаем подсветку
							}
							Lcd_on();
						}
					}
				#endif
			#endif
	// 		KEY_V_BAT_ON();
			//разрешаем прерывание (int0 по умолчанию, при низком уровне на PD2)
			EIMSK |= _BV(INT0);
	// 		KEY_V_BAT_OFF();
			#if SLEEP_MODE_USED==1
				/*Перед сном нужно проверить что закончена запись в часы по TWI*/
				sleep_enable(); // разрешаем сон
				sleep_cpu(); // спать!
			#endif
			#if SLEEP_MODE_USED==0
				_delay_ms(SLEEP_MODE_DELAY);
			#endif
		#endif

    }
		
}
