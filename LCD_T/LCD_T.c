/*
 * LCD_T.c
 *
 * Created: 22.07.2014 21:52:50
 *  Author: Vadim2
 */
//��� � ��� ��������� ��� ������������ �������
#include "settings.h" 
#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t start_number; //������ �������� ��� �����������

uint8_t adc_start_number; //������ �������� ��� ���



//��������� � �������������:
#if 1==1
/*=====================SLEEP_MODE===================================*/
#if SLEEP_MODE_USED==1
#include <avr/io.h>

#include <avr/sleep.h> // ����� ������� ������ ���
#if SLEEP_MODE_WD==1
#include <avr/wdt.h> // ����� ������������ ������ � ���������
ISR (WDT_vect)
{
	//_delay_us(100);
	
	//if (start_number<_1WIRE_START_CYCLES_NUMBER)
	//{
		//#if LCD_IS_USED==1 
		//LcdClear();
		//PORTB ^= _BV(PB4); // ����������� ���������
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
		//PORTB ^= _BV(PB4); // ����������� ���������
		//#if ADC_IS_USED==1
		//ADC_result();
		//#endif
		//Set_Temp_to_LCD();
		//#endif
	//}
	
	//LcdUpdate();
	WDTCSR |= _BV(WDIE); // ��������� ���������� �� ��������. ����� ����� �����.
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
#if (LCD_IS_USED==1)//���� LCD ������� ������������

	#if LCD_TYPE==0 //���� LCD ����������
	#include "compilers_4.h"
	#include "lcd_lib_2.h"
//#include "bcd.h"
//����� ���������� ������, �����
//����� � ����� ������ ������� LCD
//����� ������������ ���� � ����������
//������ ��������� � ��� ��� ������������� �������

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
	#if LCD_TYPE==1 //���� LCD �����������
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
uint8_t i;						//������� ���������
uint8_t tx_addr[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };		// ����� ��������� Receiver address
uint8_t rx_addr[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };		// ����� ��������� Receiver address
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
//#define _1WIRE_START_CYCLES_NUMBER 75	//���������� �������� �� ��� �������� �����������8*75=600 ��� (10 ���.)
#endif
/*===================== DS3231 ===================================*/

/*ISR (USART_RX_vect)
{
	cli(); //��������� ����������
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
		// ���������� ������� ������ �������
		// � ������������ ������������ ����� �������� � �����
		//TimerReloadValue++;
		//printf("TimerReloadValue : %u", TimerReloadValue);
		//send_Uart(13);
		break;
		
		case 0x53:// "S"
		tx_delay=tx_delay-10;
		printf("tx_delay : %u", tx_delay);
		send_Uart(13);
		// ���������� ������� ������ �������
		// � ������������ ������������ ����� �������� � �����
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
		send_Uart_str("����������� ");
		send_Uart(13);
		//Get_Temperatura();
		send_Uart(13);
		send_Uart_str("����������� ������");
		break;
		default:
		send_Uart(13);
		printf("you put:  %X", c);
		//send_Uart_str("you put: ");
		//send_Uart(c);
		// 		send_Uart_str(" �� ���!");
		send_Uart(13);
		//LCD_SendStr("Hi");
		break;
	}
	// 	send_Uart(13);
	// 	send_Uart_str("��������� ����������");
	sei(); //��������� ����������
	
	
	//����� ��������� ��������� ���������� ��� ��������� ������
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
	//���������� (�������)
	#if MODE_OF_OPERATION==0
	unsigned char tx_command[6];
	_delay_ms(1);
	//stdout = &usart_str;
	//set_message_default(1);
	/*====�������������� UART=====*/
	//volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	//stdout=stdin=&uart_stream;		// ���������, ���� ����� �������� printf
	#if (UART_IS_USED==1)
	volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	stdout=stdin=&uart_stream;		// ���������, ���� ����� �������� printf
	//stdin =&uart_stream;		// ���������, ���� ����� �������� printf
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
	//���������� LCD �������
	#if (LCD_IS_USED==1)
	//�������������� �������
	// LCD_Init();
	// LCD_Goto(0,0);
	// LCD_SendStr("Load...");
	// ��������� ����� �������
	LCD_SetUserChar(quarterNote, 0);
	LCD_SetUserChar(eighthNote,  1);
	LCD_SetUserChar(sixteenthNote, 2);
	
	//������������� ������ � ������� ������
	LCD_Goto(0,0);
	LCD_SendStr(textCompiler);
	LCD_SendStr(textTypePort);
	
	LCD_Goto(0,1);
	LCD_SendStr(textBus);
	LCD_SendStr(textBF);
	
	//������� ����� ������
	LCD_WriteData(2);
	_delay_ms(1000);
	LCD_Clear();
	LCD_Goto(0,0);
	LCD_SendStr("Ok");
	#endif
	/*===========================*/
	//���� ������������ Timer0
	#if (TIMER0_IS_USED==1)
	Init_Timer0();
	#endif

	#if (IR_IS_USED==1)
	IR_Init();
	#endif
	
	#if (NRF24L01_IS_USED==1)			//���� ������������ ���������
	#if (NRF24L01_TX_RX==0)		//���� ������������ ����� �����������
	set_all_message_default(MAX_SENSOR_UNIT);
	setup_NRF24L01_Transmitter(tx_addr);	//��������� � ����� �����������
	#endif
	#if (NRF24L01_TX_RX==1)
	//set_all_message_default(1);
	set_message_default();
	rx_addr[4]=message_unit[0].SENSOR_ADDRESS;
	setup_NRF24L01_Receiver(rx_addr);		//��������� � ����� ���������
	#endif
	#endif
	
	#if (_1WIRE_IS_USED==1)				//���� ������������ 1-Wire
	//		unsigned int tmp = 0;
	//		unsigned char temperature;
	//�������������� 1-Wire
	OWI_Init(BUS);
	Init_Temperatura();
	//puts( "Init_Temperatura\r\n" );
	//_delay_ms(1000);
	// Get_Temperatura();
	
	#endif
	#endif
	
	//�������� (�������)
	#if MODE_OF_OPERATION==1
	unsigned char tx_command[6];
	_delay_ms(1);
	//stdout = &usart_str;
	//set_message_default(1);
	/*====�������������� UART=====*/
	//volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	//stdout=stdin=&uart_stream;		// ���������, ���� ����� �������� printf
	#if (UART_IS_USED==1)
	volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	stdout=stdin=&uart_stream;		// ���������, ���� ����� �������� printf
	//stdin =&uart_stream;		// ���������, ���� ����� �������� printf
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
	//���������� LCD �������
	#if (LCD_IS_USED==1)
	//�������������� �������
	// LCD_Init();
	// LCD_Goto(0,0);
	// LCD_SendStr("Load...");
	// ��������� ����� �������
	LCD_SetUserChar(quarterNote, 0);
	LCD_SetUserChar(eighthNote,  1);
	LCD_SetUserChar(sixteenthNote, 2);
	
	//������������� ������ � ������� ������
	LCD_Goto(0,0);
	LCD_SendStr(textCompiler);
	LCD_SendStr(textTypePort);
	
	LCD_Goto(0,1);
	LCD_SendStr(textBus);
	LCD_SendStr(textBF);
	
	//������� ����� ������
	LCD_WriteData(2);
	_delay_ms(1000);
	LCD_Clear();
	LCD_Goto(0,0);
	LCD_SendStr("Ok");
	#endif
	/*===========================*/
	//���� ������������ Timer0
	#if (TIMER0_IS_USED==1)
	Init_Timer0();
	#endif

	#if (IR_IS_USED==1)
	IR_Init();
	#endif
	
	#if (NRF24L01_IS_USED==1)			//���� ������������ ���������
	#if (NRF24L01_TX_RX==0)		//���� ������������ ����� �����������
	set_all_message_default(MAX_SENSOR_UNIT);
	setup_NRF24L01_Transmitter(tx_addr);	//��������� � ����� �����������
	#endif
	#if (NRF24L01_TX_RX==1)
	//set_all_message_default(1);
	set_message_default();
	rx_addr[4]=message_unit[0].SENSOR_ADDRESS;
	setup_NRF24L01_Receiver(rx_addr);		//��������� � ����� ���������
	#endif
	#endif
	
	#if (_1WIRE_IS_USED==1)				//���� ������������ 1-Wire
	//		unsigned int tmp = 0;
	//		unsigned char temperature;
	//�������������� 1-Wire
	OWI_Init(BUS);
	Init_Temperatura();
	//puts( "Init_Temperatura\r\n" );
	//_delay_ms(1000);
	// Get_Temperatura();
	
	#endif
	#endif
	
	//������� ������ �1
	#if MODE_OF_OPERATION==2
	//�������� ����� �� �����!!
	uint8_t lcd_temp_correct_number_on=0;
	uint8_t lcd_temp_correct_number_off=0;
	// ���� �� �����
	KEY_V_BAT_DDR=(1<<KEY_V_BAT_PIN);
	KEY_V_BAT_OFF();
	//CLKPR = (1 << CLKPCE); // Enable change of CLKPS bits
	//CLKPR = (1 << CLKPS1); // Set prescaler to 4, and system clock to 2 MHz
	MCUCR = (1 << BODS); 
	uint16_t _1wire_stop_i=0;
	//unsigned char tx_command[6];
	//_delay_ms(1);
	/*====�������������� UART=====*/
	#if (UART_IS_USED==1)
	volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	stdout=stdin=&uart_stream;		// ���������, ���� ����� �������� printf
	//stdin =&uart_stream;		// ���������, ���� ����� �������� printf
	USART_Init(MYUBRR);
	printf("��������");
	send_Uart(13);
	#endif
	//sei();
	
	#if SLEEP_MODE_USED==1
		#if SLEEP_MODE_WD==1
		//������������� ��������
		wdt_reset(); // ����������
		wdt_enable(WDTO_8S); // ��������� ������� 8 ���
		WDTCSR |= _BV(WDIE); // ��������� ���������� �� ��������. ����� ����� �����.
		sei(); // ��������� ����������
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); // ���� ����� - �� �� ������
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
	//���������� LCD �������
	#if (LCD_IS_USED==1)
		#if LCD_TYPE==0
		 //�������������� �������
		 LCD_Init();
		 LCD_Goto(0,0);
		 LCD_SendStr("Load...");
		 //_delay_ms(1000);
		// ��������� ����� �������
	 	 LCD_SetUserChar(quarterNote, 0);
	 	 LCD_SetUserChar(eighthNote,  1);
	 	 LCD_SetUserChar(sixteenthNote, 2);
	 
	 	 //������������� ������ � ������� ������
	 	 LCD_Goto(0,0);
	 	 LCD_SendStr(textCompiler);
	 	 LCD_SendStr(textTypePort);
	 	 
	 	 LCD_Goto(0,1);
	 	 LCD_SendStr(textBus);
	 	 LCD_SendStr(textBF);
	 
	 	 //������� ����� ������
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
			LcdFStr(FONT_2X,(unsigned char*)PSTR("������!"));
			LcdUpdate();
		#endif
	#endif
	/*===========================*/
	//���� ������������ Timer0
	#if (TIMER0_IS_USED==1)				
		Init_Timer0();
	#endif

	#if (IR_IS_USED==1)
		IR_Init();
	#endif
	
	#if (NRF24L01_IS_USED==1)			//���� ������������ ���������
		 #if (NRF24L01_TX_RX==0)		//���� ������������ ����� �����������
			 set_all_message_default(MAX_SENSOR_UNIT);
			 setup_NRF24L01_Transmitter(tx_addr);	//��������� � ����� ����������� 
		 #endif
		 #if (NRF24L01_TX_RX==1)
			 //set_all_message_default(1);
			 set_message_default();
			  rx_addr[4]=message_unit[0].SENSOR_ADDRESS;
			 setup_NRF24L01_Receiver(rx_addr);		//��������� � ����� ���������
		 #endif
	#endif
	
	#if (_1WIRE_IS_USED==1)				//���� ������������ 1-Wire
//		unsigned int tmp = 0;
//		unsigned char temperature;
     //�������������� 1-Wire
	 //printf("�������������� 1-Wire");
	 //send_Uart(13);
	 OWI_Init(BUS);
     Init_Temperatura();
	 //puts( "Init_Temperatura\r\n" );
	 //_delay_ms(1000);
	// Get_Temperatura();
	
	#endif 
	#endif

	//������� ������ �2
	#if MODE_OF_OPERATION==4
	
	// ���� �� �����
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
	KEY_LIGHT_OFF(); //��������� ���������
	//CLKPR = (1 << CLKPCE); // Enable change of CLKPS bits
	//CLKPR = (1 << CLKPS1); // Set prescaler to 4, and system clock to 2 MHz
	MCUCR = (1 << BODS);
	uint16_t _1wire_stop_i=0;
	//unsigned char tx_command[6];
	//_delay_ms(1);
	/*====�������������� UART=====*/
	#if (UART_IS_USED==1)
	volatile FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
	stdout=stdin=&uart_stream;		// ���������, ���� ����� �������� printf
	//stdin =&uart_stream;		// ���������, ���� ����� �������� printf
	USART_Init(MYUBRR);
	printf("��������");
	send_Uart(13);
	#endif
	//sei();
	#if SLEEP_MODE_USED==1
		#if SLEEP_MODE_WD==1
			//������������� ��������
			wdt_reset(); // ����������
			wdt_enable(WDTO_8S); // ��������� ������� 8 ���
			WDTCSR |= _BV(WDIE); // ��������� ���������� �� ��������. ����� ����� �����.
			sei(); // ��������� ����������
			set_sleep_mode(SLEEP_MODE_PWR_DOWN); // ���� ����� - �� �� ������
		#else
		sei(); // ��������� ����������
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); // ���� ����� - �� �� ������
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
	//���������� LCD �������
	#if (LCD_IS_USED==1)
	#if LCD_TYPE==0
	//�������������� �������
	LCD_Init();
	LCD_Goto(0,0);
	LCD_SendStr("Load...");
	//_delay_ms(1000);
	// ��������� ����� �������
	LCD_SetUserChar(quarterNote, 0);
	LCD_SetUserChar(eighthNote,  1);
	LCD_SetUserChar(sixteenthNote, 2);
	
	//������������� ������ � ������� ������
	LCD_Goto(0,0);
	LCD_SendStr(textCompiler);
	LCD_SendStr(textTypePort);
	
	LCD_Goto(0,1);
	LCD_SendStr(textBus);
	LCD_SendStr(textBF);
	
	//������� ����� ������
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
	LcdFStr(FONT_2X,(unsigned char*)PSTR("������!"));
	LcdUpdate();
	_delay_ms(2000);
	#endif
	#endif
	/*===========================*/
	//���� ������������ Timer0
	#if (TIMER0_IS_USED==1)
	Init_Timer0();
	#endif

	#if (IR_IS_USED==1)
	IR_Init();
	#endif
	
	#if (NRF24L01_IS_USED==1)			//���� ������������ ���������
	#if (NRF24L01_TX_RX==0)		//���� ������������ ����� �����������
	set_all_message_default(MAX_SENSOR_UNIT);
	setup_NRF24L01_Transmitter(tx_addr);	//��������� � ����� �����������
	#endif
	#if (NRF24L01_TX_RX==1)
	//set_all_message_default(1);
	set_message_default();
	rx_addr[4]=message_unit[0].SENSOR_ADDRESS;
	setup_NRF24L01_Receiver(rx_addr);		//��������� � ����� ���������
	#endif
	#endif
	
	#if _1WIRE_IS_USED==1				//���� ������������ 1-Wire
	uint8_t start_1wire=0; //������ �������� ��� �����������
	//		unsigned int tmp = 0;
	//		unsigned char temperature;
	//�������������� 1-Wire
	//printf("�������������� 1-Wire");
	//send_Uart(13);
	OWI_Init(BUS);
	Init_Temperatura();
	//puts( "Init_Temperatura\r\n" );
	//_delay_ms(1000);
	// Get_Temperatura();
	
	#endif
	
	#if (DS3231_IS_USED==1)
		// ���� �� ����
		uint16_t twi_max_delay=0;
		DS3231_INT_DDR &= ~_BV(DS3231_INT_PIN);
		//PORTD |= 1<<DS3231_INT_PIN;
		//DS3231_INT_DDR=(0<<DS3231_INT_PIN);
		//��������� ���������� (int0 �� ���������, ��� ������ ������ �� PD2)
		EIMSK |= _BV(INT0);
		TWI_MasterInit(TWI_FREQ);
		sei();
		//DS3231_Set_One_Register(128,DS_REG_STATUS);
		//_delay_ms(1);
		
		//KEY_V_BAT_ON();
		DS3231_Set_Time(&ds3231,0,30,19);
		_delay_ms(1);
		//DS3231_Set_One_Register(128,DS_REG_A1SEC);//��������� ��������� �� ������ �������
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
		#if MODE_OF_OPERATION==0//���������� (�������)
		#if (NRF24L01_IS_USED==1)		//���� ������������ ���������
		
		#if (NRF24L01_TX_RX==0)		//� ������ �����������
		
		tx_command[0]=SETUP_ALL_SETTING;
		tx_command[1]++;
		tx_command[2]=1;
		tx_command[3]=1;
		tx_command[4]=1;
		tx_command[5]=1;
		tx_command[6]=1;
		//printf("����� �����������. \n\r");
		for (uint8_t n=0; n < MAX_SENSOR_UNIT; n++)
		{
			printf(n);
			tx_addr[4]=message_unit[n].SENSOR_ADDRESS;

			//SR_Send_8b_STCP(i_shift);
			
			Get_Transmit_NRF24L01(tx_command, tx_addr);	//���������� ���������
			//tx_Delay(tx_delay);			//����� � ��
			//i++;
			_delay_us(500);
		}

		_delay_ms(100);
		
		#endif
		
		#if (NRF24L01_TX_RX==1)		//� ������ ���������
		Check_Receive_NRF24L01();		//��������� ������ �� ���������
		#endif
		
		#endif
		
		#if (TIMER0_IS_USED==1)			//���� ������������ Timer0
		Check_Timer0();
		#endif
		#endif
		
		#if MODE_OF_OPERATION==1//�������� (�������)
		#if (NRF24L01_IS_USED==1)		//���� ������������ ���������
		
		#if (NRF24L01_TX_RX==0)		//� ������ �����������
		
		tx_command[0]=SETUP_ALL_SETTING;
		tx_command[1]++;
		tx_command[2]=1;
		tx_command[3]=1;
		tx_command[4]=1;
		tx_command[5]=1;
		tx_command[6]=1;
		//printf("����� �����������. \n\r");
		for (uint8_t n=0; n < MAX_SENSOR_UNIT; n++)
		{
			printf(n);
			tx_addr[4]=message_unit[n].SENSOR_ADDRESS;

			//SR_Send_8b_STCP(i_shift);
			
			Get_Transmit_NRF24L01(tx_command, tx_addr);	//���������� ���������
			//tx_Delay(tx_delay);			//����� � ��
			//i++;
			_delay_us(500);
		}

		_delay_ms(100);
		
		#endif
		
		#if (NRF24L01_TX_RX==1)		//� ������ ���������
		Check_Receive_NRF24L01();		//��������� ������ �� ���������
		#endif
		
		#endif
		
		#if (TIMER0_IS_USED==1)			//���� ������������ Timer0
		Check_Timer0();
		#endif
		#endif
		
		#if MODE_OF_OPERATION==2//������� ������ �1
		//Get_all_Temperatura();
		//Set_Temp_to_LCD();
		//ADC_result();
		//_delay_ms(2000);
		//KEY_ON();
		if (load_number_i<1000000000)
		{
			load_number_i=load_number_i+1;//�������� ������� ��������
		}
		else
		{
			load_number_i=0;
		}
		//adc_i++;
		//_delay_ms(10);
		
		
		if (start_number>=_1WIRE_START_CYCLES_NUMBER)// ����� ��������� �����������, ��� � 10 �����
		{
			start_number=0; // ������� ������ �������� ��� �����������
			#if LCD_IS_USED==1
				if (ADC_BAT_RESULT_mV>V_1WIRE_MIN)//���� ���������� �������� �������� ����������� � ������� 
				{
					_1wire_stop_i=0;//������� ������� ��������� ������� �������� �����������
					#if LCD_TYPE==1
						LcdClear();//������� ����� ������ � ������ ��
					#endif
					#if LCD_TYPE==0
						LCD_Clear();//������� �����
					#endif
					#if ADC_IS_USED==1
						ADC_V_batt();//��������� ���
					#endif
					#if _1WIRE_IS_USED==1
						Set_Temp_to_LCD();//������� ����������� � ������� � ���������� ������ ��� �������
					#endif
				}
				else //���� ���������� �� ��������� �������� ����������� � �������
				{
					_1wire_stop_i++;//�������� ������� ��������� ������� �������� �����������
					#if LCD_TYPE==1
						sprintf(str2, "T fail: %6u", _1wire_stop_i);//������ ���������� ��������� ������� �������� �����������
						LcdGotoXYFont(0,2);
						LcdStr(FONT_1X, &str2);
					#endif
				}
				#if LCD_TYPE==1
					if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN)//���� ���������� �������� ���������� ������ �� �������, �������� �� �� ����
					{
						/*������� �� ����� ����������*/
						ADC_V_bat_to_LCD();
						LcdUpdate();
					}
					#endif
			#endif
		}
		else // ����� �������� ��������� ����������
		{
			start_number++;
			//start_number=0;
			if (adc_start_number<ADC_START_CYCLES_NUMBER)// ���� ������ ������ �� ������, ������ ������� �������� � ����� �����
			{
				adc_start_number++;
			} 
			else//��� ������ ��������� ���������� 
			{
				adc_start_number=0;
				#if LCD_IS_USED==1
					
						#if ADC_IS_USED==1
						ADC_V_batt();//��������� ��� 
						/*������� �� ����� ����������*/
						ADC_V_bat_to_LCD();
						#endif
						#if LCD_TYPE==1
						if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN)
							{
								
								LcdUpdate();//���� ���������� �������� ���������� ������ �� �������, �������� �� �� ����
							}
						#endif
				#endif
			}
		}
		#if LCD_IS_USED==1

		#if LCD_TYPE==1


		// ��������� �������� � �������� ����� ��� �������� ������� ������ 8 �,
		// ���� ���������� ���� 5 �����
		
		if ((lcd_power==1) && (ADC_BAT_RESULT_mV<V_BAT_ECONOM))//���� ����� ������� � ���������� �� ������� ������� ��������
		{
			if ((Get_Temp_int())<=108)// ���� ����� ������� -20
			{
				if (lcd_temp_correct_number_off<LCD_TEMPERATURE_CORRECTION_NUMBER)// ���� ��������� ����� ��� ����
				{
					lcd_temp_correct_number_off++;//�������� ���
				}
				else// ���� ��������� ����� ��� ����
				{
					lcd_temp_correct_number_off=0;//������� �������
					Lcd_off();// �������� ����� ��� �������� �������
					KEY_V_BAT_OFF();// �������� ��������� ��� �������� �������
					lcd_power=0;//����� ��� �������
				}
			}
			else //���� �� ����� �������
			{
				lcd_temp_correct_number_off=0;//������� �������
				Lcd_off();// �������� ����� ��� �������� �������
				KEY_V_BAT_OFF();// �������� ��������� ��� �������� �������
				lcd_power=0;//����� ��� �������
			}
			
		}
		else// ���� ��������� �������
		{
			
			if (Get_Temp_int()<=108)// ���� ����� ������� -20
			{
				if (lcd_temp_correct_number_on<LCD_TEMPERATURE_CORRECTION_NUMBER)// ���� �������� ����� ��� ����
				{
					lcd_temp_correct_number_on++;//�������� ���
				}
				else// ���� �������� ����� ��� ����
				{
					lcd_temp_correct_number_on=0;//������� �������
					
					if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN) // �������� ����� ���� ���������� �������� ������� �� ��� �������
					{
						if (ADC_LIGHT_RESULT>=200)// ���� ����� �����
						{
							KEY_V_BAT_ON();// �� �������� ���������
						}
						else //������ � ���
						{
							KEY_V_BAT_OFF();// �������� ��������� ��� �������� �������
						}
						Lcd_on();// �������� �����
						lcd_power=1;//����� ��� �������
						
					}
				}
			}
		
			else //���� �� ����� �������
				{
					lcd_temp_correct_number_on=0;//������� �������
					
					if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN) // �������� ����� ���� ���������� �������� ������� �� ��� �������
					{
						
						if (ADC_LIGHT_RESULT>=200)// ���� ����� �����
						{
							KEY_V_BAT_ON();// �� �������� ���������
						}
						else
						{
							KEY_V_BAT_OFF();// �������� ��������� ��� �������� �������
						}
						Lcd_on();// �������� �����
						lcd_power=1;//����� ��� �������
						
					}
				}
		
			
		}
		#endif
		#if LCD_TYPE==0
		
		#endif
		#endif
		
		#if SLEEP_MODE_USED==1
		
		sleep_enable(); // ��������� ���
		sleep_cpu(); // �����!
		#endif
		#if SLEEP_MODE_USED==0
		_delay_ms(SLEEP_MODE_DELAY);
		#endif
		
		//KEY_OFF();
		
		
		#endif
		
		#if MODE_OF_OPERATION==4//������� ������ �2		
			#if DS3231_IS_USED==1
				//������� ���� ���������� � DS3231
				/*������������� ��������� DS1307 �� ������ �����*/
				buf[0] = (DS1307_ADR<<1)|0; //�������� �����
				buf[1] = DS_REG_STATUS;      //����� ��������   
				buf[2] &= ~_BV(DS_BIT_A1F);	//�������� ��������  
				TWI_SendData(buf, 3);		//������ 
				/*�������� �������� ������ � �����*/
				DS3231_Get_All_reg_before();
				//_delay_ms(2);
				//DS3231_Get_Time_1();
				//KEY_V_BAT_ON();
				//DS3231_Get_Time_2();
				//KEY_V_BAT_OFF();
			#endif
			
			/*���� ������� �� ����������*/
			if (load_number_i<1000000000)
			{
				load_number_i=load_number_i+1;//�������� ������� ��������
			}
			else//����� ������� ���
			{
				load_number_i=0;
			}
			/* ����� ��������� �����������, ��� � N �����/������*/
			if (start_1wire >= _1WIRE_START_CYCLES_NUMBER-1)
			{
				// ������� ������ �������� ��� �����������
				start_1wire=0; 
				/*���� ���������� LCD*/
				#if LCD_IS_USED==1
					/*�������� ����������*/
					//#if ADC_IS_USED==1
					KEY_V_BAT_ON();
					ADC_V_batt();//��������� ���
					KEY_V_BAT_OFF();
					//#endif
// 					KEY_V_BAT_ON();
// 					_delay_us(10);
// 					KEY_V_BAT_OFF();
					#if DS3231_IS_USED==1
					/*������� ������ �� �����*/
					DS3231_Get_All_reg_after(&ds3231);
					/*�������� ��������� */
					//KEY_V_BAT_ON();
					DS3231_Set_Alarm1(&ds3231,DS3231_ALARM1_DELAY_S);
					//KEY_V_BAT_OFF();
					//_delay_us(100);
					//KEY_V_BAT_ON();
					#endif
					/*���� ���������� �������� �������� ����������� � �������*/
					if (ADC_BAT_RESULT_mV>V_1WIRE_MIN)
					{
						_1wire_stop_i=0;//������� ������� ��������� ������� �������� �����������
						/*���� ���������� LCD 5110*/
						#if LCD_TYPE==1
							LcdClear();//������� ����� ������ � ������ �� (230 us)
						#endif
						#if LCD_TYPE==0
							LCD_Clear();//������� �����
						#endif
						//#if _1WIRE_IS_USED==1
						//KEY_V_BAT_OFF();
						
						#if DS3231_IS_USED==1
						//�������� ���������� TWI ��������
						TWI_Delay();
						#endif
						//KEY_V_BAT_ON();
							//sprintf(str2, "st_1w: %03i", start_1wire);//������ ���������� ��������� ������� �������� �����������
							//LcdGotoXYFont(0,2);
							//LcdStr(FONT_1X, &str2);
						Set_Temp_to_LCD();//������� ����������� � ������� ����������� � ���������� ������ ��� �������
						//KEY_V_BAT_OFF();	
						
						//KEY_V_BAT_OFF();						
					//#endif
					
					}
					else //���� ���������� �� ��������� �������� ����������� � �������
					{
						_1wire_stop_i++;//�������� ������� ��������� ������� �������� �����������
						#if LCD_TYPE==1
							sprintf(str2, "T fail: %6u", _1wire_stop_i);//������ ���������� ��������� ������� �������� �����������
							LcdGotoXYFont(0,2);
							LcdStr(FONT_1X, &str2);
						#endif
					}

				#endif
			}
			else // ����� �������� ��������� ����������
			{

				//KEY_V_BAT_ON();
				start_1wire=start_1wire+1;
				//_delay_us(5);
				//KEY_V_BAT_OFF();
				//start_number=0;
				if (adc_start_number<ADC_START_CYCLES_NUMBER)// ���� ������ ������ �� ������, ������ ������� ��� �������� � ����� �����
				{
					adc_start_number++;
					/*����� ���� ����� ���������� ��������� �� �� �� ����� ������� �����*/
					/*����� ��� ��������� �� ��������� ������ ����������� ������ ����� � ������� ����������*/
				#if (DS3231_IS_USED==1)
					DS3231_Set_Alarm1_unknow(&ds3231,DS3231_ALARM1_DELAY_S);
					//_delay_ms(2);
				#endif
				}
				else//��� ������ ��������� ����������
				{
				//������� ������� ������ ���
					adc_start_number=0;
					#if LCD_IS_USED==1
						#if LCD_TYPE==1
 							//KEY_V_BAT_ON();
	// 						_delay_us(10);
	// 						KEY_V_BAT_OFF();
							/*�������� ����������*/
							//#if ADC_IS_USED==1
							ADC_V_batt();//��������� ���
							//#endif
		// 					KEY_V_BAT_ON();
		// 					_delay_us(10);
	 						//KEY_V_BAT_OFF();
							#if DS3231_IS_USED==1
								/*������� ������ �� �����*/
								DS3231_Get_All_reg_after(&ds3231);
								/*������� ��������� */
								DS3231_Set_Alarm1(&ds3231,DS3231_ALARM1_DELAY_S);
								//_delay_ms(2);
								/*������ �� ������ ������� �����*/
								//sprintf(str2, "�����:%02i:%02i:%02i", ((ds3231.ds3231_data.time.hours.hour_10*10)+ds3231.ds3231_data.time.hours.hour_1),(ds3231.ds3231_data.time.minutes.minutes_10*10+ds3231.ds3231_data.time.minutes.minutes_1),(ds3231.ds3231_data.time.seconds.seconds_10*10+ds3231.ds3231_data.time.seconds.seconds_1));//������ ������� ������� �������� ���
								//LcdGotoXYFont(0,4);
								//LcdStr(FONT_1X, &str2);
							#endif			
						#endif
					#endif
				}
			}
			//���� ���������� �������� ���������� ������ �� ������� 5110, �������� �� �� ����
			#if LCD_IS_USED==1
				#if LCD_TYPE==1
					if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN)
					{
						//KEY_V_BAT_ON();
						/*������� �� ����� ����������*/
						ADC_V_bat_to_LCD();
						//KEY_V_BAT_OFF();
						#if DS3231_IS_USED==1
						/*������ �� ������ ������� �����*/
						DS3231_time_to_LCD(&ds3231);
						#endif
						//KEY_V_BAT_ON();
						LcdUpdate();
						//KEY_V_BAT_OFF();
					}
					// ��������� �������� � �������� ����� ��� �������� ������� ������ 8 �,
					// ���� ���������� ���� 5 �����
					if ((lcd_power==0) && (ADC_BAT_RESULT_mV<V_BAT_ECONOM))// ���� ���������� ���� 5 �����
					{
						lcd_power=1;
						Lcd_off();// �������� ����� ��� �������� �������
						KEY_LIGHT_OFF();// �������� ��������� ��� �������� �������
					}
					else// ���� ���������� ���� 5 �����
					{
						lcd_power=0;
						if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN) // �������� ����� ���� ���������� �������� ������� �� ��� �������
						{
							if (ADC_LIGHT_RESULT>=ADC_LIGHT_ON_VALUE)// ���� ����� �����
							{
								KEY_LIGHT_ON();// �� �������� ���������
							}
							Lcd_on();
						}
					}
				#endif
			#endif
	// 		KEY_V_BAT_ON();
			//��������� ���������� (int0 �� ���������, ��� ������ ������ �� PD2)
			EIMSK |= _BV(INT0);
	// 		KEY_V_BAT_OFF();
			#if SLEEP_MODE_USED==1
				/*����� ���� ����� ��������� ��� ��������� ������ � ���� �� TWI*/
				sleep_enable(); // ��������� ���
				sleep_cpu(); // �����!
			#endif
			#if SLEEP_MODE_USED==0
				_delay_ms(SLEEP_MODE_DELAY);
			#endif
		#endif

    }
		
}
