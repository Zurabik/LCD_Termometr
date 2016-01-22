/*
 * nec.h
 *
 * Created: 25.08.2014 21:43:20
 *  Author: Vadim2
 */ 


#ifndef NEC_H_
#define NEC_H_

/*****************************************************
Chip type           : ATmega16
Program type        : Application
Clock frequency     : 8,000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
*****************************************************/
#include <stddef.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "settings.h"
#include "Message.h"
//#include <mega16.h>
//#include <delay.h>
//#define Tic_10mS (255-(F_CPU/(1024*100)))						//выставляем начальное значение TCNT2(10mS) 100 раз в секунду
#include "IR_LG_device.h"
#define Reset_T2 TCNT2 = 0										//сбрасываем таймер
#define Start_T2_1024 TCCR2B = (1<<CS02)|(1<<CS01)|(1<<CS00)	// настраиваем делитель 1024 (запускаем таймер2)
#define Stop_T2 TCCR2B = (0<<CS02)|(0<<CS01)|(0<<CS00)			// настраиваем делитель 0 (stop таймер2)
//#define Tic_10mS (255-(F_CPU/(1024*100)))						//выставляем начальное значение TCNT2(10mS) 100 раз в секунду
#define INT0_UP EICRA=(1<<ISC01)|(1<<ISC00) 					// настраиваем прерывание INT0 на срабатывание при нарастании сигнала
#define INT0_DOWN EICRA=(1<<ISC01)|(0<<ISC00)					// настраиваем прерывание INT0 на срабатывание при спаде сигнала

#define START_13_5ms_min (((13.5*F_CPU)/(1024000))*0.93)
#define START_13_5ms_max (((13.5*F_CPU)/(1024000))*1.04)
#define REPEAT_11_2ms_min (((11.25*F_CPU)/(1024000))*0.9)
#define REPEAT_11_2ms_max (((11.25*F_CPU)/(1024000))*1.1)
#define _1_2_22ms_min (((2.25*F_CPU)/(1024000))*0.7)
#define _1_2_22ms_max (((2.25*F_CPU)/(1024000))*1.3)
#define _0_1_11ms_min (((1.125*F_CPU)/(1024000))*0.7)
#define _0_1_11ms_max (((1.125*F_CPU)/(1024000))*1.3)
/*
#define START_13_5ms_min 230
#define START_13_5ms_max 250
#define REPEAT_11_2ms_min 190
#define REPEAT_11_2ms_max 210
#define _1_2_22ms_min 10
#define _1_2_22ms_max 30
#define _0_1_11ms_min 31
#define _0_1_11ms_max 50
*/
typedef struct IR_device
 {
	
	unsigned char addr_0;
	unsigned char addr_1;
	unsigned char cmd_0;
	unsigned char cmd_1;
	
} IR_code;

typedef union
{
	IR_code code_ir_message;
	uint32_t i32_ir_message;
	unsigned char ch_ir_message[sizeof(IR_code)];
}IR_mes;

typedef struct
{
	IR_mes message;
	unsigned start_cond : 1;
	unsigned repeat_cond : 1;
	unsigned b_cnt : 6;
	
}IRC_mes;

//IRC_mes IRC_message;
volatile IRC_mes IRC_message;

#define START		IRC_message.start_cond						//флаг стартового условия
#define REPEAT		IRC_message.repeat_cond						//флаг повтора
#define B_CNT		IRC_message.b_cnt							//счетчик принятых бит
#define ADDR0		IRC_message.message.code_ir_message.addr_0		//Адрес пульта неинвертированный байт
#define ADDR1		IRC_message.message.code_ir_message.addr_1		//Адрес пульта инвертированный байт
#define CMD0		IRC_message.message.code_ir_message.cmd_0		//Код кнопки неинвертированный байт
#define CMD1		IRC_message.message.code_ir_message.cmd_1		//Код кнопки инвертированный байт
#define IR_MESSAGE_i32	IRC_message.message.i32_ir_message				//Вся посылка 4*8 бит
#define IR_MESSAGE_ch	IRC_message.message.ch_ir_message			//Вся посылка 4*8 бит



//unsigned char tick = 0;			//буфер таймера2
//unsigned char IR_n_bit = 0;		//подсчет номера импульса
//volatile char timer2_10_ms = 0;	//подсчет времени для определения окончания посылки 10мс
/*
unsigned int tick = 0;          //счетчик тиков таймера
unsigned char b_cnt = 0;        //счетчик принятых бит
bit start_cond = 0;             //флаг стартового условия
bit repeat_cond = 0;            //флаг повтора

unsigned char addr_1 = 0;       //прямой байт адреса
unsigned char addr_0 = 0;       //инверсный байт адреса
unsigned char cmd_1 = 0;        //прямой байт команды
unsigned char cmd_0 = 0;        //инверсный байт команды
unsigned char addr = 0;         //байт адреса
unsigned char cmd = 0;          //байт команды
*/

/*
 * Имя                   :  IR_Init
 * Описание              :  Производит инициализацию ИК приемника
 * Аргумент(ы)           :  нет
 * Возвращаемое значение :  нет
 */
void IR_Init();


/*
 * Имя                   :  IR_Check_NEC()
 * Описание              :  Производит запись полученных бит с ИК датчика и проверку на принадлежность их протоколу NEC
 * Аргумент(ы)           :  нет
 * Возвращаемое значение :  Сохраняет успешно принятую посылку в структуре "IRC_message" и возвращяет код нажатой кнопки
 */
uint8_t IR_Check_NEC();



/*
// 	timer2_counter = TCNT2;
// 	if (IR_n_bit == 0)										// если импульс первый
// 	{
// 		if (timer2_counter >=240 && timer2_counter <244 )	//если прошло 9 ms - стартовый импульс							
// 		
// 			Reset_T2;								// сбрасываем таймер
// 			Start_T2_1024;							// настраиваем делитель 1024 (запускаем таймер)
// 			send_Uart(timer2_counter);				// узнаем сколько натикало
// //			INT0_UP; 								// настраиваем прерывание INT0 на срабатывание при нарастании сигнала
// 			//TCNT2 = 0;							// сбрасываем таймер
// 			//IR_n_bit++;
// 			//send_Uart_str("start");
// 	}
// 		else										// если  срабатывание при нарастании сигнала
// 		{
// 			if (timer2_counter >=162 && timer2_counter <165 ) //если прошло 9 ms - стартовый импульс
// 			{
// 				Reset_T2;							// сбрасываем таймер
// 				INT0_DOWN; 							// настраиваем прерывание INT0 на срабатывание при спаде сигнала
// 				send_Uart(timer2_counter);			// узнаем сколько натикало
// 				//Stop_T2;							// настраиваем делитель 0 (остановим таймер)
// 				
// 				IR_n_bit == 1;
// 				//return;
// 			}
// 			
// 			//EIMSK=(0<<INT0);						// настраиваем прерывание INT0 на stop
// 			//IR_n_bit++;	
// 		}
// 		
// 	} 
// 	if (IR_n_bit > 0)								// импульс не первый
// 	{
// 		if (EICRA==0x02)							// если  срабатывание при спаде сигнала
// 		{
			
//			if (timer2_counter >=80 && timer2_counter <83 ) // 4.5 ms начало посылки
//			{
//				Reset_T2;							// сбрасываем таймер
			//	INT0_UP; 							// настраиваем прерывание INT0 на срабатывание при нарастании сигнала
//			}
			
			//Start_T2_1024;						// настраиваем делитель 1024 (запускаем таймер)
//			Reset_T2;								// сбрасываем таймер
//			send_Uart(timer2_counter);				// узнаем сколько натикало
//			INT0_UP; 								// настраиваем прерывание INT0 на срабатывание при нарастании сигнала
			//TCNT2 = 0;							// сбрасываем таймер
			//IR_n_bit++;
			//send_Uart_str("start");
//		}
// 		else										// если  срабатывание при нарастании сигнала
// 		{
// 			if (timer2_counter >=162 && timer2_counter <165 ) // 9 ms
// 			{
// 				Reset_T2;							// сбрасываем таймер
// 				INT0_DOWN; 							// настраиваем прерывание INT0 на срабатывание при спаде сигнала
// 				send_Uart(timer2_counter);			// узнаем сколько натикало
// 				//Stop_T2;							// настраиваем делитель 0 (остановим таймер)
// 				
// 				IR_n_bit == 1;
// 			}
// 			
// 			//EIMSK=(0<<INT0);						// настраиваем прерывание INT0 на stop
// 			//IR_n_bit++;
// 		}
//PORTB=(0<<PINB1);
//sei();
*/
/*
// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
	TCNT0=0xFD;                                                             //запускаем таймер
	TCCR0=0x04;
	
	if (tick >= 139 && tick < 150)                                          //если случилось от 139 до 150 тиков (4.5 ms)
	{
		start_cond = 1;                                                 //фиксируем стартовое условие
		repeat_cond = 0;                                                //обнуляем флаг повтора
		addr_1 = addr_0 = cmd_1 = cmd_0 = 0;                            //и ранее полученную команду и адрес
	}
	if (tick >= 116 && tick < 139)                                          //если случилось от 116 до 138 тиков (2.5-4.5 ms)
	{
		repeat_cond = 1;                                                //фиксируем флаг повтора
		start_cond = 0;                                                 //обнуляем флаг стартового условия
		TCNT0=0x00;
	}
	if (tick >= 22 && tick < 116 && start_cond)                             //если случилось от 22 до 115 тиков
	{
		++b_cnt;                                                        //приняли "1" и увеличили счетчик битов
		if (b_cnt < 9) addr_1 = (addr_1 << 1) + 1;                      //первый байт - прямой адрес
		if (b_cnt >= 9 && b_cnt < 17) addr_0 = (addr_0 << 1) + 1;       //второй байт - инверсный адрес
		if (b_cnt >= 16 && b_cnt < 25) cmd_1 = (cmd_1 << 1) + 1;        //третий байт - прямая команда
		if (b_cnt >= 24) cmd_0 = (cmd_0 << 1) + 1;                      //четвертый байт - инверсная команда
	}
	if (tick >= 10 && tick < 22 && start_cond)                              //если случилось от 10 до 21 тика
	{
		++b_cnt;                                                        //приняли "0" и увеличили счетчик битов
		if (b_cnt < 9) addr_1 = (addr_1 << 1);                          //далее - аналогично по байтам
		if (b_cnt >= 9 && b_cnt < 17) addr_0 = (addr_0 << 1);
		if (b_cnt >= 16 && b_cnt < 25) cmd_1 = (cmd_1 << 1);
		if (b_cnt >= 24) cmd_0 = (cmd_0 << 1);
	}
	tick = 0;                                                               //обнулили тики
	if (b_cnt == 32)                                                        //если приняли уже 4 байта
	{
		//                 if ((addr_1+addr_0) == 0xFF) addr = addr_0;                  //закомментировано, потому как в пульте First
		//                         else addr = 0;                                       //16-bit адрес 0x6120, т.о. проверка адреса не проходит
		if ((cmd_1 + cmd_0) == 0xFF) cmd = cmd_1;                       //проверили правильность приема команды
		else cmd = 0;
		b_cnt = 0;                                                      //обнулили счетчик битов
		start_cond = 0;                                                 //сбросили стартовое условие
		repeat_cond = 0;
		TCCR0=0x00;                                                     //остановили таймер
		TCNT0=0x00;
	}
}



// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
	TCNT0=0xFD;                                                             //переинициализировали таймер - отсчитывает 96мкс
	if (++tick > 1200)                                                      //если насчитали более 1200 тиков
	{
		TCCR0=0x00;                                                     //сбросили все к исходному состоянию
		TCNT0=0x00;
		tick = 0;
		start_cond = 0;
		repeat_cond = 0;
		//addr_1 = addr_0 = cmd = 0;
	}
}


void __sign (char digit)        //индикатор
{
	switch (digit)
	{
		case 0 : PORTB = 0b01110111; break;
		
		case 1 : PORTB = 0b00000101; break;
		
		case 2 : PORTB = 0b01111001; break;
		
		case 3 : PORTB = 0b00111101; break;
		
		case 4 : PORTB = 0b00001111; break;
		
		case 5 : PORTB = 0b00111110; break;
		
		case 6 : PORTB = 0b01111110; break;
		
		case 7 : PORTB = 0b00010101; break;
		
		case 8 : PORTB = 0b01111111; break;
		
		case 9 : PORTB = 0b00111111; break;
		
		case 10 : PORTB = 0b01011111; break;   //'a'
		
		case 11 : PORTB = 0b01101110; break;   //'b'
		
		case 12 : PORTB = 0b01110010; break;   //'c'
		
		case 13 : PORTB = 0b01101101; break;   //'d'
		
		case 14 : PORTB = 0b01111010; break;   //'e'
		
		case 15 : PORTB = 0b01011010; break;   //'f'
		
		case 16 : PORTB = 0b00000000; break;   //' '
		
		default:  PORTB = 0b00111000;
	}
}

void __print (char a, char b, char c, char d, char e, char f, int t)      //Процедура вывода на индикатор символов с номерами a, b, с и d в течение t мс
{
	int i = 0;
	//if (a == 0) a = 20;     //Если первый символ равен 0 - ничего не выводим
	while (i < t/12)
	{
		__sign (a);
		PORTC.7 = 1;
		delay_ms(2);
		PORTC.7 = 0;
		
		__sign (b);
		PORTC.6 = 1;
		delay_ms(2);
		PORTC.6 = 0;
		
		__sign (c);
		PORTC.5 = 1;
		delay_ms(2);
		PORTC.5 = 0;
		
		__sign (d);
		PORTC.2 = 1;
		delay_ms(2);
		PORTC.2 = 0;
		
		__sign (e);
		PORTC.3 = 1;
		delay_ms(2);
		PORTC.3 = 0;
		
		__sign (f);
		PORTC.4 = 1;
		delay_ms(2);
		PORTC.4 = 0;
		
		i++;
	}
}
*/
/*
void main(void)
{
	// Declare your local variables here

	// Input/Output Ports initialization
	// Port A initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=Out Func0=In
	// State7=T State6=T State5=T State4=T State3=T State2=P State1=0 State0=P
	PORTA=0x05;
	DDRA=0x02;

	// Port B initialization
	// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
	// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0
	PORTB=0x00;
	DDRB=0xFF;

	// Port C initialization
	// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
	// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0
	PORTC=0x00;
	DDRC=0xFF;

	// Port D initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
	// State7=T State6=T State5=T State4=T State3=T State2=P State1=T State0=T
	PORTD=0x04;
	DDRD=0x00;

	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 31,250 kHz
	// Mode: Normal top=FFh
	// OC0 output: Disconnected
	TCCR0=0x00;
	TCNT0=0x00;
	OCR0=0x00;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: Timer 1 Stopped
	// Mode: Normal top=FFFFh
	// OC1A output: Discon.
	// OC1B output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer 1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A=0x00;
	TCCR1B=0x00;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: Timer 2 Stopped
	// Mode: Normal top=FFh
	// OC2 output: Disconnected
	ASSR=0x00;
	TCCR2=0x00;
	TCNT2=0x00;
	OCR2=0x00;

	// External Interrupt(s) initialization
	// INT0: Off
	// INT1: Off
	// INT2: Off
	MCUCR=0x00;
	MCUCSR=0x00;

	// External Interrupt(s) initialization
	// INT0: On
	// INT0 Mode: Falling Edge
	// INT1: Off
	// INT2: Off
	GICR|=0x40;
	MCUCR=0x02;
	MCUCSR=0x00;
	GIFR=0x40;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK=0x01;


	// Analog Comparator initialization
	// Analog Comparator: Off
	// Analog Comparator Input Capture by Timer/Counter 1: Off
	ACSR=0x80;
	SFIOR=0x00;

	#asm ("sei")

	while (1) __print (start_cond, repeat_cond, addr_1/16, addr_1%16, cmd/16, cmd%16, 12);

}

*/
#endif /* NEC_H_ */
