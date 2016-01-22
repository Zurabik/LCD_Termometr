/*
 * Temper.c
 *
 * Created: 20.08.2014 10:39:00
 *  Author: Vadim2
 */ 
#include "Temper.h"
#include "settings.h"
#if LCD_TYPE==0
#include "lcd_lib_2.h"
#endif
#if LCD_TYPE==1
#include "n3310.h"
#endif


//#include "Message.h"
//#if _1WIRE_IS_USED==1
//������ �������� ��� �������� �������
OWI_device allDevices[MAX_DEVICES];
volatile char scratchpad[MAX_DEVICES][2];
volatile char num_device = 0;//���������� ��������� ���������

uint8_t Get_Temp_int()
{
	char scratchpad_int[MAX_DEVICES][2];
	scratchpad_int[0][0]=scratchpad[0][0];
	scratchpad_int[0][1]=scratchpad[0][1];
	uint8_t temperature;
	uint8_t znak_temper=0;// 0- ����� 1-����
	unsigned int tmp = 0;
	/*������� ���� � ����������� �����, ���� ��� �������������*/
	
	if ((scratchpad_int[0][1]&128) == 0) /*���� 7-�� ��� ����� 0, ������ ����� �������������*/
	{
		znak_temper=1;
		//�������� ���� "+" :
	}
	else
	{
		znak_temper=0;
		//�������� ����� "-" :
		tmp = ((unsigned int)scratchpad_int[0][1]<<8)|scratchpad_int[0][0];
		tmp = ~tmp + 1;
		scratchpad_int[0][0] = tmp;
		scratchpad_int[0][1] = tmp>>8;
	}
	//LcdGotoXYFont(4,n+1);
	/*������� �������� ����� ����. �����������*/
	temperature = (scratchpad_int[0][0]>>4)|((scratchpad_int[0][1]&7)<<4);
	if (znak_temper == 0) /*���� -*/
	{
		temperature=128-temperature;
		//�������� ���� "+" :
	}
	else
	{
		temperature=128+temperature;
	}
	return temperature;
}

void Init_Temperatura()
{
	OWI_Init(BUS);
	//unsigned char num= 0;
	uint8_t num = 0;
	unsigned char i= 0;
	//num = 0;
	
	
	//��������� ������� 1-Wire ���������.
	//�������� ������� SEARCH ROM, �������� ������ ���������� 1-Wire ���������,
	// ��������� �� � ��������� ����������� ����� ������� ������.
	//printf("����� 1-Wire");
	//send_Uart(13);
	OWI_SearchDevices(allDevices, MAX_DEVICES, BUS, &num);
	//�������� ������� ����� ���������
	num_device = num; 
	//printf("����� �������� 1-Wire");
	//send_Uart(13);
	//������� ������� ����� ���������
	//unsigned char* id1;
	//printf("������� %u ���������",num);
	//send_Uart(13);
	unsigned char* id1;
	#if UART_IS_USED==1
	//unsigned char* id1;
	printf("������� %u ���������",num);
	send_Uart(13);
	//��� ������� �� ��������� ���������:
 	while(i<=num_device-1)
 	{
		//�������� � UART ������ ��������� ��������
		send_Uart_str("������ � ");
 		printf("%u ",i ); 
		send_Uart_str(" ROM CODE: ");
		id1=allDevices[i].id;
		printf("%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X", id1[7],id1[6],id1[5],id1[4],id1[3],id1[2],id1[1],id1[0]);
 		send_Uart(13);
		i++;
	}
	#endif
	#if LCD_IS_USED==1
		#if LCD_TYPE==1
		char str[14];
		//unsigned char* id1;
		//printf("LCD!");
		//send_Uart(13);
		LcdClear();
		LcdGotoXYFont(0,0);
		//LcdFStr(FONT_1X,(unsigned char*)PSTR("������� "));
		sprintf(str, "������� %u ��.", num);
		LcdStr(FONT_1X, &str);
		LcdUpdate();		
		//printf("������� %u ���������",num);
		//send_Uart(13);
		//��� ������� �� ��������� ���������:
		i=0;
		while(i<=num_device-1)
		{
			//�������� � LCD ������ ��������� ��������
			LcdGotoXYFont(0,i*2+1);
			sprintf(str, "ROM ������� %u:", i);
			LcdStr(FONT_1X, &str);
			LcdGotoXYFont(0,i*2+2);
			//LcdFStr(FONT_1X,(unsigned char*)PSTR("������� "));
			//send_Uart_str("������ � ");
			//printf("%u ",i );
			//send_Uart_str(" ROM CODE: ");
			id1=allDevices[i].id;
			sprintf(str, "%.2X%.2X%.2X%.2X%.2X%.2X%.2X", id1[6],id1[5],id1[4], id1[3],id1[2],id1[1],id1[0]);
			LcdStr(FONT_1X, &str);
			LcdUpdate();
			//send_Uart(13);
			i++;
			_delay_ms(1000);
		}
		#endif
		#if LCD_TYPE==0
			char str[20];
			LCD_Clear();
			LCD_Goto(0,0);
			sprintf(str, "Naydeno %u dt.", num);
			//LcdStr(FONT_1X, &str);
			//LcdUpdate();
			LCD_SendStr(&str);
			_delay_ms(1000);
			//printf("������� %u ���������",num);
			//send_Uart(13);
			//��� ������� �� ��������� ���������:
			i=0;
			while(i<=num_device-1)
			{
				//�������� � LCD ������ ��������� ��������
				//LcdGotoXYFont(0,i*2+1);
				LCD_Goto(0,1);
				sprintf(str, "ROM ������� %u:", i);
				LCD_SendStr(&str);
				_delay_ms(1000);
				LCD_Goto(0,2);
				//LcdGotoXYFont(0,i*2+2);//
				//LcdFStr(FONT_1X,(unsigned char*)PSTR("������� "));
				//send_Uart_str("������ � ");
				//printf("%u ",i );
				//send_Uart_str(" ROM CODE: ");
				id1=allDevices[i].id;
				sprintf(str, "%.2X%.2X%.2X%.2X%.2X%.2X%.2X", id1[6],id1[5],id1[4], id1[3],id1[2],id1[1],id1[0]);
				LCD_SendStr(&str);
				//LcdUpdate();//
				//send_Uart(13);
				i++;
				//_delay_ms(1000);
			}
		#endif
		
	#endif
	//������ ������ ������
	OWI_DetectPresence(BUS);
	//������� ��� ��������� ���� ��������� �� ����
	OWI_SkipRom(BUS);
	//������ ������� - ����� �������������� 
	OWI_SendByte(DS18B20_CONVERT_T ,BUS);
	//��� ������� ����������
}

void Get_all_Temperatura()
{
	//unsigned int tmp = 0;
	//unsigned char temperature;
	unsigned char n=0;
// 
//      while (!OWI_ReadBit(BUS))
// 	{
// 	
// 	};

//��� ������� �� ��������� ���������:
	while (n<=num_device-1)
	{
		/*������ ������ ������
		������� ��� ��������� n-�� ���������� �� ����
		������� - ������ ���������� ������
		����� ��������� ���������� ������ ������� � ������
		*/
		OWI_DetectPresence(BUS);
		OWI_MatchRom(allDevices[n].id, BUS);
		OWI_SendByte(DS18B20_READ_SCRATCHPAD, BUS);
		scratchpad[n][0] = OWI_ReceiveByte(BUS);
		//message_unit[0].GET_TEMP[0]=scratchpad[0];
		scratchpad[n][1] = OWI_ReceiveByte(BUS);
		//	message_unit[0].GET_TEMP[1]=scratchpad[1];
		n++;
	}
//������ ������ ������
//������� ��� ��������� ���� ��������� �� ����
//������ ������� - ����� �������������� *
OWI_DetectPresence(BUS);
OWI_SkipRom(BUS);
OWI_SendByte(DS18B20_CONVERT_T ,BUS);
}

void Set_Temp_to_LCD()
{
	#if LCD_IS_USED==1
		//KEY_V_BAT_ON();
		Get_all_Temperatura();//(10 ms)
		//KEY_V_BAT_OFF();
		unsigned int tmp = 0;
		uint8_t znak_temper=0;// 0- ����� 1-����
		uint8_t temperature;
		uint8_t temperature_i;
		char scratchpad_lcd[MAX_DEVICES][2];
		
		unsigned char n=0;
		#if LCD_TYPE==1
			char str[14];
			//LcdClear();
			while(n<=num_device-1)
			{
				scratchpad_lcd[n][0]=scratchpad[n][0];
				scratchpad_lcd[n][1]=scratchpad[n][1];
					//LcdGotoXYFont(0,n+1);
					//sprintf(str, "t");
					//LcdStr(FONT_2X, &str);
					//LcdFStr(FONT_1X,(unsigned char*)PSTR("�-�� "));
				//LcdGotoXYFont(1,1);
				//LcdSingleBar(0,31,31,83,PIXEL_OFF);//������� ������� "���������� ������� ������ ����"(Y,X,������,������, ������� ��� ���������) �������
				/*������� ���� � ����������� �����, ���� ��� �������������*/
				//LCD_Goto(5,0);
				if ((scratchpad_lcd[n][1]&128) == 0) /*���� 7-�� ��� ����� 0, ������ ����� �������������*/
				{
					znak_temper=1;
					//�������� ���� "+" :
					//LcdSingleBar(0,17,4,12,PIXEL_ON);//- �������������� �����
					//LcdSingleBar(4,21,12,4,PIXEL_ON);//| ������������ �����
					//LcdFStr(FONT_2X,(unsigned char*)PSTR("+"));
					
					////send_Uart_str('+');
					////LCD_WriteData('+');
				}
				else
				{
					znak_temper=0;
					//LcdSingleBar(0,17,4,12,PIXEL_ON);//- �������������� ����� (�����)
					//sprintf(str, "-");
					//LcdStr(FONT_2X, &str);
					
					////LcdFStr(FONT_BIG_I,(unsigned char*)PSTR("-"));
					////send_Uart_str("-");
					////LCD_WriteData('-');
					tmp = ((unsigned int)scratchpad_lcd[n][1]<<8)|scratchpad_lcd[n][0];
					tmp = ~tmp + 1;
					scratchpad_lcd[n][0] = tmp;
					scratchpad_lcd[n][1] = tmp>>8;
				}
				//LcdGotoXYFont(4,n+1);
				/*������� �������� ����� ����. �����������*/
				temperature = (scratchpad_lcd[n][0]>>4)|((scratchpad_lcd[n][1]&7)<<4);
				//����������� � ������� ���� �� ����� :(
				if (temperature < 100)
				{ 
					//���� ����������� ���������� �����, ������ �� ������
					//KEY_V_BAT_ON();
					//LcdSingleBar(0,31,32,17,PIXEL_OFF);//������� ������� "���������� ������� ������ ����"(Y,X,������,������, ������� ��� ���������) �������
					//KEY_V_BAT_OFF();
					if (znak_temper==0)
					{
						//���� �����
						LcdSingleBar(0,17,4,12,PIXEL_ON);//- �������������� ����� (�����)
					} 
					else
					{
						//���� ����
						LcdSingleBar(0,17,4,12,PIXEL_ON);//- �������������� �����
						LcdSingleBar(4,21,12,4,PIXEL_ON);//| ������������ �����
					}
					if (temperature<10)
					{
						LcdBarsFontNumber(temperature,1);
					}
					if (temperature>=10)
					{
						/*������� �������*/
						//KEY_V_BAT_ON();
						LcdBarsFontNumber(temperature/10,0);
						//KEY_V_BAT_OFF();
						temperature_i=temperature/10;
						/*������� �������*/
						LcdBarsFontNumber((temperature-(temperature_i*10)),1);
					}
					/*������� ������� ����� ����. �����������*/
					temperature = (scratchpad[n][0]&15);
					temperature = (temperature<<1) + (temperature<<3);
					temperature = (temperature>>4);
					
					LcdSingleBar(54,31,4,4,PIXEL_ON);
					LcdBarsFontNumber(temperature,2);
					
				}
				else
				{
					//���� ����������� ������ 99 ��������, ������ �� ������
					LcdGotoXYFont(0,1);
					if (znak_temper==0)
					{
						//���� �����
						LcdFStr(FONT_2X,(unsigned char*)PSTR("-"));
					}
					else
					{
						//���� ����
						LcdFStr(FONT_2X,(unsigned char*)PSTR("+"));
					}
					
					sprintf(str, "%3u", temperature);
					LcdStr(FONT_2X, &str);
					
					/*������� ������� ����� ����. �����������*/
					temperature = (scratchpad[n][0]&15);
					temperature = (temperature<<1) + (temperature<<3);
					temperature = (temperature>>4);
					
					LcdGotoXYFont(8,1);
					sprintf(str, ".%u", temperature);
					LcdStr(FONT_2X, &str);
				}
				
				//LcdBarsFontNumber(0,1);

				//printf(" %u",  temperature);
				/*������� ������� ����� ����. �����������*/
				//temperature = (scratchpad[n][0]&15);
				//temperature = (temperature<<1) + (temperature<<3);
				//temperature = (temperature>>4);
				
				//printf(".%u",  temperature);
				//sprintf(str, ".%u", temperature);
				//LcdStr(FONT_2X, &str);
				
				//send_Uart(13);
				n++;
			}
			//LcdUpdate();
		#endif
		#if LCD_TYPE==0
			char str[14];
			//LcdClear();
			while(n<=num_device-1)
			{
				
				LCD_Goto(0,n+1);
				//LcdGotoXYFont(0,n+1);
				sprintf(str, "t");
				LCD_SendStr(&str);
				//LcdStr(FONT_2X, &str);
				//LcdFStr(FONT_1X,(unsigned char*)PSTR("�-�� "));
				//LcdGotoXYFont(4,n+1);
				LCD_Goto(4,n+1);
				/*������� ���� � ����������� �����, ���� ��� �������������*/
				//LCD_Goto(5,0);
				if ((scratchpad[n][1]&128) == 0) /*���� 7-�� ��� ����� 0, ������ ����� �������������*/
				{
					//LCD_SendStr("+");
					//LcdFStr(FONT_2X,(unsigned char*)PSTR("+"));
					//send_Uart_str('+');
					LCD_WriteData('+');
				}
				else
				{
					//LcdFStr(FONT_2X,(unsigned char*)PSTR("-"));
					//send_Uart_str("-");
					LCD_WriteData('-');
					tmp = ((unsigned int)scratchpad[n][1]<<8)|scratchpad[n][0];
					tmp = ~tmp + 1;
					scratchpad[n][0] = tmp;
					scratchpad[n][1] = tmp>>8;
				}
				LCD_Goto(6,n+1);
				//LcdGotoXYFont(6,n+1);
				/*������� �������� ����� ����. �����������*/
				temperature = (scratchpad[n][0]>>4)|((scratchpad[n][1]&7)<<4);
				//send_int_Uart(temperature);
				sprintf(str, "%2u", temperature);
				LCD_SendStr(&str);
				//LcdStr(FONT_2X, &str);
				//LcdGotoXYFont(10,n+1);
				LCD_Goto(10,n+1);
				//printf(" %u",  temperature);
				/*������� ������� ����� ����. �����������*/
				temperature = (scratchpad[n][0]&15);
				temperature = (temperature<<1) + (temperature<<3);
				temperature = (temperature>>4);
				//printf(".%u",  temperature);
				sprintf(str, ".%u", temperature);
				LCD_SendStr(&str);
				//LcdStr(FONT_2X, &str);
	
				//send_Uart(13);
				n++;
			}
			//LcdUpdate();
		#endif
	#endif

}

//void Set_Temp_to_UART()
//{
			//
			//#if UART_IS_USED==1
			//unsigned char n=0;
			//unsigned int tmp = 0;
			//unsigned char temperature;
			//send_Uart_str("������� ���� ");
			//send_int_Uart(n);
			//send_Uart_str("-�� �������");
			//send_Uart(13);
			//printf("scratchpad[0]: %X", scratchpad[n][0]);
			//send_Uart(13);
			//printf("scratchpad[1]: %X", scratchpad[n][1]);
			//send_Uart(13);
			//
			///*������� ���� � ����������� �����, ���� ��� �������������*/
			////LCD_Goto(5,0);
			//if ((scratchpad[n][1]&128) == 0) /*���� 7-�� ��� ����� 0, ������ ����� �������������*/
			//{
				//send_Uart_str('+');
				////LCD_WriteData('+');
			//}
			//else
			//{
				//send_Uart_str("-");
				////LCD_WriteData('-');
				//tmp = ((unsigned int)scratchpad[n][1]<<8)|scratchpad[n][0];
				//tmp = ~tmp + 1;
				//scratchpad[n][0] = tmp;
				//scratchpad[n][1] = tmp>>8;
			//}
			///*������� �������� ����� ����. �����������*/
			//temperature = (scratchpad[n][0]>>4)|((scratchpad[n][1]&7)<<4);
			////send_int_Uart(temperature);
			//printf(" %u",  temperature);
			///*������� ������� ����� ����. �����������*/
			//temperature = (scratchpad[n][0]&15);
			//temperature = (temperature<<1) + (temperature<<3);
			//temperature = (temperature>>4);
			//printf(".%u",  temperature);
			//send_Uart(13);
			//#endif
			//#if LCD_IS_USED==1
			//
			//send_Uart_str("������� ���� ");
			//send_int_Uart(n);
			//send_Uart_str("-�� �������");
			//send_Uart(13);
			//printf("scratchpad[0]: %X", scratchpad[n][0]);
			//send_Uart(13);
			//printf("scratchpad[1]: %X", scratchpad[n][1]);
			//send_Uart(13);
			//
			///*������� ���� � ����������� �����, ���� ��� �������������*/
			////LCD_Goto(5,0);
			//if ((scratchpad[n][1]&128) == 0) /*���� 7-�� ��� ����� 0, ������ ����� �������������*/
			//{
				//send_Uart_str('+');
				////LCD_WriteData('+');
			//}
			//else
			//{
				//send_Uart_str("-");
				////LCD_WriteData('-');
				//tmp = ((unsigned int)scratchpad[n][1]<<8)|scratchpad[n][0];
				//tmp = ~tmp + 1;
				//scratchpad[n][0] = tmp;
				//scratchpad[n][1] = tmp>>8;
			//}
			///*������� �������� ����� ����. �����������*/
			//temperature = (scratchpad[n][0]>>4)|((scratchpad[n][1]&7)<<4);
			////send_int_Uart(temperature);
			//printf(" %u",  temperature);
			///*������� ������� ����� ����. �����������*/
			//temperature = (scratchpad[n][0]&15);
			//temperature = (temperature<<1) + (temperature<<3);
			//temperature = (temperature>>4);
			//printf(".%u",  temperature);
			//send_Uart(13);
			//#endif
			//n++;
//}

void Get_temp(unsigned char *temp[])
{
	//���� ���� ���� ���������� �� ����
if (num_device==1)
	{
	//������ ������ ������
    OWI_DetectPresence(BUS);
    //OWI_MatchRom(allDevices[0].id, BUS);
	//������� ������
	OWI_SkipRom(BUS);
	//������� - ������ ���������� ������
    OWI_SendByte(DS18B20_READ_SCRATCHPAD, BUS);
    //scratchpad[0] = OWI_ReceiveByte(BUS);
	//message_unit.GET_TEMP[0]= OWI_ReceiveByte(BUS);
	*temp[0]=OWI_ReceiveByte(BUS);
    //scratchpad[1] = OWI_ReceiveByte(BUS);
	//message_unit.GET_TEMP[1]=OWI_ReceiveByte(BUS);
	*temp[1]=OWI_ReceiveByte(BUS);
	// 		 /*������ ������ ������
	 //     ������� ��� ��������� ���� ��������� �� ����
	 //     ������ ������� - ����� �������������� */
	 OWI_DetectPresence(BUS);
	 OWI_SkipRom(BUS);
	 OWI_SendByte(DS18B20_CONVERT_T ,BUS);
	 }
}

//#endif