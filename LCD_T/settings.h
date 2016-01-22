/*
 * settings.h
 *
 * Created: 26.07.2014 1:01:00
 *  Author: Vadim2
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_
//Режим работы МК
#define MODE_OF_OPERATION 2
//домашний термометр-психрометр
#if MODE_OF_OPERATION==3
#ifndef F_CPU
/* prevent compiler error by supplying a default */
//# warning "F_CPU not defined for <util/delay.h>"
# define F_CPU 16000000UL
#endif
//#define F_CPU 2000000UL  // 2 MHz

#define UART_IS_USED 0			//Если UART используется 1 если нет 0
#define LCD_IS_USED 1			//Если LCD дисплей используется 1 если нет 0
#if LCD_IS_USED==1				//Если LCD дисплей используется
#define LCD_TYPE 1			//Тип LCD дисплея 0-символьный; 1-графичексий(Нокиа 5110)
#endif
#define SHIFT_REGISTER_IS_USED 0//Если сдвиговый регистр (74HC595/165) используется 1 если нет 0
#define ADC_IS_USED 1			//Если ADC используется 1 если нет 0
#if ADC_IS_USED==1
#define KEY_V_BAT_DDR		DDRD
#define KEY_V_BAT_PIN		PIND4
#define KEY_V_BAT_PORT		PORTD
#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//высокий уровень
#define KEY_V_BAT_ON()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//низкий уровень

#define KEY_LIGHT_DDR		DDRD
#define KEY_LIGHT_PIN		PIND3
#define KEY_LIGHT_PORT		PORTD
#define KEY_LIGHT_ON()		KEY_LIGHT_PORT |=  _BV(KEY_LIGHT_PIN);//высокий уровень
#define KEY_LIGHT_OFF()		KEY_LIGHT_PORT &= ~_BV(KEY_LIGHT_PIN);//низкий уровень

#define ADC_START_CYCLES_NUMBER 2		//Количество запусков МК без проверки напряжения 8*8=64 сек (~1 мин.)
#define ADC_CYCLES_NUMBER 32			//Количество запусков ADC для вычисления среднего значения mV
#define V_1WIRE_MIN 3000				//Минимальное напряжение для DS18B20
#define V_LCD_5110_MIN 2100				//Минимальное напряжение для LCD NOKIA 5110
#define V_BAT_ECONOM 5000				//Минимальное напряжение для LCD NOKIA 5110

#endif
#define NRF24L01_IS_USED 0					//Если NRF24L01 трансивер используется 1 если нет 0
#if (NRF24L01_IS_USED==1)
#define NRF24L01_TX_RX 1				//Если NRF24L01 передатчик  0 если приемник 1
//#define MAX_SENSOR_UNIT 10				// количество устройств (10)
#define NUMB_SENSOR_UNIT 7				// номер устройства (для приемника) (1)
#endif
#define _1WIRE_IS_USED 1					//Если 1-Wire используется 1 если нет 0
#if _1WIRE_IS_USED==1
#define _1WIRE_START_CYCLES_NUMBER 75	//Количество запусков МК без проверки температуры8*75=600 сек (10 мин.)
#endif
#define TIMER0_IS_USED 0		//Если Timer0(8bit) используется 1 если нет 0
#define IR_IS_USED 0			//Если IR используется 1 если нет 0
#define SLEEP_MODE_USED 0		//Если режим сна используется 1 если нет 0






#endif 


//Уличный термометр №2 на NIMH, с часами и прм/прд,
#if MODE_OF_OPERATION==4
#ifndef F_CPU
	/* prevent compiler error by supplying a default */
	//# warning "F_CPU not defined for <util/delay.h>"
	# define F_CPU						16000000UL
#endif
//#define F_CPU 2000000UL  // 2 MHz

//Если UART используется 1 если нет 0
#define UART_IS_USED					0
			
//Если LCD дисплей используется 1 если нет 0
#define LCD_IS_USED						1	
#if (LCD_IS_USED==1)			
	#define LCD_TYPE					1			//Тип LCD дисплея 0-символьный; 1-графичексий(Нокиа 5110)
	//если дисплей 5110
	#if (LCD_TYPE==1)
		//если нужен режим нескольких изображений
		#define LCD_FRAME_USED			0
		#if (LCD_FRAME_USED==1)
			//время (сек) в течениие которго показывается главный экран
			#define LCD_FRAME_MAIN_DELAY 10
			#define LCD_FRAME_SECOND_DELAY 5
		#endif
	#endif
#endif

//Если сдвиговый регистр (74HC595/165) используется 1 если нет 0
#define SHIFT_REGISTER_IS_USED			0

//Если ADC используется 1 если нет 0
#define ADC_IS_USED						1			
#if (ADC_IS_USED==1)
	#define KEY_V_BAT_USED				1	//Если маркер PD4 используется 1 если нет 0
	//Если маркер PD4 используется
	#if (KEY_V_BAT_USED==1)			
		#define KEY_V_BAT_DDR		DDRD
		#define KEY_V_BAT_PIN		PIND4
		#define KEY_V_BAT_PORT		PORTD
		#define KEY_V_BAT_ON()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//высокий уровень
		#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//низкий уровень
	#else
		#define KEY_V_BAT_ON()		nop();//высокий уровень
		#define KEY_V_BAT_OFF()		nop();//низкий уровень
	#endif
/*	
		//#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//высокий уровень
		//#define KEY_V_BAT_ON()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//низкий уровень
	//#define KEY_V_BAT_ON()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//высокий уровень
	//#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//низкий уровень
*/
	#define KEY_LIGHT_DDR				DDRD
	#define KEY_LIGHT_PIN				PIND3
	#define KEY_LIGHT_PORT				PORTD
	#define KEY_LIGHT_ON()				KEY_LIGHT_PORT |=  _BV(KEY_LIGHT_PIN);//высокий уровень
	#define KEY_LIGHT_OFF()				KEY_LIGHT_PORT &= ~_BV(KEY_LIGHT_PIN);//низкий уровень
	//Количество запусков МК без проверки напряжения
	#define ADC_START_CYCLES_NUMBER		1		
	//Количество запусков ADC для вычисления среднего значения mV не более 63
	#define ADC_CYCLES_NUMBER			16
	#if (ADC_CYCLES_NUMBER>63)
		#define _ADC_CYCLES_NUMBER		63	//не более 63
	#endif
	//Значение АЦП (светло-0...1023-темно) освещения при котором зажигается подсветка
	#define ADC_LIGHT_ON_VALUE			700 
	//#define ADC_BAT_RESULT_mV ((((adc_bat_res_i)/ADC_CYCLES_NUMBER)*65)/10) //средний результат ацп в виде напряжения в мВ
	//средний результат ацп баттарей в виде напряжения в мВ
	#define ADC_BAT_RESULT_mV			adc_bat_res_i32 
	//средний результат ацп фоторезистора в виде 0...1024
	#define ADC_LIGHT_RESULT_mV			adc_light_res_mV 
	//Минимальное напряжение для DS18B20 в мВ
	#define V_1WIRE_MIN					3000
	//Минимальное напряжение для отображения информации на LCD NOKIA 5110 в мВ
	#define V_LCD_5110_MIN				3000
	//Минимальное напряжение (мВ) для неконтролируемого расхода Энергии LCD NOKIA 5110,
	// ниже начинается экономия
	#define V_BAT_ECONOM				4400				
#endif

//Если NRF24L01 трансивер используется 1 если нет 0
#define NRF24L01_IS_USED				0					
#if (NRF24L01_IS_USED==1)
#define NRF24L01_TX_RX 1				//Если NRF24L01 передатчик  0 если приемник 1
//#define MAX_SENSOR_UNIT 10				// количество устройств (10)
#define NUMB_SENSOR_UNIT 7				// номер устройства (для приемника) (1)
#endif

//Если 1-Wire используется 1 если нет 0
#define _1WIRE_IS_USED					1					
#if (_1WIRE_IS_USED==1)
	//Количество минут без проверки температуры
	#define _1WIRE_START_MINUTES		10
	//Количество запусков МК без проверки температуры
	#define _1WIRE_START_CYCLES_NUMBER	(_1WIRE_START_MINUTES*20/DS3231_ALARM1_DELAY_S)	
	//#define _1WIRE_START_CYCLES_NUMBER 7	//75 Количество запусков МК без проверки температуры8*75=600 сек (10 мин.)
#endif

//Если Timer0(8bit) используется 1 если нет 0
#define TIMER0_IS_USED					0

//Если IR используется 1 если нет 0
#define IR_IS_USED						0
			
//Протокол TWI (I2C) Если  используется 1 если нет 0
#define TWI_IS_USED						0

//Модуль часов DS3231 + AT24C32
#define DS3231_IS_USED					1
#if (DS3231_IS_USED==1)
	#define DS3231_INT_DDR				DDRD
	#define DS3231_INT_PIN				PIND2
	#define DS3231_INT_PORT				PORTD
	#define TWI_IS_USED					1
	//частота TWI в кГц 1...400
	#define TWI_FREQ					400
	//уставка будильника №1 в секундах не более 59
	#define DS3231_ALARM1_DELAY_S		10		
#else
	//#define DS3231_ALARM1_DELAY_S 2
#endif

//Если режим сна используется 1 если нет 0
#define SLEEP_MODE_USED					1				
#if (SLEEP_MODE_USED==1)
	#define SLEEP_MODE_WD				0	//Если режим сна с WD используется 1 если нет 0
	#define SLEEP_MODE_DS3231			1	//Если режим сна с DS3231 используется 1 если нет 0
#else
	//#define _1WIRE_START_CYCLES_NUMBER 2	//Количество запусков МК без проверки температуры8*8=64 сек (1 мин.)
	#define ADC_START_CYCLES_NUMBER		1		
	#define ADC_CYCLES_NUMBER			8	
	#define SLEEP_MODE_DELAY			500	//"усыпляем" мк в режиме эмуляции
#endif





#endif //Уличный термометр


//Уличный термометр №1 ионисторы, без часов
#if MODE_OF_OPERATION==2
	#ifndef F_CPU
	/* prevent compiler error by supplying a default */
	//# warning "F_CPU not defined for <util/delay.h>"
	# define F_CPU 8000000UL
	#endif
	//#define F_CPU 2000000UL  // 2 MHz
	
	#define UART_IS_USED 0			//Если UART используется 1 если нет 0
	#define LCD_IS_USED 1			//Если LCD дисплей используется 1 если нет 0
	#if LCD_IS_USED==1				//Если LCD дисплей используется 
		#define LCD_TYPE 1			//Тип LCD дисплея 0-символьный; 1-графичексий(Нокиа 5110)
		//#define LCD_TEMPERATURE_CORRECTION_IS_USED 1	//Температурнная коррекция времени отображения данных на дисплее
		#define LCD_TEMPERATURE_CORRECTION_NUMBER 2		//на сколько циклов увеличиться отображение данных при -20
		//при низкой температуре ниже -20 данные за 10 сек не успевают отобразиться на дисплее нужно увеличить время их отображения
	#endif
	#define SHIFT_REGISTER_IS_USED 0//Если сдвиговый регистр (74HC595/165) используется 1 если нет 0
	#define ADC_IS_USED 1			//Если ADC используется 1 если нет 0
	#if ADC_IS_USED==1
		#define KEY_V_BAT_DDR		DDRD
		#define KEY_V_BAT_PIN		PIND4
		#define KEY_V_BAT_PORT		PORTD
		#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//высокий уровень
		#define KEY_V_BAT_ON()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//низкий уровень
		
		#define KEY_LIGHT_DDR		DDRD
		#define KEY_LIGHT_PIN		PIND3
		#define KEY_LIGHT_PORT		PORTD
		#define KEY_LIGHT_ON()		KEY_LIGHT_PORT |=  _BV(KEY_LIGHT_PIN);//высокий уровень
		#define KEY_LIGHT_OFF()		KEY_LIGHT_PORT &= ~_BV(KEY_LIGHT_PIN);//низкий уровень
		
		//#define ADC_BAT_RESULT_mV ((adc_bat_res_i/ADC_CYCLES_NUMBER)*48) //средний результат ацп в виде напряжения в мВ
		#define ADC_START_CYCLES_NUMBER 8		//Количество запусков МК без проверки напряжения 8*8=64 сек (~1 мин.)
		#define ADC_CYCLES_NUMBER 32			//Количество запусков ADC для вычисления среднего значения mV
		#define V_1WIRE_MIN 3000				//Минимальное напряжение для DS18B20
		#define V_LCD_5110_MIN 2100				//Минимальное напряжение для отображения информации на LCD NOKIA 5110
		#define V_BAT_ECONOM 5000				//Минимальное напряжение для неконтролируемого расхода Энергии LCD NOKIA 5110, ниже начинается экономия
		//средний результат ацп баттарей в виде напряжения в мВ
		#define ADC_BAT_RESULT_mV			adc_bat_res_i32
		//средний результат ацп фоторезистора в виде 0...1024
		#define ADC_LIGHT_RESULT_mV			adc_light_res_mV
	#endif
	#define NRF24L01_IS_USED 0					//Если NRF24L01 трансивер используется 1 если нет 0
	#if (NRF24L01_IS_USED==1)
		#define NRF24L01_TX_RX 1				//Если NRF24L01 передатчик  0 если приемник 1
	//#define MAX_SENSOR_UNIT 10				// количество устройств (10)
		#define NUMB_SENSOR_UNIT 7				// номер устройства (для приемника) (1)
	#endif
	#define _1WIRE_IS_USED 1					//Если 1-Wire используется 1 если нет 0
	#if _1WIRE_IS_USED==1
		#define _1WIRE_START_CYCLES_NUMBER 75	//Количество запусков МК без проверки температуры8*75=600 сек (10 мин.)
	#endif
	#define TIMER0_IS_USED 0		//Если Timer0(8bit) используется 1 если нет 0
	#define IR_IS_USED 0			//Если IR используется 1 если нет 0
	#define SLEEP_MODE_USED 1		//Если режим сна используется 1 если нет 0
	#if SLEEP_MODE_USED==0
	#define SLEEP_MODE_WD				1	//Если режим сна с WD используется 1 если нет 0
	#define _1WIRE_START_CYCLES_NUMBER 8	//Количество запусков МК без проверки температуры8*8=64 сек (1 мин.)
	#define ADC_START_CYCLES_NUMBER 1		//Количество запусков МК без проверки напряжения 8*1=8 сек 
	#define ADC_CYCLES_NUMBER 5				//Количество запусков ADC для вычисления среднего значения mV
	#define SLEEP_MODE_DELAY 500			//"усыпляем" мк в режиме эмуляции
 	#endif
	
	
	
	
	
#endif //Уличный термометр

//Передатчик (ведущий)
#if MODE_OF_OPERATION==0
	#define F_CPU 18432000UL  // 18.432 MHz
	#define UART_IS_USED 1			//Если UART используется 1 если нет 0
	#define LCD_IS_USED 0			//Если LCD дисплей используется 1 если нет 0
	#define SHIFT_REGISTER_IS_USED 0//Если сдвиговый регистр (74HC595/165) используется 1 если нет 0
	#define ADC_IS_USED 0			//Если ADC используется 1 если нет 0
	#define NRF24L01_IS_USED 0		//Если NRF24L01 трансивер используется 1 если нет 0
	#define NRF24L01_TX_RX 0		//Если NRF24L01 передатчик  0 если приемник 1
	#define MAX_SENSOR_UNIT 10		// количество устройств (10)
	#define NUMB_SENSOR_UNIT 1		// номер устройства (для приемника) (1)
	#define _1WIRE_IS_USED 0		//Если 1-Wire используется 1 если нет 0
	#define TIMER0_IS_USED 1		//Если Timer0(8bit) используется 1 если нет 0
	#define IR_IS_USED 1			//Если IR используется 1 если нет 0

#endif //Передатчик

//Приемник (ведомый)
#if MODE_OF_OPERATION==1
	#define F_CPU 16000000UL  // 16 MHz
	#define UART_IS_USED 0			//Если UART используется 1 если нет 0
	#define LCD_IS_USED 1			//Если LCD дисплей используется 1 если нет 0
	#define SHIFT_REGISTER_IS_USED 1//Если сдвиговый регистр (74HC595/165) используется 1 если нет 0
	#define ADC_IS_USED 1			//Если ADC используется 1 если нет 0
	#define NRF24L01_IS_USED 0		//Если NRF24L01 трансивер используется 1 если нет 0
	#define NRF24L01_TX_RX 1		//Если NRF24L01 передатчик  0 если приемник 1
	//#define MAX_SENSOR_UNIT 10		// количество устройств (10)
	#define NUMB_SENSOR_UNIT 1		// номер устройства (для приемника) (1)
	#define _1WIRE_IS_USED 1		//Если 1-Wire используется 1 если нет 0
	#define TIMER0_IS_USED 1		//Если Timer0(8bit) используется 1 если нет 0
	#define IR_IS_USED 0			//Если IR используется 1 если нет 0

#endif //Приемник

/*
#define CRYSTAL_FREQ 0			//Если 
								//8MHz		-2
								//18.432MHz	-1;
								//16 MHz	-0
#if (CRYSTAL_FREQ==2)		//
#define F_CPU 8000000UL  // 8.00 MHz

#endif
#if (CRYSTAL_FREQ==1)		//
#define F_CPU 18432000UL  // 18.432 MHz
#endif
#if (CRYSTAL_FREQ==0)
#define F_CPU 16000000UL  // 16 MHz
#endif
*/								



#endif /* SETTINGS_H_ */
