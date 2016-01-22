/*
 * Message.c
 *
 * Created: 31.01.2015 15:09:09
 *  Author: Vadim
 */ 

#include "Message.h"

#if NRF24L01_IS_USED==1
#if (NRF24L01_TX_RX==0)
volatile u_message message_unit[MAX_SENSOR_UNIT]; //  в режим передатчика массив MAX_SENSOR_UNIT
#endif
#if (NRF24L01_TX_RX==1)
volatile u_message message_unit[1]; //  в режим приемника 1 штука
#endif
volatile u_message message_unit[1]; //  в режим приемника 1 штука

#define default_value 0x00

void set_message_default()
{
	//default_value=0x00;
	message_unit[0].message.status_dt.ch_status=default_value;
	message_unit[0].SENSOR_ADDRESS=NUMB_SENSOR_UNIT*10;
	// Выключаем свет
	//message_unit[i].SETUP_LAMP=(0<<SL12_1)|(0<<SL12_2)|(0<<SL12_3)|(0<<SL12_4)|(0<<SL220_1)|(0<<SL220_2)|(0<<SL220_3)|(0<<SL220_4);
	message_unit[0].SETUP_LAMP=default_value;
	// Выключаем IR приемник
	message_unit[0].SETUP_IR=(0<<IREN);
	message_unit[0].GET_IR_DATA_i32=default_value;
	//message_unit[i].GET_IR[1]=default_value;
	//message_unit[i].GET_IR[2]=default_value;
	//message_unit[i].GET_IR[3]=default_value;
	// Выключаем датчика освещения, делитель 256
	message_unit[0].SETUP_LIGHTING=(0<<LTEN)|(1<<LTPS2)|(1<<LTPS1)|(1<<LTPS0);
	message_unit[0].GET_LIGHTING=default_value;
	// Выключаем датчик температуры, 12bit, делитель (256)???
	message_unit[0].SETUP_TEMP=(0<<TPEN)|(1<<TPR1)|(1<<TPR0)|(1<<TPPS2)|(1<<TPPS1)|(1<<TPPS0);
	message_unit[0].GET_TEMP[0]=default_value;
	message_unit[0].GET_TEMP[1]=default_value;
	// Выключаем PIR датчика движения
	message_unit[0].SETUP_PIR=(0<<PIREN);
	message_unit[0].GET_PIR_DATA=default_value;
	// Выключаем УЗ датчика расстояния, делитель (256)???
	message_unit[0].SETUP_US=(0<<USEN)|(1<<USPS2)|(1<<USPS1)|(1<<USPS0);
	message_unit[0].GET_US=default_value;
	//send_Uart(default_value);
	//puts(message_unit[i]);
	//send_Uart_str(message_unit[i]);
	//send_Uart_message(message_unit[i]);

}
void set_all_message_default(unsigned char n)
{
	uint8_t i;
	for (i=0;i<n;i++)
	{
	//default_value=0x00;
	message_unit[i].message.status_dt.ch_status=default_value;
	message_unit[i].SENSOR_ADDRESS=i*10;
	// Выключаем свет
	//message_unit[i].SETUP_LAMP=(0<<SL12_1)|(0<<SL12_2)|(0<<SL12_3)|(0<<SL12_4)|(0<<SL220_1)|(0<<SL220_2)|(0<<SL220_3)|(0<<SL220_4);
	message_unit[i].SETUP_LAMP=default_value;
	// Выключаем IR приемник
	message_unit[i].SETUP_IR=(0<<IREN);
	message_unit[i].GET_IR_DATA_i32=default_value;
	//message_unit[i].GET_IR[1]=default_value;
	//message_unit[i].GET_IR[2]=default_value;
	//message_unit[i].GET_IR[3]=default_value;
	// Выключаем датчика освещения, делитель 256
	message_unit[i].SETUP_LIGHTING=(0<<LTEN)|(1<<LTPS2)|(1<<LTPS1)|(1<<LTPS0);
	message_unit[i].GET_LIGHTING=default_value;
	// Выключаем датчик температуры, 12bit, делитель (256)???
	message_unit[0].SETUP_TEMP=(0<<TPEN)|(1<<TPR1)|(1<<TPR0)|(1<<TPPS2)|(1<<TPPS1)|(1<<TPPS0);
	message_unit[i].GET_TEMP[0]=default_value;
	message_unit[i].GET_TEMP[1]=default_value;
	// Выключаем PIR датчика движения
	message_unit[i].SETUP_PIR=(0<<PIREN);
	message_unit[i].GET_PIR_DATA=default_value;
	// Выключаем УЗ датчика расстояния, делитель (256)???
	message_unit[i].SETUP_US=(0<<USEN)|(1<<USPS2)|(1<<USPS1)|(1<<USPS0);
	message_unit[i].GET_US=default_value;
	//send_Uart(default_value);
	//puts(message_unit[i]);
	//send_Uart_str(message_unit[i]);
	//send_Uart_message(message_unit[i]);
	}
	/*
	char output[128];
	char buffer[50];
//	snprintf(buffer, sizeof(buffer), "%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X \n\r",
	snprintf(buffer, sizeof(buffer), 
	message_unit[i].SENSOR_ADDRESS,
	message_unit[i].SETUP_LAMP,
	message_unit[i].SETUP_IR,
	message_unit[i].GET_IR[0],
	message_unit[i].GET_IR[1],
	message_unit[i].GET_IR[2],
	message_unit[i].GET_IR[3],
	message_unit[i].SETUP_LIGHTING,
	message_unit[i].GET_LIGHTING,
	message_unit[i].SETUP_TEMP,
	message_unit[i].GET_TEMP[0],
	message_unit[i].GET_TEMP[1],
	message_unit[i].SETUP_PIR,
	message_unit[i].GET_PIR,
	message_unit[i].SETUP_US,
	message_unit[i].GET_US);
	//snprintf(output, sizeof(output), message_unit[i]);
	//printf("Data not trasmitted. Max retry. \n\r");
	printf(buffer);
	*/
}

/*
void send_message(unsigned char i)
{
	putchar(message_unit[i].SENSOR_ADDRESS);
	putchar(message_unit[i].SETUP_LAMP);
	putchar(message_unit[i].SETUP_IR);
	putchar(message_unit[i].GET_IR[0]);
	putchar(message_unit[i].GET_IR[1]);
	putchar(message_unit[i].GET_IR[2]);
	putchar(message_unit[i].GET_IR[3]);
	putchar(message_unit[i].SETUP_LIGHTING);
	putchar(message_unit[i].GET_LIGHTING);
	putchar(message_unit[i].SETUP_TEMP);
	putchar(message_unit[i].GET_TEMP[0]);
	putchar(message_unit[i].GET_TEMP[1]);
	putchar(message_unit[i].SETUP_PIR);
	putchar(message_unit[i].GET_PIR);
	putchar(message_unit[i].SETUP_US);
	putchar(message_unit[i].GET_US);
}
*/
void send_Uart_message2(unsigned char i)//   Отправка пакета через уарт
{
	uint8_t i2;
	for (i2 = 0; i2 < sizeof(message_unit[i].ch_message); i2++) 
	{
		putchar( message_unit[i].ch_message[i2]);
	}

}

#endif