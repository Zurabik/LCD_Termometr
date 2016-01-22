/*
 * DS3231.h
 *
 * Created: 01.08.2015 21:38:30
 *  Author: Vadim2
 */ 


#ifndef DS3231_H_
#define DS3231_H_


#include "twim.h"

#define DS_REG_SECONDS		0x00
#define DS_REG_MINUTES		0x01
#define DS_REG_HOURS		0x02
#define DS_REG_DAY			0x03
#define DS_REG_DATE			0x04
#define DS_REG_MONTH		0x05
#define DS_REG_YEAR			0x06

#define DS_REG_A1SEC		0x07
#define DS_REG_A1MIN		0x08
#define DS_REG_A1HOUR		0x09
#define DS_REG_A1DYDT		0x0A

#define DS_REG_A2MIN		0x0B
#define DS_REG_A2HOUR		0x0C
#define DS_REG_A2DYDT		0x0D

#define DS_REG_CONTROL		0x0E
	#define DS_BIT_A1IE		0
	#define DS_BIT_A2IE		1
	#define DS_BIT_INTCN	2
	#define DS_BIT_RS1		3
	#define DS_BIT_RS2		4
	#define DS_BIT_CONV		5
	#define DS_BIT_BBSQW	6
	#define DS_BIT_EOSC		7	
		
#define DS_REG_STATUS		0x0F
	#define DS_BIT_A1F		0
	#define DS_BIT_A2F		1
	#define DS_BIT_BSY		2
	#define DS_BIT_EN32KHZ	3
	#define DS_BIT_OSF		7
	
#define DS_REG_AGING		0x10
#define DS_REG_MSB_TEMP		0x11
#define DS_REG_LSB_TEMP		0x12

#define DS_BIT_AMPM			5
#define DS_BIT_1224			6
#define DS_BIT_CENTURY		7
#define DS_BIT_A1M1			7
#define DS_BIT_A1M2			7
#define DS_BIT_A1M3			7
#define DS_BIT_A1M4			7
#define DS_BIT_SIGN			7


#define ASCII_NUL     '0'
#define DS1307_ADR    104
#define SIZE_UINT8    3
/* структура секунд
struct ds3231_00h_seconds					
{
	unsigned seconds_1	: 4;
	unsigned seconds_10	: 3;
	unsigned out_8 : 1;

};
*/
/* структура минут
struct ds3231_01h_minutes
{
	unsigned minutes_1	: 4;
	unsigned minutes_10	: 3;
	unsigned res : 1;

};
*/
/* структура часов
struct ds3231_02h_hours
{
	unsigned hour_1			: 4;
	unsigned hour_10		: 1;
	unsigned AM_PM_hour_20	: 1;
	unsigned hour_12_24		: 1;
	unsigned res			: 1;
};
*/
/* структура Time*/
struct ds3231_time
{
	struct ds3231_00h_seconds
	{
		unsigned seconds_1	: 4;
		unsigned seconds_10	: 3;
		unsigned out_8 : 1;

	} seconds;
	struct ds3231_01h_minutes
	{
		unsigned minutes_1	: 4;
		unsigned minutes_10	: 3;
		unsigned res : 1;

	}minutes;
	struct ds3231_02h_hours
	{
		unsigned hour_1			: 4;
		unsigned hour_10		: 1;
		unsigned AM_PM_hour_20	: 1;
		unsigned hour_12_24		: 1;
		unsigned res			: 1;
	}hours;
};

/* структура дни недели
struct ds3231_03h_day
{
	unsigned day			: 3;
	unsigned res			: 5;
};
*/
/* структура дни месяца
struct ds3231_04h_date
{
	unsigned date_1			: 4;
	unsigned date_10		: 2;
	unsigned res			: 2;
};
*/
/* структура месяцы
struct ds3231_05h_month
{
	unsigned month			: 4;
	unsigned month_10		: 1;
	unsigned res			: 2;
	unsigned century		: 1;
	
};
*/
/* структура годы
struct ds3231_06h_year
{
	unsigned year			: 4;
	unsigned year_10		: 4;
};
*/
/* структура Date*/
struct ds3231_date
{
	struct ds3231_03h_day
	{
		unsigned day			: 3;
		unsigned res			: 5;
	} day;
	struct ds3231_04h_date
	{
		unsigned date_1			: 4;
		unsigned date_10		: 2;
		unsigned res			: 2;
	} date;
	struct ds3231_05h_month
	{
		unsigned month			: 4;
		unsigned month_10		: 1;
		unsigned res			: 2;
		unsigned century		: 1;
		
	} month;
	struct ds3231_06h_year
	{
		unsigned year			: 4;
		unsigned year_10		: 4;
	} year;
};

/* структура секунды Будильник№1
struct ds3231_07h_a1_seconds
{
	unsigned seconds_1		: 4;
	unsigned seconds_10		: 3;
	unsigned a1m1			: 1;
};
*/
/* структура минуты Будильник№1
struct ds3231_08h_a1_minutes
{
	unsigned minutes_1		: 4;
	unsigned minutes_10		: 3;
	unsigned a1m2			: 1;
};
*/
/* структура часы Будильник№1
struct ds3231_09h_a1_hours
{
	unsigned hour_1			: 4;
	unsigned hour_10		: 1;
	unsigned AM_PM_hour_20	: 1;
	unsigned hour_12_24		: 1;
	unsigned a1m3			: 1;
};
*/
/* структура дни Будильник№1
struct ds3231_0Ah_a1_date
{
	unsigned date_1			: 4;
	unsigned date_10		: 2;
	unsigned dydt			: 1;
	unsigned a1m4			: 1;
};
*/
/* структура Будильник№1*/
struct ds3231_alarm1
{
	struct ds3231_07h_a1_seconds
	{
		unsigned seconds_1		: 4;
		unsigned seconds_10		: 3;
		unsigned a1m1			: 1;
	} seconds;
	struct ds3231_08h_a1_minutes
	{
		unsigned minutes_1		: 4;
		unsigned minutes_10		: 3;
		unsigned a1m2			: 1;
	} minutes;
	struct ds3231_09h_a1_hours
	{
		unsigned hour_1			: 4;
		unsigned hour_10		: 1;
		unsigned AM_PM_hour_20	: 1;
		unsigned hour_12_24		: 1;
		unsigned a1m3			: 1;
	} hours;
	struct ds3231_0Ah_a1_date
	{
		unsigned date_1			: 4;
		unsigned date_10		: 2;
		unsigned dydt			: 1;
		unsigned a1m4			: 1;
	} date;
};


/* структура минуты Будильник№2
struct ds3231_0Bh_a2_minutes
{
	unsigned minutes_1		: 4;
	unsigned minutes_10		: 3;
	unsigned a2m2			: 1;
};
*/
/* структура часы Будильник№2
struct ds3231_0Ch_a2_hours
{
	unsigned hour_1			: 4;
	unsigned hour_10		: 1;
	unsigned AM_PM_hour_20	: 1;
	unsigned hour_12_24		: 1;
	unsigned a2m3			: 1;
};
*/
/* структура дни Будильник№2
struct ds3231_0Dh_a2_date
{
	unsigned date_1			: 4;
	unsigned date_10		: 2;
	unsigned dydt			: 1;
	unsigned a2m4			: 1;
};
*/
/* структура Будильник№2
struct ds3231_alarm2
{
	struct ds3231_0Bh_a2_minutes
	{
		unsigned minutes_1		: 4;
		unsigned minutes_10		: 3;
		unsigned a2m2			: 1;
	} minutes;
	struct ds3231_0Ch_a2_hours
	{
		unsigned hour_1			: 4;
		unsigned hour_10		: 1;
		unsigned AM_PM_hour_20	: 1;
		unsigned hour_12_24		: 1;
		unsigned a2m3			: 1;
	} hours;
	struct ds3231_0Dh_a2_date
	{
		unsigned date_1			: 4;
		unsigned date_10		: 2;
		unsigned dydt			: 1;
		unsigned a2m4			: 1;
	} date;
};
*/
/* структура Control
struct ds3231_0Eh_control
{
	unsigned a1ie			: 1;
	unsigned a2ie			: 1;
	unsigned intcn			: 1;
	unsigned rs1			: 1;
	unsigned rs2			: 1;
	unsigned conv			: 1;
	unsigned bbsqw			: 1;
	unsigned eosc			: 1;
} ;
*/
/* структура Status
struct ds3231_0Fh_status
{
	unsigned a1f			: 1;
	unsigned a2f			: 1;
	unsigned bsy			: 1;
	unsigned en32khz		: 1;
	unsigned res			: 3;
	unsigned osf			: 1;
} ;
*/
/* структура DS3231*/
struct ds3231_registers
{
	struct ds3231_time time;
	struct ds3231_date date;
	struct ds3231_alarm1 alarm1;
	struct ds3231_alarm2
	{
		struct ds3231_0Bh_a2_minutes
		{
			unsigned minutes_1		: 4;
			unsigned minutes_10		: 3;
			unsigned a2m2			: 1;
		} minutes;
		struct ds3231_0Ch_a2_hours
		{
			unsigned hour_1			: 4;
			unsigned hour_10		: 1;
			unsigned AM_PM_hour_20	: 1;
			unsigned hour_12_24		: 1;
			unsigned a2m3			: 1;
		} hours;
		struct ds3231_0Dh_a2_date
		{
			unsigned date_1			: 4;
			unsigned date_10		: 2;
			unsigned dydt			: 1;
			unsigned a2m4			: 1;
		} date;
	} alarm2;
	struct ds3231_0Eh_control
	{
		unsigned a1ie			: 1;
		unsigned a2ie			: 1;
		unsigned intcn			: 1;
		unsigned rs1			: 1;
		unsigned rs2			: 1;
		unsigned conv			: 1;
		unsigned bbsqw			: 1;
		unsigned eosc			: 1;
	} control;
	struct ds3231_0Fh_status
	{
		unsigned a1f			: 1;
		unsigned a2f			: 1;
		unsigned bsy			: 1;
		unsigned en32khz		: 1;
		unsigned res			: 3;
		unsigned osf			: 1;
	} status;
	unsigned char aging_offset;
	unsigned char MSB_of_temp;
	unsigned char LSB_of_temp;
} ;

union u_ds3231_data
{
	struct ds3231_registers ds3231_data;
	unsigned char ch_ds3231_data[sizeof(struct ds3231_registers)];
};


struct st_uc_date_time
{
	uint8_t uc_sec;
	uint8_t uc_min;
	uint8_t uc_hour;
	uint8_t uc_day;
	uint8_t uc_date;
	uint8_t uc_month;
	uint16_t uc_year;
};

union u_uc_date_time
{
	struct st_uc_date_time uc_dt;
	unsigned char ch_uc_dt[sizeof(struct st_uc_date_time)];
};


volatile union u_ds3231_data ds3231;
volatile union u_uc_date_time uc_dt;


uint8_t buf[18];
uint8_t sec, min, hour;

ISR (INT0_vect)
{
	//сбросим флаг прерывания в мк
	EIFR=(1<<INTF0);
	//nop();
	//запретим прерывание (int0 по умолчанию, при низком уровне на PD2)
	EIMSK &= ~_BV(INT0);
	
}

void DS3231_time_to_LCD(union u_ds3231_data *str_ds3231)
{
	#if DS3231_IS_USED==1						
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	//char str3[14];
	sec=(str_ds3231->ds3231_data.time.seconds.seconds_1+(str_ds3231->ds3231_data.time.seconds.seconds_10*10));
	min=(str_ds3231->ds3231_data.time.minutes.minutes_1+(str_ds3231->ds3231_data.time.minutes.minutes_10*10));
	if (str_ds3231->ds3231_data.time.hours.AM_PM_hour_20==1)
	{
		hour=(str_ds3231->ds3231_data.time.hours.hour_1+20);
	} 
	else
	{
		hour=(str_ds3231->ds3231_data.time.hours.hour_1+(str_ds3231->ds3231_data.time.hours.hour_10*10));
	}
	
	sprintf(str2, "Время:%02i:%02i:%02i", hour, min, sec);//укажем текущее среднее значение АЦП
	LcdGotoXYFont(0,4);
	LcdStr(FONT_1X, &str2);
	#endif
}

void DS3231_Set_Time(union u_ds3231_data *str_ds3231,uint8_t sec, uint8_t min, uint8_t hour)
{
	
	str_ds3231->ds3231_data.time.seconds.seconds_10=sec/10;
	str_ds3231->ds3231_data.time.seconds.seconds_1=sec%10;
	str_ds3231->ds3231_data.time.minutes.minutes_10=min/10;
	str_ds3231->ds3231_data.time.minutes.minutes_1=min%10;
	str_ds3231->ds3231_data.time.hours.hour_10 =hour/10;
	str_ds3231->ds3231_data.time.hours.hour_1=hour%10;	
 /*устанавливаем указатель DS1307 на нужный адрес*/
		buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
		buf[1] = DS_REG_SECONDS;        //адрес начального регистра   
		buf[2] = str_ds3231->ch_ds3231_data[DS_REG_SECONDS];	//значение регистра
		buf[3] = str_ds3231->ch_ds3231_data[DS_REG_MINUTES];	//значение регистра
		buf[4] = str_ds3231->ch_ds3231_data[DS_REG_HOURS];		//значение регистра
		TWI_SendData(buf, 5);		//запись 
}

void DS3231_Set_Alarm1(union u_ds3231_data *str_ds3231, uint8_t sec)
{
	uint8_t sec_now;
	uint8_t i;
	uint8_t sec_alarm;
	if (sec<=59)
	{
		sec_now=str_ds3231->ds3231_data.time.seconds.seconds_10*10+str_ds3231->ds3231_data.time.seconds.seconds_1;
		if (sec_now+sec>59)//если наша уставка выходит за границы текущей минуты
		{
			sec_alarm=sec_now+sec-60;
		} 
		else
		{
			sec_alarm=sec_now+sec;
		}
		str_ds3231->ds3231_data.alarm1.seconds.seconds_10=sec_alarm/10;
		str_ds3231->ds3231_data.alarm1.seconds.seconds_1=sec_alarm%10;
		str_ds3231->ds3231_data.alarm1.seconds.a1m1=0;
		str_ds3231->ds3231_data.alarm1.minutes.a1m2=1;
		str_ds3231->ds3231_data.alarm1.hours.a1m3=1;
		str_ds3231->ds3231_data.alarm1.date.a1m4=1;
		str_ds3231->ds3231_data.control.a1ie=1;
		//str_ds3231->ds3231_data.control.a2ie=0;
		str_ds3231->ds3231_data.control.intcn=1;
		
		/*устанавливаем указатель DS1307 на нужный адрес*/
		buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
		buf[1] = DS_REG_A1SEC;        //адрес начального регистра
		 for(i = 0; i < 8; i++){  //то переписываем его из внутреннего буфера в переданный
			 buf[i+2] = str_ds3231->ch_ds3231_data[i+DS_REG_A1SEC];
		 }		
		TWI_SendData(buf, 10);		//запись
		
	}
	/* */
	/*
		DS3231_Set_One_Register(128,DS_REG_A1SEC);//установим будильник на каждую секунду
		DS3231_Set_One_Register(128,DS_REG_A1MIN);
		DS3231_Set_One_Register(128,DS_REG_A1HOUR);
		DS3231_Set_One_Register(128,DS_REG_A1DYDT);
		
		DS3231_Set_One_Register(31,DS_REG_CONTROL);
		DS3231_Set_One_Register(128,DS_REG_STATUS);
		*/
	
	
}

void DS3231_Set_Alarm1_unknow(union u_ds3231_data *str_ds3231, uint8_t sec)
{

	uint8_t sec_now;
	uint8_t i;
	uint8_t sec_alarm;
	if (sec<=59)
	{
		//вычислим текущее время
		sec_now=str_ds3231->ds3231_data.alarm1.seconds.seconds_10*10+str_ds3231->ds3231_data.alarm1.seconds.seconds_1;
		if (sec_now+sec>59)//если наша уставка выходит за границы текущей минуты
		{
			sec_alarm=sec_now+sec-60;
		} 
		else
		{
			sec_alarm=sec_now+sec;
		}
		str_ds3231->ds3231_data.alarm1.seconds.seconds_10=sec_alarm/10;
		str_ds3231->ds3231_data.alarm1.seconds.seconds_1=sec_alarm%10;
		str_ds3231->ds3231_data.alarm1.seconds.a1m1=0;
		str_ds3231->ds3231_data.alarm1.minutes.a1m2=1;
		str_ds3231->ds3231_data.alarm1.hours.a1m3=1;
		str_ds3231->ds3231_data.alarm1.date.a1m4=1;
		str_ds3231->ds3231_data.control.a1ie=1;
		//str_ds3231->ds3231_data.control.a2ie=0;
		str_ds3231->ds3231_data.control.intcn=1;
		
		/*устанавливаем указатель DS1307 на нужный адрес*/
		buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
		buf[1] = DS_REG_A1SEC;        //адрес начального регистра
		 for(i = 0; i < 8; i++){  //то переписываем его из внутреннего буфера в переданный
			 buf[i+2] = str_ds3231->ch_ds3231_data[i+DS_REG_A1SEC];
		 }		
		TWI_SendData(buf, 10);		//запись
		
	}
	/* */
	/*
		DS3231_Set_One_Register(128,DS_REG_A1SEC);//установим будильник на каждую секунду
		DS3231_Set_One_Register(128,DS_REG_A1MIN);
		DS3231_Set_One_Register(128,DS_REG_A1HOUR);
		DS3231_Set_One_Register(128,DS_REG_A1DYDT);
		
		DS3231_Set_One_Register(31,DS_REG_CONTROL);
		DS3231_Set_One_Register(128,DS_REG_STATUS);
		*/
	
	
}


// void DS3231_Set_All_registers(u_ds3231_data *str_ds3231)
// {
// 	memcpy(buf,str_ds3231,16);//копируем все нужные данные
// 
// 	/*устанавливаем указатель DS1307 на нужный адрес*/
// 	buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
// 	buf[1] = DS_REG_SECONDS;        //адрес начального регистра
// 	TWI_SendData(buf, 16);		//запись
// }


void DS3231_Get_All_reg_before()
{
	/*устанавливаем указатель DS1307 
       на нулевой адрес*/
      buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
      buf[1] = 0;                 //адрес регистра   

      TWI_SendData(buf, 2);
   
      /*считываем время с DS1307*/
      buf[0] = (DS1307_ADR<<1)|1;
      TWI_SendData(buf, 18);
}

void DS3231_Get_All_reg_after(union u_ds3231_data *str_ds3231)
{
	uint8_t i;
	/*переписываем данные буфера драйвера в свой буфер*/
	TWI_GetData(buf, 16);//то переписываем его из внутреннего буфера в переданный
	for(i = 0; i < 16; i++)
	{  
		str_ds3231->ch_ds3231_data[i] = buf[i+1];
	}
	  //memcpy(str_ds3231->ch_ds3231_data, buf, 2);
	  
	  //sprintf(str2, "Время:%02i", (str_ds3231->ch_ds3231_data[0]));//укажем текущее среднее значение АЦП
	  
	  
}


void DS3231_Set_One_Register(uint8_t *value, uint8_t reg_number)
{
 /*устанавливаем указатель DS1307 
       на нужный адрес*/
		buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
		buf[1] = reg_number;        //адрес регистра   
		buf[2] = value;	//значение регистра  
		TWI_SendData(buf, 3);		//запись 
   
      /*считываем время с DS1307*/
    //  buf[0] = (DS1307_ADR<<1)|1;
     // TWI_SendData(buf, 5);
     
      /*переписываем данные буфера 
      драйвера в свой буфер*/
/*       TWI_GetData(buf, 5);
      
      sec  = buf[1];
      min  = buf[2];
      hour = buf[3];
       
      /*выводим время на LCD*/
//       LCD_Goto(0, 1);
//       LCD_WriteData((hour>>4)  + ASCII_NUL);
//       LCD_WriteData((hour&0xf) + ASCII_NUL);    
//       LCD_WriteData(':');
//       LCD_WriteData((min>>4)  + ASCII_NUL);
//       LCD_WriteData((min&0xf) + ASCII_NUL);    
//       LCD_WriteData(':');    
//       LCD_WriteData((sec>>4)  + ASCII_NUL);
//       LCD_WriteData((sec&0xf) + ASCII_NUL); 
   
/*   
   sprintf(str2, "%1u%1u:%1u%1u", ((hour>>4)  + ASCII_NUL), ((hour&0xf) + ASCII_NUL), ((min>>4)  + ASCII_NUL) ,((min&0xf) + ASCII_NUL));//укажем текущее среднее значение АЦП
   LcdGotoXYFont(0,4);
   LcdStr(FONT_1X, &str2);
   */
}
    
void DS3231_Get_Time_before()
{
 /*устанавливаем указатель DS1307 
       на нулевой адрес*/
      buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
      buf[1] = 0;                 //адрес регистра   

      TWI_SendData(buf, 2);
   
      /*считываем время с DS1307*/
      buf[0] = (DS1307_ADR<<1)|1;
      TWI_SendData(buf, 5);
     
      /*переписываем данные буфера 
      драйвера в свой буфер*/
/*       TWI_GetData(buf, 5);
      
      sec  = buf[1];
      min  = buf[2];
      hour = buf[3];
       
      /*выводим время на LCD*/
//       LCD_Goto(0, 1);
//       LCD_WriteData((hour>>4)  + ASCII_NUL);
//       LCD_WriteData((hour&0xf) + ASCII_NUL);    
//       LCD_WriteData(':');
//       LCD_WriteData((min>>4)  + ASCII_NUL);
//       LCD_WriteData((min&0xf) + ASCII_NUL);    
//       LCD_WriteData(':');    
//       LCD_WriteData((sec>>4)  + ASCII_NUL);
//       LCD_WriteData((sec&0xf) + ASCII_NUL); 
   
/*   
   sprintf(str2, "%1u%1u:%1u%1u", ((hour>>4)  + ASCII_NUL), ((hour&0xf) + ASCII_NUL), ((min>>4)  + ASCII_NUL) ,((min&0xf) + ASCII_NUL));//укажем текущее среднее значение АЦП
   LcdGotoXYFont(0,4);
   LcdStr(FONT_1X, &str2);
   */
}	
	
void DS3231_Get_Time_after()
{
 /*устанавливаем указатель DS1307 
       на нулевой адрес*/
      buf[0] = (DS1307_ADR<<1)|0; //адресный пакет
      buf[1] = 0;                 //адрес регистра   

//      TWI_SendData(buf, 2);
   
      /*считываем время с DS1307*/
//      buf[0] = (DS1307_ADR<<1)|1;
//      TWI_SendData(buf, 5);
      
      /*переписываем данные буфера 
      драйвера в свой буфер*/
      TWI_GetData(buf, 5);
      
      sec  = buf[1];
      min  = buf[2];
      hour = buf[3];
       
      /*выводим время на LCD*/
//       LCD_Goto(0, 1);
//       LCD_WriteData((hour>>4)  + ASCII_NUL);
//       LCD_WriteData((hour&0xf) + ASCII_NUL);    
//       LCD_WriteData(':');
//       LCD_WriteData((min>>4)  + ASCII_NUL);
//       LCD_WriteData((min&0xf) + ASCII_NUL);    
//       LCD_WriteData(':');    
//       LCD_WriteData((sec>>4)  + ASCII_NUL);
//       LCD_WriteData((sec&0xf) + ASCII_NUL); 
   
   
   sprintf(str2, "Время:%01i%01i:%01i%01i:%01i%01i", (hour>>4),(hour&0xf),(min>>4),(min&0xf),(sec>>4),(sec&0xf));//укажем текущее среднее значение АЦП
   LcdGotoXYFont(0,4);
   LcdStr(FONT_1X, &str2);
}
#endif /* DS3231_H_ */