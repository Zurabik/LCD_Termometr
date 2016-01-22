/*
 * TX_RX_command.c
 *
 * Created: 31.01.2015 15:03:28
 *  Author: Vadim
 */ 
#include "TX_RX_command.h"
#if (NRF24L01_IS_USED==1)
char* TX_to_RX_command (char* command, u_message* mes)
{
	
	switch (command[0])
	{
		//ответ со всеми данными
		case ANSWER_ALL_MESSAGE:
		send_Uart_str("ответ со всеми данными");
		return message_unit[0].ch_message;
		break;
		
		//установка всех уставок
		case SETUP_ALL_SETTING: 
		
		//printf("установка всех уставок. \n\r");
		//send_Uart_str("установка всех уставок");
		//printf(command);
		mes[0].CH_LAMP=command[1];
		mes[0].SETUP_TEMP=command[2];
		mes[0].SETUP_LIGHTING =command[3];
		mes[0].SETUP_PIR=command[4];
		mes[0].SETUP_US=command[5];
		mes[0].SETUP_IR=command[6];
		//return "vse ustavki";
		return message_unit[0].ch_message;
		//printf("Message:  %s", command);
		break;
		
		case 0x54:
		send_Uart_str("ВЕРНО");
		send_Uart_str(command);
		
		//snprintf((char*)packet.payload.message.messagecontent, sizeof(packet.payload.message.messagecontent), "Ответ Ответ Ответ Ответ 26");
		//send_instruction(W_ACK_PAYLOAD, &packet, NULL, sizeof(packet));
		break;
		
		default:
		send_Uart_str(command);
		//printf("Message:  %s", packet.payload.message.messagecontent);
		break;
	}
}

void to_RX_command()
{
	
}

#endif