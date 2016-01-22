/*
 * Temper.h
 *
 * Created: 20.08.2014 10:39:00
 *  Author: Vadim2
 */ 
#ifndef TEMPER_H_
#define TEMPER_H_

//подключение заголовочных файлов 1Wire библиотеки
#include "OWIPolled.h"
#include "OWIHighLevelFunctions.h"
#include "OWIBitFunctions.h"
#include "common_files\OWIcrc.h"
//#include "Message.h"

//команды датчика DS18B20
#define DS18B20_CONVERT_T                0x44
#define DS18B20_READ_SCRATCHPAD          0xbe

//номер вывода, к которому подключен датчик
#define BUS   OWI_PIN_3					//PC3
//количество устройств на 1-Wire
#define MAX_DEVICES       0x05

/*
typedef struct
{
	unsigned char temp0;
	unsigned char temp1;
} __BYTE_TEMP;

typedef union
{
	uint16_t TEMP_16;
	__BYTE_BITS TEMP;
} BYTE_BITS;
*/
void Init_Temperatura();
void Get_all_Temperatura();
void Set_Temp_to_LCD();
//uint16_t Get_byte_Temperatura();
void Get_temp(unsigned char *temp[]);
#endif /* TEMPER_H_ */
