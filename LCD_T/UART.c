/*
 * CFile1.c
 *
 * Created: 31.01.2015 15:58:45
 *  Author: Vadim
 */ 
#include "UART.h"

#if (UART_IS_USED==1)
volatile FIFO( 64 ) uart_tx_fifo;
volatile FIFO( 64 ) uart_rx_fifo;


//������� ������������� USART
void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//UBRR0=16;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<USBS0)|(3<<UCSZ00);
}

//���������� ���������� �� ��������� ����� �����
ISR( USART_RX_vect )
{
	unsigned char rxbyte = UDR0;
	if( !FIFO_IS_FULL( uart_rx_fifo ) )
	{
		FIFO_PUSH( uart_rx_fifo, rxbyte );
	}
}

ISR( USART_UDRE_vect )
{
	if( FIFO_IS_EMPTY( uart_tx_fifo ) )
	{
		//���� ������ � fifo ������ ��� �� ��������� ��� ����������
		UCSR0B &= ~( 1 << UDRIE0 );
	}
	else
	{
		//����� �������� ��������� ����
		char txbyte = FIFO_FRONT( uart_tx_fifo );
		FIFO_POP( uart_tx_fifo );
		UDR0 = txbyte;
	}
}



int uart_putc(  char c, FILE *file )
{
	int ret;
	cli(); //��������� ����������
	if( !FIFO_IS_FULL( uart_tx_fifo ) )
	{
		//���� � ������ ���� �����, �� ��������� ���� ����
		FIFO_PUSH( uart_tx_fifo, c );
		//� ��������� ���������� �� ������������ �����������
		UCSR0B |= ( 1 << UDRIE0 );
		ret = 0;
	}
	else
	{
		ret = -1; //����� ����������
	}
	sei(); //��������� ����������
	return ret;
}


int uart_getc( FILE* file )
{
	int ret;
	cli(); //��������� ����������
	if( !FIFO_IS_EMPTY( uart_rx_fifo ) )
	{
		//���� � ������ ���� ������, �� ��������� ��
		ret = FIFO_FRONT( uart_rx_fifo );
		FIFO_POP( uart_rx_fifo );
	}
	else
	{
		ret = _FDEV_EOF; //������ ���
	}
	sei(); //��������� ����������
	return ret;
}




void send_Uart(unsigned char c)//   �������� �����
{
	
	//while(!(UCSR0A&(1<<UDRE0)));    //  ���������������, ����� ������� ��������
	//UDR0 = c;
	putchar( c );
	
}
//void send_Uart_message(uint8_t* data)//   �������� �����
//{
//	uint8_t i;
//	for (i = 0; i < 16; i++) {
//
//		printf( data[i]);
//	}
//while(!(UCSR0A&(1<<UDRE0)));    //  ���������������, ����� ������� ��������
//UDR0 = c;


//}
void send_Uart_str(unsigned char* s)//  �������� ������
{
	while (*s!= 0)
	{
		send_Uart(*s++);
	}
}

void send_int_Uart(unsigned int c)//    �������� ����� �� 0000 �� 9999
{
	unsigned char temp;
	c=c%10000;
	temp=c/100;
	send_Uart(temp/10+'0');
	send_Uart(temp%10+'0');
	temp=c%100;;
	send_Uart(temp/10+'0');
	send_Uart(temp%10+'0');
}

#endif