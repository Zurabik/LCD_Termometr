/*
 * TX_RX_command.h
 *
 * Created: 31.01.2015 12:43:23
 *  Author: Vadim
 */ 
/*typedef enum _command {
	ANSWER_ALL_DATA=0x10,
	SETUP_ALL_SETTING=0x10,
	
	
	
} COMMAND;
*/

#ifndef TX_RX_COMMAND_H_
#define TX_RX_COMMAND_H_
//#include "radio.h"
//#include "settings.h"
#include "Message.h"

#define ANSWER_ALL_MESSAGE			10

#define	SETUP_ALL_SETTING			50
#define	SETUP_DISCRET_SETTING		51
#define	SETUP_TEMP_SETTING			52
#define	SETUP_LIGHTING_SETTING		53
#define	SETUP_PIR_SETTING			54
#define	SETUP_USOUND_SETTING		55
#define	SETUP_IR_SETTING			56

#define	GET_ALL_DATA				100
#define	GET_STATUS_DATA				101
#define	GET_TEMP_DATA				102
#define	GET_LIGHTING_DATA			103
#define	GET_PIR_DATA				104
#define	GET_USOUND_DATA				105
#define	GET_IR_DATA					106

#define	GET_ALL_SETTING				150
#define	GET_TEMP_SETTING			151
#define	GET_LIGHTING_SETTING		152
#define	GET_PIR_SETTING				153
#define	GET_USOUND_SETTING			154
#define	GET_IR_SETTING				155


char* TX_to_RX_command(char* command, u_message* message__unit);


void to_RX_command();



#endif /* TX_RX_COMMAND_H_ */