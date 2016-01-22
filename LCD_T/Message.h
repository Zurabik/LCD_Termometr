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

typedef  struct 					// ��������� ���������� ������� (1 ����)
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

typedef  union 						// ����������� �������� ��������� � ��������� LED (1 ����)
{
	mf_bit_discrete_output bit_discrete_output;
	uint8_t ui8_discrete_output;				// �������� ����� ��������� �  ������� LED
	unsigned char ch_discrete_output;
} mf_discrete_output;



typedef struct 						// ��������� ������� ������� �������� � ��������� (1+5=6 ����)
{
	uint8_t set_temperature_dt;		// ��������� ������� �����������
	uint8_t set_lighting_dt;		// ��������� ������� ���������
	uint8_t set_pir_motion_dt;		// ��������� PIR ������� ��������
	uint8_t set_ultrasound_dt;		// ��������� �� ������� ����������
	uint8_t set_ir_reciver_dt;		// ��������� IR ��������� ��
	
} mf_setpoint;

typedef  struct 					// ��������� ������ � IR ������� 1*4 �����
{
	unsigned char addr_0;
	unsigned char addr_1;
	unsigned char cmd_0;
	unsigned char cmd_1;
	
}mf_IR_data;

typedef  union 						// ����������� ��������� ������ � IR �������
{
	mf_IR_data IR_data;
	uint32_t i32_IR_data;
	unsigned char ch_IR_data[sizeof(mf_IR_data)];
} u_IR_data;

typedef struct 						// ��������� ������ ������� �������� � ��������� (10 ����)
{
	uint8_t get_temperature_data[2];// ������ ������� �����������
	uint8_t get_lighting_data;		// ������ ������� ���������
	uint8_t get_pir_motion_data;	// ������ PIR ������� ��������
	uint16_t get_ultrasound_data;	// ������ �� ������� ����������
	u_IR_data get_ir_reciver_data;	// ������ IR ��������� ��
	
} mf_data_dt;

typedef  struct 					// ��������� �������
{
	unsigned load_st : 1;
	unsigned temperature_st : 1;
	unsigned lighting_st : 1;
	unsigned pir_motion_st : 1;
	unsigned ultrasound_st : 1;
	unsigned ir_reciver_st : 1;
	unsigned st_7 : 1;
	unsigned st_8 : 1;
	//	uint8_t led_lamp;			// �������� ����� ��������� �  ������� LED
	//	uint8_t led_status;			// ��������� LED
} mf_bit_status;
typedef  union 						// ����������� �������
{
	mf_bit_status	bit_status;
	uint8_t			ui8_status;
	unsigned char	ch_status;
} mf_status;

typedef struct _message				// ��������� ����������� ��������� � ������� � ����������� ���������� �������� �� ����� 22 �����
{									// (1+1+6+10=18 ����)
	mf_status status_dt; 			//							
	uint8_t address_dt;				// ����� ����������
	mf_discrete_output discrete_output;					// ���������� ������
	mf_setpoint set_setpoint_dt;	// ������� �������� ����������
	mf_data_dt  get_data_dt;		// ������ �������� ����������
} mf_message_st;


//#define MAX_SENSOR_UNIT 10			// ���������� ���������

typedef union 
{
	mf_message_st message; 
	unsigned char ch_message[sizeof(mf_message_st)];
}u_message;



//=================
//������ ������������ ���������
//=================
#define SENSOR_STATUS	message.status_dt.ch_status
#define LOAD_ST			message.status_dt.bit_status.load_st
#define TEMP_ST			message.status_dt.bit_status.temperature_st
#define LIGHTING_ST		message.status_dt.bit_status.lighting_st
#define PM_ST			message.status_dt.bit_status.pir_motion_st
#define US_ST			message.status_dt.bit_status.ultrasound_st
#define IR_ST			message.status_dt.bit_status.ir_reciver_st

#define SENSOR_ADDRESS	message.address_dt							// ������ ����������
// ���������� ������
#define CH_LAMP		message.discrete_output.ch_discrete_output				// ����������� ������� �� ������� LED � ����� ���������
#define SETUP_LAMP	message.discrete_output.ui8_discrete_output				// ����������� ������� �� ������� LED � ����� ���������
#define OUT_LED_1	message.discrete_output.bit_discrete_output.out_3		// ����� ������� LED �1
#define OUT_LED_2	message.discrete_output.bit_discrete_output.out_4		// ����� ������� LED �2
#define OUT_LED_3	message.discrete_output.bit_discrete_output.out_5		// ����� ������� LED �3
#define OUT_LED_4	message.discrete_output.bit_discrete_output.out_7		// ����� ������� LED �4
#define OUT_220_1	message.discrete_output.bit_discrete_output.out_2		// ����� 220 � �1
#define OUT_220_2	message.discrete_output.bit_discrete_output.out_6		// ����� 220 � �2
#define OUT_220_3	message.discrete_output.bit_discrete_output.out_8		// ����� 220 � �3
#define OUT_4		message.discrete_output.bit_discrete_output.out_1		// ����� 220 � �4(������)

// IR �������� ��
#define GET_IR_DATA_ch	message.get_data_dt.get_ir_reciver_data.ch_IR_data	// ������ IR ��������� �� 1*4 �����
#define GET_IR_DATA_i32	message.get_data_dt.get_ir_reciver_data.i32_IR_data	// ������ IR ��������� �� 32 ����
#define GET_IR_BTT	message.get_data_dt.get_ir_reciver_data.IR_data.cmd_0	// ��� ������� ������ ������ �� 1 ����
#define GET_IR_ADDR	message.get_data_dt.get_ir_reciver_data.IR_data.addr_0	// ����� ������ �� 1 ����
#define SETUP_IR	message.set_setpoint_dt.set_ir_reciver_dt				// ������� IR ��������� ��
#define IREN		7				// ��������� ������ IR

// ������ ��������� (������������ �� ADC)
#define GET_LIGHTING	message.get_data_dt.get_lighting_data			// ������ ������� ��������� 1 ����
#define SETUP_LIGHTING	message.set_setpoint_dt.set_lighting_dt			// ������� ������� ���������
#define LTEN		7				// ��������� ������ ������� ���������
#define LTPS2		2				// �������� ������� ������ ������� ���������
#define LTPS1		1				// �������� ������� ������ ������� ���������
#define LTPS0		0				// �������� ������� ������ ������� ���������

// ������ ����������� (1-Wire DS18B20)
#define GET_TEMP	message.get_data_dt.get_temperature_data			// ������ ������� ����������� 2 �����
#define SETUP_TEMP	message.set_setpoint_dt.set_temperature_dt			// ������� ������� �����������
#define TPEN		7				// ��������� ������ ������� �����������
#define TPR1		6				// ����������� ����������� ������� �����������
#define TPR0		5				// ����������� ����������� ������� �����������
#define TPPS2		2				// �������� ������� ������ ������� �����������
#define TPPS1		1				// �������� ������� ������ ������� �����������
#define TPPS0		0				// �������� ������� ������ ������� �����������

// PIR ������ �������� (HC SR-501)
#define GET_PIR_DATA		message.get_data_dt.get_pir_motion_data		// ������ PIR ������� �������� 1 ����(������ 1 ���)
#define SETUP_PIR	message.set_setpoint_dt.set_pir_motion_dt			// ������� PIR ������� ��������
#define PIREN		7				// ��������� ������ PIR ������� ��������

// �� ������ ����������(HC SR04)
#define GET_US		message.get_data_dt.get_ultrasound_data				// ������ �� ������� ���������� 2 �����
#define SETUP_US	message.set_setpoint_dt.set_ultrasound_dt			// ������� �� ������� ����������
#define USEN		7				// ��������� ������ �� ������� ����������
#define USPS2		2				// �������� ������� ������ �� ������� ����������
#define USPS1		1				// �������� ������� ������ �� ������� ����������
#define USPS0		0				// �������� ������� ������ �� ������� ����������
#if NRF24L01_IS_USED==1
#if (NRF24L01_TX_RX==0)
volatile u_message message_unit[MAX_SENSOR_UNIT]; //  � ����� ����������� ������ MAX_SENSOR_UNIT
#else
volatile u_message message_unit[1]; //  � ����� ��������� 1 �����
#endif
#endif


//#define default_value 0x00

void set_message_default();

void set_all_message_default(unsigned char n);

void send_Uart_message2(unsigned char i);//   �������� ������ ����� ����

#endif /* MESSAGE_H_ */

