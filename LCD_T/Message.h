/*
 * Message.h
 *
 * Created: 16.08.2014 22:01:06
 *  Author: Vadim2
 */ 


#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <stddef.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "settings.h"

typedef  struct 					// структура дискретных выходов (1 байт)
{
	unsigned out_1 : 1;
	unsigned out_2 : 1;
	unsigned out_3 : 1;
	unsigned out_4 : 1;
	unsigned out_5 : 1;
	unsigned out_6 : 1;
	unsigned out_7 : 1;
	unsigned out_8 : 1;

} mf_bit_discrete_output;

typedef  union 						// объединение настроек освещения и статусных LED (1 байт)
{
	mf_bit_discrete_output bit_discrete_output;
	uint8_t ui8_discrete_output;				// Выходные лампы освещения и  полоски LED
	unsigned char ch_discrete_output;
} mf_discrete_output;



typedef struct 						// структура уставок внешних датчиков и освещения (1+5=6 байт)
{
	uint8_t set_temperature_dt;		// настройки датчика температуры
	uint8_t set_lighting_dt;		// настройки датчика освещения
	uint8_t set_pir_motion_dt;		// настройки PIR датчика движения
	uint8_t set_ultrasound_dt;		// настройки УЗ датчика расстояния
	uint8_t set_ir_reciver_dt;		// настройки IR приемника ДУ
	
} mf_setpoint;

typedef  struct 					// структура данных с IR датчика 1*4 байта
{
	unsigned char addr_0;
	unsigned char addr_1;
	unsigned char cmd_0;
	unsigned char cmd_1;
	
}mf_IR_data;

typedef  union 						// обьединение структуры данных с IR датчика
{
	mf_IR_data IR_data;
	uint32_t i32_IR_data;
	unsigned char ch_IR_data[sizeof(mf_IR_data)];
} u_IR_data;

typedef struct 						// структура данных внешних датчиков и освещения (10 байт)
{
	uint8_t get_temperature_data[2];// данные датчика температуры
	uint8_t get_lighting_data;		// данные датчика освещения
	uint8_t get_pir_motion_data;	// данные PIR датчика движения
	uint16_t get_ultrasound_data;	// данные УЗ датчика расстояния
	u_IR_data get_ir_reciver_data;	// данные IR приемника ДУ
	
} mf_data_dt;

typedef  struct 					// структура статуса
{
	unsigned load_st : 1;
	unsigned temperature_st : 1;
	unsigned lighting_st : 1;
	unsigned pir_motion_st : 1;
	unsigned ultrasound_st : 1;
	unsigned ir_reciver_st : 1;
	unsigned st_7 : 1;
	unsigned st_8 : 1;
	//	uint8_t led_lamp;			// Выходные лампы освещения и  полоски LED
	//	uint8_t led_status;			// Статусные LED
} mf_bit_status;
typedef  union 						// объединение статуса
{
	mf_bit_status	bit_status;
	uint8_t			ui8_status;
	unsigned char	ch_status;
} mf_status;

typedef struct _message				// структура содержимого сообщения с данными и настройками устройства размером не более 22 байта
{									// (1+1+6+10=18 байт)
	mf_status status_dt; 			//							
	uint8_t address_dt;				// адрес устройства
	mf_discrete_output discrete_output;					// Дискретные выходы
	mf_setpoint set_setpoint_dt;	// уставки датчиков устройства
	mf_data_dt  get_data_dt;		// данные датчиков устройства
} mf_message_st;


//#define MAX_SENSOR_UNIT 10			// количество устройств

typedef union 
{
	mf_message_st message; 
	unsigned char ch_message[sizeof(mf_message_st)];
}u_message;



//=================
//статус подключенных устройств
//=================
#define SENSOR_STATUS	message.status_dt.ch_status
#define LOAD_ST			message.status_dt.bit_status.load_st
#define TEMP_ST			message.status_dt.bit_status.temperature_st
#define LIGHTING_ST		message.status_dt.bit_status.lighting_st
#define PM_ST			message.status_dt.bit_status.pir_motion_st
#define US_ST			message.status_dt.bit_status.ultrasound_st
#define IR_ST			message.status_dt.bit_status.ir_reciver_st

#define SENSOR_ADDRESS	message.address_dt							// адресс устройства
// Дискретные выходы
#define CH_LAMP		message.discrete_output.ch_discrete_output				// соостоояние выходов на полоски LED и лампы освещения
#define SETUP_LAMP	message.discrete_output.ui8_discrete_output				// соостоояние выходов на полоски LED и лампы освещения
#define OUT_LED_1	message.discrete_output.bit_discrete_output.out_3		// выход полоски LED №1
#define OUT_LED_2	message.discrete_output.bit_discrete_output.out_4		// выход полоски LED №2
#define OUT_LED_3	message.discrete_output.bit_discrete_output.out_5		// выход полоски LED №3
#define OUT_LED_4	message.discrete_output.bit_discrete_output.out_7		// выход полоски LED №4
#define OUT_220_1	message.discrete_output.bit_discrete_output.out_2		// выход 220 В №1
#define OUT_220_2	message.discrete_output.bit_discrete_output.out_6		// выход 220 В №2
#define OUT_220_3	message.discrete_output.bit_discrete_output.out_8		// выход 220 В №3
#define OUT_4		message.discrete_output.bit_discrete_output.out_1		// выход 220 В №4(РЕЗЕРВ)

// IR приемник ДУ
#define GET_IR_DATA_ch	message.get_data_dt.get_ir_reciver_data.ch_IR_data	// данные IR приемника ДУ 1*4 Байта
#define GET_IR_DATA_i32	message.get_data_dt.get_ir_reciver_data.i32_IR_data	// данные IR приемника ДУ 32 Бита
#define GET_IR_BTT	message.get_data_dt.get_ir_reciver_data.IR_data.cmd_0	// код нажатой кнопки пульта ДУ 1 Байт
#define GET_IR_ADDR	message.get_data_dt.get_ir_reciver_data.IR_data.addr_0	// адрес пульта ДУ 1 Байт
#define SETUP_IR	message.set_setpoint_dt.set_ir_reciver_dt				// уставки IR приемника ДУ
#define IREN		7				// разрешаем работу IR

// датчик освещения (фоторезистор на ADC)
#define GET_LIGHTING	message.get_data_dt.get_lighting_data			// данные датчика освещения 1 Байт
#define SETUP_LIGHTING	message.set_setpoint_dt.set_lighting_dt			// уставки датчика освещения
#define LTEN		7				// разрешаем работу датчика освещения
#define LTPS2		2				// делитель частоты опроса датчика освещения
#define LTPS1		1				// делитель частоты опроса датчика освещения
#define LTPS0		0				// делитель частоты опроса датчика освещения

// датчик температуры (1-Wire DS18B20)
#define GET_TEMP	message.get_data_dt.get_temperature_data			// данные датчика температуры 2 Байта
#define SETUP_TEMP	message.set_setpoint_dt.set_temperature_dt			// уставки датчика температуры
#define TPEN		7				// разрешаем работу датчика температуры
#define TPR1		6				// разрешающая способность датчика температуры
#define TPR0		5				// разрешающая способность датчика температуры
#define TPPS2		2				// делитель частоты опроса датчика температуры
#define TPPS1		1				// делитель частоты опроса датчика температуры
#define TPPS0		0				// делитель частоты опроса датчика температуры

// PIR датчик движения (HC SR-501)
#define GET_PIR_DATA		message.get_data_dt.get_pir_motion_data		// данные PIR датчика движения 1 Байт(точнее 1 бит)
#define SETUP_PIR	message.set_setpoint_dt.set_pir_motion_dt			// уставки PIR датчика движения
#define PIREN		7				// разрешаем работу PIR датчика движения

// УЗ датчик расстояния(HC SR04)
#define GET_US		message.get_data_dt.get_ultrasound_data				// данные УЗ датчика расстояния 2 Байта
#define SETUP_US	message.set_setpoint_dt.set_ultrasound_dt			// уставки УЗ датчика расстояния
#define USEN		7				// разрешаем работу УЗ датчика расстояния
#define USPS2		2				// делитель частоты опроса УЗ датчика расстояния
#define USPS1		1				// делитель частоты опроса УЗ датчика расстояния
#define USPS0		0				// делитель частоты опроса УЗ датчика расстояния
#if NRF24L01_IS_USED==1
#if (NRF24L01_TX_RX==0)
volatile u_message message_unit[MAX_SENSOR_UNIT]; //  в режим передатчика массив MAX_SENSOR_UNIT
#else
volatile u_message message_unit[1]; //  в режим приемника 1 штука
#endif
#endif


//#define default_value 0x00

void set_message_default();

void set_all_message_default(unsigned char n);

void send_Uart_message2(unsigned char i);//   Отправка пакета через уарт

#endif /* MESSAGE_H_ */

