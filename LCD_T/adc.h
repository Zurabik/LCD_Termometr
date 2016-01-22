/*
 * adc.h
 *
 * Created: 21.08.2014 8:44:00
 *  Author: Vadim
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "adc_avr.h"
#include "settings.h"

volatile uint32_t load_number_i=0;//������� ���������� �������������� ���
//volatile char adc_res;//��������� ���
char str2[14];//����� ��� ������ � 14 ��������
uint16_t adc_bat_res_i;//����� ����������� ��� 
uint32_t adc_bat_res_i32;//����� ����������� ��� 
uint16_t adc_light_res_i;//����� ����������� ���
uint16_t adc_light_res_mV;//����� ����������� ���
//float  adc_bat_res_i;//����� ����������� ��� 
char i=0;
//#define ADC_DDR DDRC    // DDR of SPI port
//#define ADC_PORT PORTC  // SPI port
//#define ADC PORTC3 		// MOSI pin (Master out, Slave in)

//#define ADC_BAT_RESULT_mV ((adc_bat_res_i/ADC_CYCLES_NUMBER)*48) //������� ��������� ��� � ���� ���������� � ��
#define ADC_LIGHT_RESULT (adc_light_res_i/ADC_CYCLES_NUMBER) //������� ��������� ���

#define LOAD_NUMBER_DAY (load_number_i/10800) //
//uint16_t load_number_day_i=0;//����� ����������� ���
#define LOAD_NUMBER_HOUR ((load_number_i/450)-LOAD_NUMBER_DAY*24) //
//uint8_t load_number_hour_i=0;//����� ����������� ���
#define LOAD_NUMBER_MINUT (((load_number_i*2)/15)-LOAD_NUMBER_HOUR*60-LOAD_NUMBER_DAY*1440) //
//uint8_t load_number_minut_i=0;//����� ����������� ���
//������� �������������
void ADC_init()
{
#if MODE_OF_OPERATION==2 //������� ������ �1	
//������������� ���������� ���������, ������������� ����� 
         //ADMUX = 0b00000000;
         //ADCSRA = 0x8D;
		 //�������� ��� AREF - (0<<REFS1)|(0<<REFS0) 2,56 �
		 //���� ADLAR = 1, �� ��������� �������������� ����� ����� ������������� ������, � ��������� ������ - ��������������
		 //����� ADC - ADC2 (0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0)
		 ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0);
		//��������� ��������������, �� ��������� ��� ��� ���������� �� ���������� � ������������� 4
		 ADCSRA =(1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
#endif	

#if MODE_OF_OPERATION==4 //������� ������ �2    
//��� �������� ������� �2
//�������� ��� AVCC - (0<<REFS1)|(1<<REFS0) 3.3 V
//���� ADLAR = 1, �� ��������� �������������� ����� ����� ������������� ������, � ��������� ������ - ��������������
//����� ADC - ADC2 (0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0)
ADMUX = (0<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0);
//��������� ��������������, �� ��������� ���, ��� ���������� �� ���������� � ������������� 4
ADCSRA =(1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
#endif
	 
}

 
void ADC_V_batt()
{  //��� ���������� �������
	/*
#if 0
	
#if MODE_OF_OPERATION==2 //������� ������ �1	
KEY_V_BAT_ON();//������ ������� �� ����������� �������� �� ����� �������������
	//����� ADC - ADC2 (0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0)
	ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0);
	
	
	adc_bat_res_i=0;//������� ���������� ��������� ���
	for (i=1;i<=ADC_CYCLES_NUMBER;i++)
	{
		
		ADCSRA =(1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);//�������� �������������� (ADSC = 1)
		while((ADCSRA & 0x10)==0);//����, ���� ��� �������� �������������� (ADIF = 0)
		ADCSRA=(1<<ADIF);//������������� ADIF
		adc_bat_res_i=adc_bat_res_i+ADCH;
		ADCSRA =(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE); //��������� ADC �� ��� ���� �� �����
	}
	
	
KEY_V_BAT_OFF();	//������� ������� � ��������
	
	KEY_LIGHT_ON();//������ ������� �� ����������� �������� �� ����� �������������

	//adc_i++;//�������� ������� ���������� �������������� ���
	adc_light_res_i=0;//������� ���������� ��������� ���
	for (i=1;i<=ADC_CYCLES_NUMBER;i++)
	{
		ADMUX = (0<<REFS1)|(0<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0); //ADC6 
		ADCSRA =(1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);//�������� �������������� (ADSC = 1)
		while((ADCSRA & 0x10)==0);//����, ���� ��� �������� �������������� (ADIF = 0)
		ADCSRA=(1<<ADIF);//������������� ADIF
		adc_light_res_i=adc_light_res_i+ADCH;
		ADCSRA =(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE); //��������� ADC �� ��� ���� �� �����
	}
	
	KEY_LIGHT_OFF();	//������� ������� � ��������
	
	#if LCD_IS_USED==1
		#if LCD_TYPE==1
			if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN)//���� ���������� �������� ���������� ������ �� �������, ���������� ��
			{
				
				LcdSingleBar(0,39,7,83,PIXEL_OFF);//������� �������
				for (i=0;i<=7;i++)//�������� �����
				{
					if (i<6)
					{
						LcdSingleBar((i*20),39,7,1,PIXEL_ON);
					}
					if (i<5)
					{
						LcdSingleBar((10+i*20),38,5,1,PIXEL_ON);
					}
					
				}
				
				LcdSingleBar(0,37,3,((adc_bat_res_i/ADC_CYCLES_NUMBER)-41),PIXEL_ON);//�������� �������
// 				/
// 				if (((load_number_i*8)/60)<=60)
// 				{
// 					sprintf(str2, "���:%5lu�����", (load_number_i*8)/60);//������ ���������� ������ ���
// 				}
// 				if ((((load_number_i*8)/60)>60) && (((load_number_i*8)/60)<=(60*24)))
// 				{
// 					sprintf(str2, "���:%5lu�����", (load_number_i*8)/3600);//������ ���������� ������ ���
// 				}
// 				if (((load_number_i*8)/60)>(60*24) )
// 				{
// 					sprintf(str2, "���:%6lu����", (load_number_i*8)/86400);//������ ���������� ������ ���
// 				}
// 				
// 				if (ADC_BAT_RESULT_mV>V_1WIRE_MIN)//���� ���������� �������� �������� ����������� � ������� 
// 				{
// 					
				//	sprintf(str2, "    ��� ������");//������ ���������� ������ ���
				//	LcdGotoXYFont(0,2);
				//	LcdStr(FONT_1X, &str2);
				}

				sprintf(str2, "���:%04lu:%02lu:%02lu", LOAD_NUMBER_DAY, LOAD_NUMBER_HOUR, LOAD_NUMBER_MINUT);//������ ���������� ������ ���
				LcdGotoXYFont(0,3);
				LcdStr(FONT_1X, &str2);
				sprintf(str2, "L:%3u V:%4u��", (adc_light_res_i/ADC_CYCLES_NUMBER),((adc_bat_res_i/ADC_CYCLES_NUMBER)*49));//������ ������� ������� �������� ���
				LcdGotoXYFont(0,5);			
				LcdStr(FONT_1X, &str2);
			}
		#endif
		#if LCD_TYPE==0
			sprintf(str2, "���� ��� %5u", load_number_i);//������ ���������� ������ ���
			LCD_Goto(0,2);
			LCD_SendStr(&str2);
			sprintf(str2, "Ubat %4u ��", ((adc_bat_res_i/ADC_CYCLES_NUMBER)*49));//������ ������� ������� �������� ���
			LCD_Goto(0,3);
			LCD_SendStr(&str2);
		#endif
	#endif
#endif	

#endif
*/
//#if MODE_OF_OPERATION==4 //������� ������ �2	
#if ADC_IS_USED==1
	
//KEY_V_BAT_ON();//������ ������� �� ����������� �������� �� ����� �������������
	//����� ADC - ADC2 (0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0)
	ADMUX = (0<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0);
	adc_init(ADC_PRESCALER_DIV64);
	adc_bat_res_i=0;//������� ���������� ��������� ���
	for (i=1;i<=ADC_CYCLES_NUMBER;i++)
	{
		adc_enable();
		adc_bat_res_i=adc_bat_res_i+adc_read_10bit(ADC_MUX_ADC2,ADC_VREF_AVCC);
		adc_disable();
		//ADCSRA =(1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);//�������� �������������� (ADSC = 1)
		//while((ADCSRA & 0x10)==0);//����, ���� ��� �������� �������������� (ADIF = 0)
		//ADCSRA=(1<<ADIF);//������������� ADIF
		//adc_bat_res_i=adc_bat_res_i+ADCH;
		//ADCSRA =(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE); //��������� ADC �� ��� ���� �� �����
	}
	adc_bat_res_i32=adc_bat_res_i/ADC_CYCLES_NUMBER;
	adc_bat_res_i32=adc_bat_res_i32*414;
	adc_bat_res_i32=adc_bat_res_i32/64;
	//adc_bat_res_i32=adc_bat_res_i32/16;
	
//KEY_V_BAT_OFF();	//������� ������� � ��������
//**********************���������****************//	
	KEY_LIGHT_ON();//������ ������� �� ����������� �������� �� ����� �������������

	//adc_i++;//�������� ������� ���������� �������������� ���
	
	adc_light_res_i=0;//������� ���������� ��������� ���
	adc_init(ADC_PRESCALER_DIV64);
	for (i=1;i<=ADC_CYCLES_NUMBER;i++)
	{
		adc_enable();
		adc_light_res_i=adc_light_res_i+adc_read_10bit(ADC_MUX_ADC6,ADC_VREF_AVCC);
		adc_disable();
		//ADMUX = (0<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0); //ADC6 
		//ADCSRA =(1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);//�������� �������������� (ADSC = 1)
		//while((ADCSRA & 0x10)==0);//����, ���� ��� �������� �������������� (ADIF = 0)
		//ADCSRA=(1<<ADIF);//������������� ADIF
		//adc_light_res_i=adc_light_res_i+ADCH;
		//ADCSRA =(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE); //��������� ADC �� ��� ���� �� �����
	}
	KEY_LIGHT_OFF();	//������� ������� � ��������
	adc_light_res_mV=adc_light_res_i/ADC_CYCLES_NUMBER;
	
#endif	
//#endif

}	

void ADC_V_bat_to_LCD()
{
	#if LCD_IS_USED==1
		#if LCD_TYPE==1
			
				/*
				LcdSingleBar(0,39,7,83,PIXEL_OFF);//������� �������
				for (i=0;i<=7;i++)//�������� �����
				{
					if (i<6)
					{
						LcdSingleBar((i*20),39,7,1,PIXEL_ON);
					}
					if (i<5)
					{
						LcdSingleBar((10+i*20),38,5,1,PIXEL_ON);
					}
					
				}
				
				LcdSingleBar(0,37,3,((adc_bat_res_i/ADC_CYCLES_NUMBER)-41),PIXEL_ON);//�������� �������
				
				
				
				
				if (((load_number_i*8)/60)<=60)
				{
					sprintf(str2, "���:%5lu�����", (load_number_i*8)/60);//������ ���������� ������ ���
				}
				if ((((load_number_i*8)/60)>60) && (((load_number_i*8)/60)<=(60*24)))
				{
					sprintf(str2, "���:%5lu�����", (load_number_i*8)/3600);//������ ���������� ������ ���
				}
				if (((load_number_i*8)/60)>(60*24) )
				{
					sprintf(str2, "���:%6lu����", (load_number_i*8)/86400);//������ ���������� ������ ���
				}
				*/
				

				//sprintf(str2, "���:%04lu:%02lu:%02lu", LOAD_NUMBER_DAY, LOAD_NUMBER_HOUR, LOAD_NUMBER_MINUT);//������ ���������� ������ ���
				//LcdGotoXYFont(0,3);
				//LcdStr(FONT_1X, &str2);
				//sprintf(str2, "L:%3u V:%4u��", ((adc_light_res_i/ADC_CYCLES_NUMBER)),(ADC_BAT_RESULT_mV));//������ ������� ������� �������� ���
				sprintf(str2, "L:%4uV:%4u��", ADC_LIGHT_RESULT_mV, ADC_BAT_RESULT_mV);//������ ������� ������� �������� ���
				LcdGotoXYFont(0,5);			
				LcdStr(FONT_1X, &str2);
				
			
		#endif
		#if LCD_TYPE==0
			sprintf(str2, "���� ��� %5u", load_number_i);//������ ���������� ������ ���
			LCD_Goto(0,2);
			LCD_SendStr(&str2);
			sprintf(str2, "Ubat %4u ��", ((adc_bat_res_i/ADC_CYCLES_NUMBER)*49));//������ ������� ������� �������� ���
			LCD_Goto(0,3);
			LCD_SendStr(&str2);
		#endif
	#endif
};

/*
void ADC_Lidht_to_LCD()
{
	
	KEY_LIGHT_ON();//������ ������� �� ����������� �������� �� ����� �������������

	//adc_i++;//�������� ������� ���������� �������������� ���
	adc_light_res_i=0;//������� ���������� ��������� ���
	for (i=1;i<=ADC_CYCLES_NUMBER;i++)
	{
		ADMUX = (0<<REFS1)|(0<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0);
		ADCSRA =(1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);//�������� �������������� (ADSC = 1)
		while((ADCSRA & 0x10)==0);//����, ���� ��� �������� �������������� (ADIF = 0)
		ADCSRA=(1<<ADIF);//������������� ADIF
		adc_light_res_i=adc_light_res_i+ADCH;
		ADCSRA =(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE); //��������� ADC �� ��� ���� �� �����
	}
	
	KEY_LIGHT_OFF();	//������� ������� � ��������
	
	#if LCD_IS_USED==1
	#if LCD_TYPE==1
	if (ADC_BAT_RESULT_mV>V_LCD_5110_MIN)//���� ���������� �������� ���������� ������ �� �������, ���������� ��
	{
		if (ADC_LIGHT_RESULT>=200)
		{
			KEY_V_BAT_ON();
		} 
		//LcdSingleBar(0,39,7,83,PIXEL_OFF);//������� �������
		//for (i=0;i<=7;i++)//�������� �����
		//{
		//	if (i<6)
		//	{
		//		LcdSingleBar((i*20),39,7,1,PIXEL_ON);
		//	}
		//	if (i<5)
		//	{
		//		LcdSingleBar((10+i*20),38,5,1,PIXEL_ON);
		//	}
		//	
		//}
		
		//LcdSingleBar(0,37,3,((adc_bat_res_i/ADC_CYCLES_NUMBER)-41),PIXEL_ON);//�������� �������
		
		//sprintf(str2, "���� ��� %5u", adc_i);//������ ���������� ������ ���
		//LcdGotoXYFont(0,3);
		//LcdStr(FONT_1X, &str2);
		
		sprintf(str2, "%3u %", ((adc_light_res_i/ADC_CYCLES_NUMBER)));//������ ������� ������� �������� ���
		LcdGotoXYFont(0,5);
		LcdStr(FONT_1X, &str2);
	}
	#endif
	#if LCD_TYPE==0
	sprintf(str2, "���� ��� %5u", load_number_i);//������ ���������� ������ ���
	LCD_Goto(0,2);
	LCD_SendStr(&str2);
	sprintf(str2, "Ubat %4u ��", ((adc_bat_res_i/ADC_CYCLES_NUMBER)*49));//������ ������� ������� �������� ���
	LCD_Goto(0,3);
	LCD_SendStr(&str2);
	#endif
	#endif
	
}
*/
#if 0


	//ADCSRA =(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE); //��������� ADC �� ��� ���� �� �����
//}




//         ADMUX=adc_input | (ADMUX & 0xF0);
//�������� ��� ������������ �������� ����������
         //_delay_us(10);


//      ADCSRA =(1<<ADSC);
         //while((ADCSRA & 0x10)==0);
		 //{}
	//	 while(!(ADCSRA & (1<<ADIF))); //����, ���� ��� �������� �������������� (ADIF = 0)
      //  ADCSRA=(0<<ADIF);//������������� ADIF
        
		 //ADCW - �������� ADCH � ADCL ��� ��� �����
//KEY_OFF();		 


//ISR(ADC_vect)
//{
	//if (adc_i<=7)
	//{
	//	adc_result[adc_i]=ADCH;
	//	adc_i++;
	//	ADC_result();
	//}
	//else
	//{
		//ADCSRA =(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE); //��������� ADC �� ��� ���� �� �����
		//ADC_result();
	//}
	
//	#if (NRF24L01_IS_USED==1)
//	message_unit[0].GET_LIGHTING=ADCL;
//	message_unit[0].GET_LIGHTING=ADCH;
//	#endif
//send_Uart(ADCH);

//printf("U=%.2u mV", (ADCH*25));
//send_Uart(13);
//ADCSRA =(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE); //��������� ADC �� ��� ���� �� �����
//#if LCD_IS_USED==1
//adc_i++;
//adc_res=ADCH;
//LcdSingleBar(0,37,4,83,PIXEL_OFF);
//LcdSingleBar(0,37,4,(adc_res-40),PIXEL_ON);
//sprintf(str2, "������: %6u", adc_i);
//LcdGotoXYFont(0,3);
//LcdStr(FONT_1X, &str2);
    


//sprintf(str2, "Ubat %4u ��", (adc_res*48));
//LcdGotoXYFont(0,5);
//LcdStr(FONT_1X, &str2);
//LcdUpdate();
//#endif


//}
#endif

#endif /* ADC_H_ */
