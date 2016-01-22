/*
 * settings.h
 *
 * Created: 26.07.2014 1:01:00
 *  Author: Vadim2
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_
//����� ������ ��
#define MODE_OF_OPERATION 2
//�������� ���������-����������
#if MODE_OF_OPERATION==3
#ifndef F_CPU
/* prevent compiler error by supplying a default */
//# warning "F_CPU not defined for <util/delay.h>"
# define F_CPU 16000000UL
#endif
//#define F_CPU 2000000UL  // 2 MHz

#define UART_IS_USED 0			//���� UART ������������ 1 ���� ��� 0
#define LCD_IS_USED 1			//���� LCD ������� ������������ 1 ���� ��� 0
#if LCD_IS_USED==1				//���� LCD ������� ������������
#define LCD_TYPE 1			//��� LCD ������� 0-����������; 1-�����������(����� 5110)
#endif
#define SHIFT_REGISTER_IS_USED 0//���� ��������� ������� (74HC595/165) ������������ 1 ���� ��� 0
#define ADC_IS_USED 1			//���� ADC ������������ 1 ���� ��� 0
#if ADC_IS_USED==1
#define KEY_V_BAT_DDR		DDRD
#define KEY_V_BAT_PIN		PIND4
#define KEY_V_BAT_PORT		PORTD
#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//������� �������
#define KEY_V_BAT_ON()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//������ �������

#define KEY_LIGHT_DDR		DDRD
#define KEY_LIGHT_PIN		PIND3
#define KEY_LIGHT_PORT		PORTD
#define KEY_LIGHT_ON()		KEY_LIGHT_PORT |=  _BV(KEY_LIGHT_PIN);//������� �������
#define KEY_LIGHT_OFF()		KEY_LIGHT_PORT &= ~_BV(KEY_LIGHT_PIN);//������ �������

#define ADC_START_CYCLES_NUMBER 2		//���������� �������� �� ��� �������� ���������� 8*8=64 ��� (~1 ���.)
#define ADC_CYCLES_NUMBER 32			//���������� �������� ADC ��� ���������� �������� �������� mV
#define V_1WIRE_MIN 3000				//����������� ���������� ��� DS18B20
#define V_LCD_5110_MIN 2100				//����������� ���������� ��� LCD NOKIA 5110
#define V_BAT_ECONOM 5000				//����������� ���������� ��� LCD NOKIA 5110

#endif
#define NRF24L01_IS_USED 0					//���� NRF24L01 ��������� ������������ 1 ���� ��� 0
#if (NRF24L01_IS_USED==1)
#define NRF24L01_TX_RX 1				//���� NRF24L01 ����������  0 ���� �������� 1
//#define MAX_SENSOR_UNIT 10				// ���������� ��������� (10)
#define NUMB_SENSOR_UNIT 7				// ����� ���������� (��� ���������) (1)
#endif
#define _1WIRE_IS_USED 1					//���� 1-Wire ������������ 1 ���� ��� 0
#if _1WIRE_IS_USED==1
#define _1WIRE_START_CYCLES_NUMBER 75	//���������� �������� �� ��� �������� �����������8*75=600 ��� (10 ���.)
#endif
#define TIMER0_IS_USED 0		//���� Timer0(8bit) ������������ 1 ���� ��� 0
#define IR_IS_USED 0			//���� IR ������������ 1 ���� ��� 0
#define SLEEP_MODE_USED 0		//���� ����� ��� ������������ 1 ���� ��� 0






#endif 


//������� ��������� �2 �� NIMH, � ������ � ���/���,
#if MODE_OF_OPERATION==4
#ifndef F_CPU
	/* prevent compiler error by supplying a default */
	//# warning "F_CPU not defined for <util/delay.h>"
	# define F_CPU						16000000UL
#endif
//#define F_CPU 2000000UL  // 2 MHz

//���� UART ������������ 1 ���� ��� 0
#define UART_IS_USED					0
			
//���� LCD ������� ������������ 1 ���� ��� 0
#define LCD_IS_USED						1	
#if (LCD_IS_USED==1)			
	#define LCD_TYPE					1			//��� LCD ������� 0-����������; 1-�����������(����� 5110)
	//���� ������� 5110
	#if (LCD_TYPE==1)
		//���� ����� ����� ���������� �����������
		#define LCD_FRAME_USED			0
		#if (LCD_FRAME_USED==1)
			//����� (���) � �������� ������� ������������ ������� �����
			#define LCD_FRAME_MAIN_DELAY 10
			#define LCD_FRAME_SECOND_DELAY 5
		#endif
	#endif
#endif

//���� ��������� ������� (74HC595/165) ������������ 1 ���� ��� 0
#define SHIFT_REGISTER_IS_USED			0

//���� ADC ������������ 1 ���� ��� 0
#define ADC_IS_USED						1			
#if (ADC_IS_USED==1)
	#define KEY_V_BAT_USED				1	//���� ������ PD4 ������������ 1 ���� ��� 0
	//���� ������ PD4 ������������
	#if (KEY_V_BAT_USED==1)			
		#define KEY_V_BAT_DDR		DDRD
		#define KEY_V_BAT_PIN		PIND4
		#define KEY_V_BAT_PORT		PORTD
		#define KEY_V_BAT_ON()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//������� �������
		#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//������ �������
	#else
		#define KEY_V_BAT_ON()		nop();//������� �������
		#define KEY_V_BAT_OFF()		nop();//������ �������
	#endif
/*	
		//#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//������� �������
		//#define KEY_V_BAT_ON()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//������ �������
	//#define KEY_V_BAT_ON()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//������� �������
	//#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//������ �������
*/
	#define KEY_LIGHT_DDR				DDRD
	#define KEY_LIGHT_PIN				PIND3
	#define KEY_LIGHT_PORT				PORTD
	#define KEY_LIGHT_ON()				KEY_LIGHT_PORT |=  _BV(KEY_LIGHT_PIN);//������� �������
	#define KEY_LIGHT_OFF()				KEY_LIGHT_PORT &= ~_BV(KEY_LIGHT_PIN);//������ �������
	//���������� �������� �� ��� �������� ����������
	#define ADC_START_CYCLES_NUMBER		1		
	//���������� �������� ADC ��� ���������� �������� �������� mV �� ����� 63
	#define ADC_CYCLES_NUMBER			16
	#if (ADC_CYCLES_NUMBER>63)
		#define _ADC_CYCLES_NUMBER		63	//�� ����� 63
	#endif
	//�������� ��� (������-0...1023-�����) ��������� ��� ������� ���������� ���������
	#define ADC_LIGHT_ON_VALUE			700 
	//#define ADC_BAT_RESULT_mV ((((adc_bat_res_i)/ADC_CYCLES_NUMBER)*65)/10) //������� ��������� ��� � ���� ���������� � ��
	//������� ��������� ��� �������� � ���� ���������� � ��
	#define ADC_BAT_RESULT_mV			adc_bat_res_i32 
	//������� ��������� ��� ������������� � ���� 0...1024
	#define ADC_LIGHT_RESULT_mV			adc_light_res_mV 
	//����������� ���������� ��� DS18B20 � ��
	#define V_1WIRE_MIN					3000
	//����������� ���������� ��� ����������� ���������� �� LCD NOKIA 5110 � ��
	#define V_LCD_5110_MIN				3000
	//����������� ���������� (��) ��� ����������������� ������� ������� LCD NOKIA 5110,
	// ���� ���������� ��������
	#define V_BAT_ECONOM				4400				
#endif

//���� NRF24L01 ��������� ������������ 1 ���� ��� 0
#define NRF24L01_IS_USED				0					
#if (NRF24L01_IS_USED==1)
#define NRF24L01_TX_RX 1				//���� NRF24L01 ����������  0 ���� �������� 1
//#define MAX_SENSOR_UNIT 10				// ���������� ��������� (10)
#define NUMB_SENSOR_UNIT 7				// ����� ���������� (��� ���������) (1)
#endif

//���� 1-Wire ������������ 1 ���� ��� 0
#define _1WIRE_IS_USED					1					
#if (_1WIRE_IS_USED==1)
	//���������� ����� ��� �������� �����������
	#define _1WIRE_START_MINUTES		10
	//���������� �������� �� ��� �������� �����������
	#define _1WIRE_START_CYCLES_NUMBER	(_1WIRE_START_MINUTES*20/DS3231_ALARM1_DELAY_S)	
	//#define _1WIRE_START_CYCLES_NUMBER 7	//75 ���������� �������� �� ��� �������� �����������8*75=600 ��� (10 ���.)
#endif

//���� Timer0(8bit) ������������ 1 ���� ��� 0
#define TIMER0_IS_USED					0

//���� IR ������������ 1 ���� ��� 0
#define IR_IS_USED						0
			
//�������� TWI (I2C) ����  ������������ 1 ���� ��� 0
#define TWI_IS_USED						0

//������ ����� DS3231 + AT24C32
#define DS3231_IS_USED					1
#if (DS3231_IS_USED==1)
	#define DS3231_INT_DDR				DDRD
	#define DS3231_INT_PIN				PIND2
	#define DS3231_INT_PORT				PORTD
	#define TWI_IS_USED					1
	//������� TWI � ��� 1...400
	#define TWI_FREQ					400
	//������� ���������� �1 � �������� �� ����� 59
	#define DS3231_ALARM1_DELAY_S		10		
#else
	//#define DS3231_ALARM1_DELAY_S 2
#endif

//���� ����� ��� ������������ 1 ���� ��� 0
#define SLEEP_MODE_USED					1				
#if (SLEEP_MODE_USED==1)
	#define SLEEP_MODE_WD				0	//���� ����� ��� � WD ������������ 1 ���� ��� 0
	#define SLEEP_MODE_DS3231			1	//���� ����� ��� � DS3231 ������������ 1 ���� ��� 0
#else
	//#define _1WIRE_START_CYCLES_NUMBER 2	//���������� �������� �� ��� �������� �����������8*8=64 ��� (1 ���.)
	#define ADC_START_CYCLES_NUMBER		1		
	#define ADC_CYCLES_NUMBER			8	
	#define SLEEP_MODE_DELAY			500	//"��������" �� � ������ ��������
#endif





#endif //������� ���������


//������� ��������� �1 ���������, ��� �����
#if MODE_OF_OPERATION==2
	#ifndef F_CPU
	/* prevent compiler error by supplying a default */
	//# warning "F_CPU not defined for <util/delay.h>"
	# define F_CPU 8000000UL
	#endif
	//#define F_CPU 2000000UL  // 2 MHz
	
	#define UART_IS_USED 0			//���� UART ������������ 1 ���� ��� 0
	#define LCD_IS_USED 1			//���� LCD ������� ������������ 1 ���� ��� 0
	#if LCD_IS_USED==1				//���� LCD ������� ������������ 
		#define LCD_TYPE 1			//��� LCD ������� 0-����������; 1-�����������(����� 5110)
		//#define LCD_TEMPERATURE_CORRECTION_IS_USED 1	//�������������� ��������� ������� ����������� ������ �� �������
		#define LCD_TEMPERATURE_CORRECTION_NUMBER 2		//�� ������� ������ ����������� ����������� ������ ��� -20
		//��� ������ ����������� ���� -20 ������ �� 10 ��� �� �������� ������������ �� ������� ����� ��������� ����� �� �����������
	#endif
	#define SHIFT_REGISTER_IS_USED 0//���� ��������� ������� (74HC595/165) ������������ 1 ���� ��� 0
	#define ADC_IS_USED 1			//���� ADC ������������ 1 ���� ��� 0
	#if ADC_IS_USED==1
		#define KEY_V_BAT_DDR		DDRD
		#define KEY_V_BAT_PIN		PIND4
		#define KEY_V_BAT_PORT		PORTD
		#define KEY_V_BAT_OFF()		KEY_V_BAT_PORT |=  _BV(KEY_V_BAT_PIN);//������� �������
		#define KEY_V_BAT_ON()		KEY_V_BAT_PORT &= ~_BV(KEY_V_BAT_PIN);//������ �������
		
		#define KEY_LIGHT_DDR		DDRD
		#define KEY_LIGHT_PIN		PIND3
		#define KEY_LIGHT_PORT		PORTD
		#define KEY_LIGHT_ON()		KEY_LIGHT_PORT |=  _BV(KEY_LIGHT_PIN);//������� �������
		#define KEY_LIGHT_OFF()		KEY_LIGHT_PORT &= ~_BV(KEY_LIGHT_PIN);//������ �������
		
		//#define ADC_BAT_RESULT_mV ((adc_bat_res_i/ADC_CYCLES_NUMBER)*48) //������� ��������� ��� � ���� ���������� � ��
		#define ADC_START_CYCLES_NUMBER 8		//���������� �������� �� ��� �������� ���������� 8*8=64 ��� (~1 ���.)
		#define ADC_CYCLES_NUMBER 32			//���������� �������� ADC ��� ���������� �������� �������� mV
		#define V_1WIRE_MIN 3000				//����������� ���������� ��� DS18B20
		#define V_LCD_5110_MIN 2100				//����������� ���������� ��� ����������� ���������� �� LCD NOKIA 5110
		#define V_BAT_ECONOM 5000				//����������� ���������� ��� ����������������� ������� ������� LCD NOKIA 5110, ���� ���������� ��������
		//������� ��������� ��� �������� � ���� ���������� � ��
		#define ADC_BAT_RESULT_mV			adc_bat_res_i32
		//������� ��������� ��� ������������� � ���� 0...1024
		#define ADC_LIGHT_RESULT_mV			adc_light_res_mV
	#endif
	#define NRF24L01_IS_USED 0					//���� NRF24L01 ��������� ������������ 1 ���� ��� 0
	#if (NRF24L01_IS_USED==1)
		#define NRF24L01_TX_RX 1				//���� NRF24L01 ����������  0 ���� �������� 1
	//#define MAX_SENSOR_UNIT 10				// ���������� ��������� (10)
		#define NUMB_SENSOR_UNIT 7				// ����� ���������� (��� ���������) (1)
	#endif
	#define _1WIRE_IS_USED 1					//���� 1-Wire ������������ 1 ���� ��� 0
	#if _1WIRE_IS_USED==1
		#define _1WIRE_START_CYCLES_NUMBER 75	//���������� �������� �� ��� �������� �����������8*75=600 ��� (10 ���.)
	#endif
	#define TIMER0_IS_USED 0		//���� Timer0(8bit) ������������ 1 ���� ��� 0
	#define IR_IS_USED 0			//���� IR ������������ 1 ���� ��� 0
	#define SLEEP_MODE_USED 1		//���� ����� ��� ������������ 1 ���� ��� 0
	#if SLEEP_MODE_USED==0
	#define SLEEP_MODE_WD				1	//���� ����� ��� � WD ������������ 1 ���� ��� 0
	#define _1WIRE_START_CYCLES_NUMBER 8	//���������� �������� �� ��� �������� �����������8*8=64 ��� (1 ���.)
	#define ADC_START_CYCLES_NUMBER 1		//���������� �������� �� ��� �������� ���������� 8*1=8 ��� 
	#define ADC_CYCLES_NUMBER 5				//���������� �������� ADC ��� ���������� �������� �������� mV
	#define SLEEP_MODE_DELAY 500			//"��������" �� � ������ ��������
 	#endif
	
	
	
	
	
#endif //������� ���������

//���������� (�������)
#if MODE_OF_OPERATION==0
	#define F_CPU 18432000UL  // 18.432 MHz
	#define UART_IS_USED 1			//���� UART ������������ 1 ���� ��� 0
	#define LCD_IS_USED 0			//���� LCD ������� ������������ 1 ���� ��� 0
	#define SHIFT_REGISTER_IS_USED 0//���� ��������� ������� (74HC595/165) ������������ 1 ���� ��� 0
	#define ADC_IS_USED 0			//���� ADC ������������ 1 ���� ��� 0
	#define NRF24L01_IS_USED 0		//���� NRF24L01 ��������� ������������ 1 ���� ��� 0
	#define NRF24L01_TX_RX 0		//���� NRF24L01 ����������  0 ���� �������� 1
	#define MAX_SENSOR_UNIT 10		// ���������� ��������� (10)
	#define NUMB_SENSOR_UNIT 1		// ����� ���������� (��� ���������) (1)
	#define _1WIRE_IS_USED 0		//���� 1-Wire ������������ 1 ���� ��� 0
	#define TIMER0_IS_USED 1		//���� Timer0(8bit) ������������ 1 ���� ��� 0
	#define IR_IS_USED 1			//���� IR ������������ 1 ���� ��� 0

#endif //����������

//�������� (�������)
#if MODE_OF_OPERATION==1
	#define F_CPU 16000000UL  // 16 MHz
	#define UART_IS_USED 0			//���� UART ������������ 1 ���� ��� 0
	#define LCD_IS_USED 1			//���� LCD ������� ������������ 1 ���� ��� 0
	#define SHIFT_REGISTER_IS_USED 1//���� ��������� ������� (74HC595/165) ������������ 1 ���� ��� 0
	#define ADC_IS_USED 1			//���� ADC ������������ 1 ���� ��� 0
	#define NRF24L01_IS_USED 0		//���� NRF24L01 ��������� ������������ 1 ���� ��� 0
	#define NRF24L01_TX_RX 1		//���� NRF24L01 ����������  0 ���� �������� 1
	//#define MAX_SENSOR_UNIT 10		// ���������� ��������� (10)
	#define NUMB_SENSOR_UNIT 1		// ����� ���������� (��� ���������) (1)
	#define _1WIRE_IS_USED 1		//���� 1-Wire ������������ 1 ���� ��� 0
	#define TIMER0_IS_USED 1		//���� Timer0(8bit) ������������ 1 ���� ��� 0
	#define IR_IS_USED 0			//���� IR ������������ 1 ���� ��� 0

#endif //��������

/*
#define CRYSTAL_FREQ 0			//���� 
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
