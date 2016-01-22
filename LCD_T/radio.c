/*
 * radio.c
 *
 * Created: 27.05.2014 0:02:39
 *  Author: Vadim2
 */ 
/*
 * radio.c
 *
 *  Created on: 24-Jan-2009
 *      Author: Neil MacMillan
 */
#include "radio.h"
#if (NRF24L01_IS_USED==1)
//#include "Message.h"

//#include "settings.h"

// non-public constants and macros

#define CHANNEL 110
#define ADDRESS_LENGTH 5
#define ACK_W_PAYLOAD_ENABLE 1 //Используем ответ с данными(необходимо настроить: авторетрансмит, автоответ, динамическая длинна пакета)

// Указываем ноги для chip select and chip enable куда подключен  radio module
#define CE_DDR          DDRC
#define CE_PORT         PORTC
#define CE_PIN          PINC1
#define CSN_DDR         DDRB
#define CSN_PORT        PORTB
#define CSN_PIN         PINB2

// Определения для выбора и разрешения радио
#define CSN_HIGH()      CSN_PORT |=  _BV(CSN_PIN);
#define CSN_LOW()       CSN_PORT &= ~_BV(CSN_PIN);
#define CE_HIGH()       CE_PORT |=  _BV(CE_PIN);
#define CE_LOW()        CE_PORT &= ~_BV(CE_PIN);

// Flag which denotes that the radio is currently transmitting
static volatile uint8_t transmit_lock;
// tracks the payload widths of the Rx pipes
static volatile uint8_t rx_pipe_widths[6] = {32, 32, 0, 0, 0, 0};
// holds the transmit address (Rx pipe 0 is set to this address when transmitting with auto-ack enabled).
static volatile uint8_t tx_address[5] = { 0xe7, 0xe7, 0xe7, 0xe7, 0xe7 };
// holds the receiver address for Rx pipe 0 (the address is overwritten when transmitting with auto-ack enabled).
static volatile uint8_t rx_pipe0_address[5] = { 0xe7, 0xe7, 0xe7, 0xe7, 0xe7 };
// the driver keeps track of the success status for the last 16 transmissions
static volatile uint16_t tx_history = 0xFF;

static volatile RADIO_TX_STATUS tx_last_status = RADIO_TX_SUCCESS;
//extern void radio_rxhandler(uint8_t pipenumber)
volatile uint8_t rxflag = 0;
//volatile uint8_t rxflag_NRF = 0;
//volatile uint8_t rxflag_NRF = 0;
//volatile char tx_delay = 255;
void radio_rxhandler(uint8_t pipe_number)
{
	//message.address_dt++;
	//rxflag = 1;
	//send_Uart_str("isr");
	//send_Uart(13);
	// Эта функция вызывается, когда радио получает пакет.
	// It is called in the radio's ISR, so it must be kept short.
	// Он вызывается в ISR радиостанции, поэтому он должен быть коротким.
	// The function may be left empty if the application doesn't need to respond immediately to the interrupt.
	//Функция может быть пустым, если приложение не нужно немедленно реагировать на прерывания.
}
//NRF:

char output[128];

//uint8_t station_addr[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };	// Адрес приемника Receiver address
uint8_t station_addr[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };		// Адрес приемника Receiver address
//uint8_t my_addr[5] = { 0x98, 0x76, 0x54, 0x32, 0x10 };		// Адрес передатчика Transmitter address
uint8_t my_addr[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7  };			// Адрес передатчика Transmitter address


RADIO_RX_STATUS rx_status;

radiopacket_t packet;
//extern void radio_rxhandler(uint8_t pipenumber);

/**
 * Получаем статусный регистр
 * Retrieve the status register.
 */
static uint8_t get_status()
{
        uint8_t status = 0;
        CSN_LOW();

        status = SPI_Write_Byte(NOP);

        CSN_HIGH();

        return status;
}
/**
 * Set a register in the radio
 * \param reg The register value defined in nRF24L01.h (e.g. CONFIG, EN_AA, &c.).
 * \param value The value to write to the given register (the whole register is overwritten).
 * \return The status register.
 */
static uint8_t set_register(radio_register_t reg, uint8_t* value, uint8_t len)
{
        uint8_t status;
        CSN_LOW();

        status = SPI_Write_Byte(W_REGISTER | (REGISTER_MASK & reg));
        SPI_Write_Block(value, len);

        CSN_HIGH();

        return status;
}

/**
 * Retrieve a register value from the radio.
 * \param reg The register value defined in nRF24L01.h (e.g. CONFIG, EN_AA, &c.).
 * \param buffer A contiguous memory block into which the register contents will be copied.  If the buffer is too long for the
 *              register contents, then the remaining bytes will be overwritten with 0xFF.
 * \param len The length of the buffer.
 */
static uint8_t get_register(radio_register_t reg, uint8_t* buffer, uint8_t len)
{
        uint8_t status, i;
        for (i = 0; i < len; i++)
        {
                // If the buffer is too long for the register results, then the radio will interpret the extra bytes as instructions.
                // To remove the risk, we set the buffer elements to NOP instructions.
                buffer[i] = 0xFF;
        }
        CSN_LOW();

        status = SPI_Write_Byte(R_REGISTER | (REGISTER_MASK & reg));
        SPI_ReadWrite_Block(NULL, buffer, len);

        CSN_HIGH();

        return status;
}

/**
 * Send an instruction to the nRF24L01.
 * \param instruction The instruction to send (see the bottom of nRF24L01.h)
 * \param data An array of argument data to the instruction.  If len is 0, then this may be NULL.
 * \param buffer An array for the instruction's return data.  This can be NULL if the instruction has no output.
 * \param len The length of the data and buffer arrays.
 */
static void send_instruction(uint8_t instruction, uint8_t* data, uint8_t* buffer, uint8_t len)
{
    CSN_LOW();
        // send the instruction
        SPI_Write_Byte(instruction);
        // pass in args
        if (len > 0)
        {
                if (buffer == NULL)     //
                        SPI_Write_Block(data, len);
                else
                        SPI_ReadWrite_Block(data, buffer, len);
        }
    // resynch SPI
    CSN_HIGH();
}
/************************************************************************/
/* Установка режима	приема или передачи									*/
/************************************************************************/
static void set_mode(uint8_t mode, RADIO_STDB2A_DELAY delay )
{
	uint8_t config;
	get_register(CONFIG, &config, 1);
	if ((config & _BV(PRIM_RX)) != mode)
	{
		
		if (mode==1)
		{
			config |= _BV(PRIM_RX);			//приемник
		} 
		else
		{
			config &= ~_BV(PRIM_RX);		//передатчик
		}
		//config |= _BV(PRIM_RX);
		set_register(CONFIG, &config, 1);
		if (delay==RADIO_DELAY_ENABLE)
		{
			// the radio takes 130 us to power up the receiver.
			_delay_us(65);
			_delay_us(65);
		} 
		
	}
}



/**
 * Switch the radio to receive mode.  If the radio is already in receive mode, this does nothing.
 */
/*
static void set_rx_mode()
{
        uint8_t config;
        get_register(CONFIG, &config, 1);
        if ((config & _BV(PRIM_RX)) == 0)
        {
                config |= _BV(PRIM_RX);
                set_register(CONFIG, &config, 1);
                // the radio takes 130 us to power up the receiver.
                _delay_us(65);
                _delay_us(65);
        }
}
*/

/**
 * Switch the radio to transmit mode.  If the radio is already in transmit mode, this does nothing.
 */
/*
static void set_tx_mode()
{
        uint8_t config;
        get_register(CONFIG, &config, 1);
        if ((config & _BV(PRIM_RX)) != 0)
        {
                config &= ~_BV(PRIM_RX);
                set_register(CONFIG, &config, 1);
                // The radio takes 130 us to power up the transmitter
                // You can delete this if you're sending large packets (I'm thinking > 25 bytes, but I'm not sure) because it
                // sending the bytes over SPI can take this long.
                _delay_us(65);
                _delay_us(65);
        }
}
*/

/**
 * Reset the pipe 0 address if pipe 0 is enabled.  This is necessary when the radio is using Enhanced Shockburst, because
 * the pipe 0 address is set to the transmit address while the radio is transmitting (this is how the radio receives
 * auto-ack packets).
 */
static void reset_pipe0_address()
{
        if (rx_pipe_widths[RADIO_PIPE_0] != 0)
        {
                // reset the pipe 0 address if pipe 0 is enabled.
                set_register(RX_ADDR_P0, (uint8_t*)rx_pipe0_address, ADDRESS_LENGTH);
        }
}

/**
 * Configure radio defaults and turn on the radio in receive mode.
 * This configures the radio to its max-power, max-packet-header-length settings.  If you want to reduce power consumption
 * or increase on-air payload bandwidth, you'll have to change the config.
 */
static void configure_registers()
{
        uint8_t value;

        SPI_Init();
/************************************************************************/
/*Setup of Address Widths                                               */
/************************************************************************/
        // set address width to 5 bytes.
        value = ADDRESS_LENGTH - 2;                     // 0b11 for 5 bytes, 0b10 for 4 bytes, 0b01 for 3 bytes
        set_register(SETUP_AW, &value, 1);
/************************************************************************/
/*Setup of Automatic Retransmission										*/
/************************************************************************/
        // set Enhanced Shockburst retry to every 586 us, up to 5 times.  If packet collisions are a problem even with AA enabled,
        // then consider changing the retry delay to be different on the different stations so that they do not keep colliding on each retry.
        /*
		Auto Retransmit Delay
		‘0000’ – Wait 250µS
		‘0001’ – Wait 500µS
		‘0010’ – Wait 750µS
		……..
		‘1111’ – Wait 4000µS
		(Delay defined from end of transmission to start of
		next transmission)
		
		Auto Retransmit Count
		‘0000’ –Re-Transmit disabled
		‘0001’ – Up to 1 Re-Transmit on fail of AA
		……
		‘1111’ – Up to 15 Re-Transmit on fail of AA
		*/
		value = (1<<ARD)|(9<<ARC);//9 повторов передачи каждые 500 мкс
		//value = 0x15;
        //value = 0x10;
        set_register(SETUP_RETR, &value, 1);
/************************************************************************/
/*Setup RF Channel														*/
/************************************************************************/
        // Set to use 2.4 GHz channel 110.
        value = CHANNEL;
        set_register(RF_CH, &value, 1);
/************************************************************************/
/* RF Setup Register													*/
/************************************************************************/
        // Set radio to 2 Mbps and high power.  Leave LNA_HCURR at its default.
        //value = _BV(RF_DR) | _BV(LNA_HCURR);
		//value = _BV(RF_DR_HIGH);
		value = (0<<RF_DR_LOW)|(0<<PLL_LOCK)|(1<<RF_DR_HIGH)|(3<<RF_PWR);
        set_register(RF_SETUP, &value, 1);
/************************************************************************/
/* Configuration Register												*/
/************************************************************************/
        // Enable 2-byte CRC and power up in receive mode.
        value = _BV(EN_CRC) | _BV(CRCO) | _BV(PWR_UP) | _BV(PRIM_RX);
        set_register(CONFIG, &value, 1);
/************************************************************************/
/* Очистка Status Register												*/
/************************************************************************/
        // clear the interrupt flags in case the radio's still asserting an old unhandled interrupt
		value = _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT);
		set_register(STATUS, &value, 1);
/************************************************************************/
/* FEATURE регистр											*/
/************************************************************************/
// разрешим динамическую длинну пакета, ответ с пакетом
value = (1<<EN_DPL)|(1<<EN_ACK_PAY);
set_register(FEATURE, &value, 1);
/************************************************************************/
/* DYNPD регистр											*/
/************************************************************************/
// разрешим динамическую длинну пакета для канала 0
value = (1<<DPL_P0);
set_register(DYNPD, &value, 1);
/************************************************************************/
/* Очистка буфера приема и передачи										*/
/************************************************************************/
    // flush the FIFOs in case there are old data in them.
	//Radio_Flush();
       send_instruction(FLUSH_TX, NULL, NULL, 0);
       send_instruction(FLUSH_RX, NULL, NULL, 0);
}

void Radio_Init()
{
        transmit_lock = 0;				// disable radio during config
        CE_LOW();

        // set as output AT90 pins connected to the radio's slave select and chip enable pins.
        CE_DDR |= _BV(CE_PIN);
        CSN_DDR |= _BV(CSN_PIN);

        // Enable radio interrupt.  This interrupt is triggered when data are received and when a transmission completes.
//		DDRC &= ~_BV(PIND2);			// настраиваем прерывания INT0
//		EICRA = (0<<ISC01)|(1<<ISC00);	// настраиваем прерывания INT0
//		EIMSK = (1<<INT0);				// настраиваем прерывания INT0


         DDRC &= ~_BV(PINC0);
         PCICR = (1<<PCIE1);
//		 PCICR |= _BV(PCIE1);// настраиваем прерывания PCINT8
		 PCMSK1 = (1<<PCINT8);
 //        PCMSK1 |= _BV(PCINT8);// настраиваем прерывания PCINT8
// 		EICRB |= _BV(ISC71);
//         EICRB &= ~_BV(ISC70);
//         EIMSK |= _BV(INT7);

        // A 10.3 ms delay is required between power off and power on states (controlled by 3.3 V supply).
        _delay_ms(11);

        // Configure the radio registers that are not application-dependent.
        configure_registers();

        // A 1.5 ms delay is required between power down and power up states (controlled by PWR_UP bit in CONFIG)
        _delay_ms(2);

        // enable radio as a receiver
        CE_HIGH();
}

// default address for pipe 0 is 0xe7e7e7e7e7
// default address for pipe 1 is 0xc2c2c2c2c2
// default address for pipe 2 is 0xc2c2c2c2c3 (disabled)
// default address for pipe 3 is 0xc2c2c2c2c4 (disabled)
// default address for pipe 4 is 0xc2c2c2c2c5 (disabled)
// default address for pipe 5 is 0xc2c2c2c2c6 (disabled)
void Radio_Configure_Rx(RADIO_PIPE pipe, uint8_t* address, uint8_t enable)
{
        uint8_t value;
        uint8_t use_aa = 1;
        uint8_t payload_width = 32; //размер пакета
        if (payload_width < 1 || payload_width > 32 || pipe < RADIO_PIPE_0 || pipe > RADIO_PIPE_5) return;

        // store the pipe 0 address so that it can be overwritten when transmitting with auto-ack enabled.
        if (pipe == RADIO_PIPE_0)
        {
                rx_pipe0_address[0] = address[0];
                rx_pipe0_address[1] = address[1];
                rx_pipe0_address[2] = address[2];
                rx_pipe0_address[3] = address[3];
                rx_pipe0_address[4] = address[4];
        }

        // Set the address.  We set this stuff even if the pipe is being disabled, because for example the transmitter
        // needs pipe 0 to have the same address as the Tx address for auto-ack to work, even if pipe 0 is disabled.
        set_register(RX_ADDR_P0 + pipe, address, pipe > RADIO_PIPE_1 ? 1 : ADDRESS_LENGTH);

        // Set auto-ack.
        get_register(EN_AA, &value, 1);
        if (use_aa)
                value |= _BV(pipe); //в единичку
        else
                value &= ~_BV(pipe);//в нолик
        set_register(EN_AA, &value, 1);

        // Set the pipe's payload width.  If the pipe is being disabled, then the payload width is set to 0.
        value = enable ? payload_width : 0;
        set_register(RX_PW_P0 + pipe, &value, 1);
        rx_pipe_widths[pipe] = value;

        // Enable or disable the pipe.
        get_register(EN_RXADDR, &value, 1);
        if (enable)
                value |= _BV(pipe);
        else
                value &= ~_BV(pipe);
        set_register(EN_RXADDR, &value, 1);

}

// default transmitter address is 0xe7e7e7e7e7.
/*
void Radio_Set_Tx_Addr(uint8_t* address)
{
        tx_address[0] = address[0];
        tx_address[1] = address[1];
        tx_address[2] = address[2];
        tx_address[3] = address[3];
        tx_address[4] = address[4];
        set_register(TX_ADDR, address, ADDRESS_LENGTH);
}
*/
void Radio_Configure(RADIO_DATA_RATE dr, RADIO_TX_POWER power)
{
        uint8_t value;

        if (power < RADIO_LOWEST_POWER || power > RADIO_HIGHEST_POWER || dr < RADIO_1MBPS || dr > RADIO_2MBPS) return;

        // set the address
        //Radio_Set_Tx_Addr(address);

        // set the data rate and power bits in the RF_SETUP register
        get_register(RF_SETUP, &value, 1);

        value |= 3 << RF_PWR;                   // set the power bits so that the & will mask the power value in properly.
        value &= power << RF_PWR;               // mask the power value into the RF status byte.

        if (dr)
                value |= _BV(RF_DR_HIGH);
        else
                value &= ~_BV(RF_DR_HIGH);

        set_register(RF_SETUP, &value, 1);
}

uint8_t Radio_Transmit(radiopacket_t* payload, uint8_t *tx_addr, RADIO_TX_WAIT wait)
{
        //if (block && transmit_lock) while (transmit_lock);
        //if (!block && transmit_lock) return 0;
    uint8_t len = 32;
	//uint8_t max_delay = 0;
        // indicate that the driver is transmitting.
    transmit_lock = 1;

     
 // disable the radio while writing to the Tx FIFO.
    CE_LOW();

    //set_tx_mode();
	set_mode(RADIO_PRIM_TX, RADIO_DELAY_DISABLE);
    // for auto-ack to work, the pipe0 address must be set to the Tx address while the radio is transmitting.
    // The register will be set back to the original pipe 0 address when the TX_DS or MAX_RT interrupt is asserted.
    set_register(RX_ADDR_P0, (uint8_t*)tx_addr, ADDRESS_LENGTH);
	set_register(TX_ADDR, (uint8_t*)tx_addr, ADDRESS_LENGTH);
	// Записываем пакет в ТХ FIFO буфер для передачи
 	send_instruction(W_TX_PAYLOAD, payload, NULL, len);
    // Начинаем передачу
    CE_HIGH();
	_delay_us(11);// подождем положенные 10 мкс
	CE_LOW();
    if (wait == RADIO_WAIT_FOR_TX) 
    {
		while (transmit_lock)
		return tx_last_status;//сдесь будет либо "RADIO_TX_SUCCESS"-отправили успешно, либо "RADIO_TX_MAX_RT"-попытки передать не увеньчались успехом

		//printf("ждемс...");
       // while (transmit_lock || max_delay<90) //собсно ждемс...
		//	{
		//		_delay_ms(1);
		//		max_delay++;
		//	}
		//printf("дождались!!");
		//if(transmit_lock!=1)
		//	{
		//		printf("ok");
		//		return tx_last_status;//сдесь будет либо "RADIO_TX_SUCCESS"-отправили успешно, либо "RADIO_TX_MAX_RT"-попытки передать не увеньчались успехом
		//	}
		//	else
		//	{
		//		printf("no ok");
		//		return RADIO_TX_MAX_RT; // "RADIO_TX_MAX_RT"-попытки передать не увеньчались успехом
		//	}
	}
	else
	{
		//данные кудато , наверное, переданы
		return RADIO_TX_SUCCESS;
	}

	
    
}

RADIO_RX_STATUS Radio_Receive(radiopacket_t* buffer)
{
        uint8_t len = 32;
		//uint8_t len_rx_pl=0;
        uint8_t status;
        uint8_t pipe_number;
        uint8_t doMove = 1;
        RADIO_RX_STATUS result;

        transmit_lock = 0;

        CE_LOW();

		status = get_status();
		
        pipe_number =  (status & 0xE) >> 1;
		//printf("Номер канала по которому принят пакет %u .", pipe_number);
        if (pipe_number == RADIO_PIPE_EMPTY)
        {
                result = RADIO_RX_FIFO_EMPTY;
                doMove = 0;
        }

        if (rx_pipe_widths[pipe_number] > len)
        {
                // the buffer isn't big enough, so don't copy the data.
                result = RADIO_RX_INVALID_ARGS;
                doMove = 0;
        }

        if (doMove)
        {
                // попробуем так:
				//send_instruction(R_RX_PL_WID, NULL, len_rx_pl, NULL);
				//printf("длинна пакета %u .", len_rx_pl);
				//send_instruction(R_RX_PAYLOAD, (uint8_t*)buffer, (uint8_t*)buffer, len_rx_pl);
				// Move the data payload into the local
                send_instruction(R_RX_PAYLOAD, (uint8_t*)buffer, (uint8_t*)buffer, rx_pipe_widths[pipe_number]);

                status = get_status();
                pipe_number =  (status & 0xE) >> 1;

                if (pipe_number != RADIO_PIPE_EMPTY)
                        result = RADIO_RX_MORE_PACKETS;
                else
                        result = RADIO_RX_SUCCESS;
        }

        CE_HIGH();

        transmit_lock = 0;

        //release_radio();

        return result;
}

// This is only accurate if all the failed packets were sent using auto-ack.
uint8_t Radio_Success_Rate()
{
        uint16_t wh = tx_history;
        uint8_t weight = 0;
        while (wh != 0)
        {
                if ((wh & 1) != 0) weight++;
                wh >>= 1;
        }
        wh = (16 - weight) * 100;
        wh /= 16;
        return wh;
}

/*
void Radio_Flush()
{
        send_instruction(FLUSH_TX, NULL, NULL, 0);
        send_instruction(FLUSH_RX, NULL, NULL, 0);
}
*/

// Interrupt handler
ISR(PCINT1_vect)
{
	uint8_t status;
	uint8_t pipe_number;

	CE_LOW();

	status = get_status();

	if (status & _BV(RX_DR))				//Новые данные в буфере приемника
	{
		pipe_number =  (status & 0xE) >> 1;//номер канала для которого получили данные
		radio_rxhandler(pipe_number);
		rxflag=1;
	}
	// We can get the TX_DS or the MAX_RT interrupt, but not both.
	if (status & _BV(TX_DS))//Данные переданы успешно. Если автоответ разрешен то он получен.
	{
		// if there's nothing left to transmit, switch back to receive mode.
		transmit_lock = 0;
		//reset_pipe0_address();
		//set_mode(RADIO_PRIM_RX);//??????
		//set_rx_mode();

		// indicate in the history that a packet was transmitted successfully by appending a 1.
		tx_history <<= 1;
		tx_history |= 1;

		tx_last_status = RADIO_TX_SUCCESS;
	}
	else if (status & _BV(MAX_RT))//Максимальное количество попыток передать пакет исчерпано, пакет не передан.
	{
		//очищаем буфер данных передатчика
		send_instruction(FLUSH_TX, NULL, NULL, 0);

		transmit_lock = 0;
		//reset_pipe0_address();
		//set_mode(RADIO_PRIM_RX);//??????
		//set_rx_mode();
		// indicate in the history that a packet was dropped by appending a 0.
		tx_history <<= 1;

		tx_last_status = RADIO_TX_MAX_RT;
	}

	// Очищаем флаги прерываний в STATUS регистре
	status = _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT);
	set_register(STATUS, &status, 1);
	CE_HIGH();
}
/*
void tx_Delay(char tx_delay_set)
{
	while (tx_delay_set != 0)
	{
		_delay_ms(1);
		tx_delay_set--;
	}
}
*/
void Check_Receive_NRF24L01()
{
	char message_char;
	
	// The rxflag is set by radio_rxhandler function below indicating that a
	// new packet is ready to be read.
	//Если поступили новые данные
	if (rxflag==1)
	{
		rx_status = Radio_Receive(&packet); // Copy received packet to memory and store the result in rx_status.
		if (rx_status == RADIO_RX_SUCCESS || rx_status == RADIO_RX_MORE_PACKETS) // Check if a packet is available.
		{
			//digitalWrite(LED_PIN, HIGH); // Turn on the led.
			
			if (packet.type != MESSAGE)
			{
				//snprintf(output, sizeof(output), "Error: wrong packet type: %d. Should be %d\n\r", packet.type, MESSAGE);
				//printf(output);
				send_Uart_str("не сообщение");
				send_Uart(13);
			}

			// Print out the message, along with the message ID and sender address.
// 			
// 			snprintf(output, sizeof(output), "Message ID %d from 0x%.2X%.2X%.2X%.2X%.2X: '%s'\n\r",
// 			packet.payload.message.messageid,
// 			packet.payload.message.address[0],
// 			packet.payload.message.address[1],
// 			packet.payload.message.address[2],
// 			packet.payload.message.address[3],
// 			packet.payload.message.address[4],
// 			packet.payload.message.messagecontent);
// 			//Serial.print(output);
// 			printf(output);
// 			
			//send_Uart_str(packet.payload.message.messagecontent);
			//message_unit[0].ch_message=packet.payload.message.messagecontent
			//message_char=message_unit[0].message.;
			//TX_to_RX_command(&packet.payload.message.messagecontent, &message_unit[0]);
			snprintf((char*)packet.payload.message.messagecontent, sizeof(packet.payload.message.messagecontent), TX_to_RX_command(&packet.payload.message.messagecontent, &message_unit[0]));
			//snprintf((char*)packet.payload.message.messagecontent, sizeof(packet.payload.message.messagecontent), "Ответ Ответ Ответ Ответ 26");
			send_instruction(W_ACK_PAYLOAD, &packet, NULL, sizeof(packet));
			/*
			switch (message_char)
			{
// 				case 0x00:
// 				break;
				
				
				case 0x54:
				send_Uart_str("ВЕРНО");
				send_Uart_str(packet.payload.message.messagecontent);
				
				snprintf((char*)packet.payload.message.messagecontent, sizeof(packet.payload.message.messagecontent), "Ответ Ответ Ответ Ответ 26");
				send_instruction(W_ACK_PAYLOAD, &packet, NULL, sizeof(packet));
				break;
				
				default:
				send_Uart_str(packet.payload.message.messagecontent);
				//printf("Message:  %s", packet.payload.message.messagecontent);
				break;
			}
				
				*/
/*Ручной ответ использовать его мы не будем!*/
/*			
				//snprintf(output, sizeof(output), packet.payload.message.messagecontent);
				//printf(output);
			// Use the commented line below to set the transmit address to the one specified in the received message packet.
			// Radio_Set_Tx_Addr(packet.payload.message.address);
			//
			Radio_Set_Tx_Addr(packet.payload.message.address);  // or use the address manually informed by trans_addr.
			// Reply to the sender by sending an ACK packet, reusing the packet data structure.
			packet.type = ACK;
			
			// Se the ack message id:
			packet.payload.ack.messageid = packet.payload.message.messageid;
			

			if (Radio_Transmit(&packet, RADIO_WAIT_FOR_TX) == RADIO_TX_MAX_RT)
			{
				// If the max retries was reached, the packet was not acknowledged.
				// This usually occurs if the receiver was not configured correctly or
				// if the sender didn't copy its address into the radio packet properly.
				//snprintf(output, sizeof(output), "Could not reply to sender.\n\r");
				//Serial.print(output);
				//printf(output);
				//send_Uart_str("NRTS :(");
				//send_Uart(13);
			}
			else
			{
				// the transmission was completed successfully
				//snprintf(output, sizeof(output), "Replied to sender.\n\r");
				//Serial.print(output);
				//printf(output);
				//send_Uart_str("RTS :)");
				//send_Uart(13);
				//digitalWrite(LED_PIN, LOW); // turn off the led.
			}
			/**/
		}
		rxflag = 0;  // clear the flag.
	}
} // End of main loop.
void setup_NRF24L01_Receiver(uint8_t *message_addr)
{
	// 	pinMode(LED_PIN, OUTPUT);
	// 	// start the serial output module at 57600 bps
	// 	Serial.begin(57600);
	
	// initialize the radio, including the SPI module
	Radio_Init();				//Уже в режиме приемника
	
	// configure the receive settings for radio pipe 0
	Radio_Configure_Rx(RADIO_PIPE_0, message_addr, ENABLE);
	
	// configure radio transceiver settings.
	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);
	
	snprintf((char*)packet.payload.message.messagecontent, sizeof(packet.payload.message.messagecontent), "Ответ Ответ Ответ О21111");
	send_instruction(W_ACK_PAYLOAD, &packet, NULL, sizeof(packet));
	
	// print a message to UART to indicate that the program has started up
	snprintf(output, sizeof(output), "STATION START IN RECEIVER\n\r");
	printf(output);
}
void setup_NRF24L01_Transmitter(uint8_t *message_addr)
{
	// 	pinMode(LED_PIN, OUTPUT);
	// 	// start the serial output module at 57600 bps
	// 	Serial.begin(57600);
	
	// initialize the radio, including the SPI module
	Radio_Init();				//переключили в режим приемника
	
	 // configure the receive settings for radio pipe 0
	 Radio_Configure_Rx(RADIO_PIPE_0, message_addr, ENABLE);
	 // configure radio transceiver settings.
	 Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

	 // load up the packet contents
	 packet.type = MESSAGE;
	 memcpy(packet.payload.message.address, message_addr, RADIO_ADDRESS_LENGTH);
	 packet.payload.message.messageid = 55;
	 snprintf((char*)packet.payload.message.messagecontent, sizeof(packet.payload.message.messagecontent), "Test message.");

	 //Radio_Set_Tx_Addr(station_addr);
	 //digitalWrite(LED_PIN, HIGH);
 	 //Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);
	 if (Radio_Transmit(&packet, message_addr, RADIO_WAIT_FOR_TX) == RADIO_TX_MAX_RT)
	 {
		 // If the max retries was reached, the packet was not acknowledged.
		 // This usually occurs if the receiver was not configured correctly or
		 // if the sender didn't copy its address into the radio packet properly.
		 snprintf(output, sizeof(output), "Could not reply to sender.\n\r");
		 //Serial.print(output);
		 printf(output);
		 //send_Uart_str("NRTS :(");
		 //send_Uart(13);
	 }
	 else
	 {
		 // the transmission was completed successfully
		 snprintf(output, sizeof(output), "Replied to sender.\n\r");
		 //Serial.print(output);
		 printf(output);
		 //send_Uart_str("RTS :)");
		 //send_Uart(13);
		 //digitalWrite(LED_PIN, LOW); // turn off the led.
	 }
 //	 _delay_ms(10);
//printf("закончили настройку. \n\r");
}

/*
 * Имя                   :  IR_Check_NEC()
 * Описание              :  Производит запись полученных бит с ИК датчика и проверку на принадлежность их протоколу NEC
 * Аргумент(ы)           :  нет
 * Возвращаемое значение :  Сохраняет успешно принятую посылку в структуре "IRC_message" и возвращяет код нажатой кнопки
 */
void Get_Transmit_NRF24L01(unsigned char *message_str, uint8_t *message_addr)
{
	//printf("отправляем 2. \n\r");
	//digitalWrite(LED_PIN, HIGH);
	// load up the packet contents again because the packet was used as an ACK packet before.
	packet.type = MESSAGE;
	memcpy(packet.payload.message.address, message_addr, RADIO_ADDRESS_LENGTH);
	packet.payload.message.messageid = 55;
	//packet.payload.message.messageid ++;
	snprintf((char*)packet.payload.message.messagecontent, sizeof(packet.payload.message.messagecontent), message_str);
	//printf("отправляем. \n\r");
	if (Radio_Transmit(&packet, message_addr, RADIO_WAIT_FOR_TX) == RADIO_TX_MAX_RT) // Transmitt packet.
	{
		//Serial.println("Data not trasmitted. Max retry.");
		//printf("DNT_MR. \n\r");
		// send_Uart_str("DNT MR :(");
		// send_Uart(13);
	}
	else // Transmitted succesfully.
	{
		//Serial.println("Data trasmitted.");
		//printf("DT. \n\r");
		//send_Uart_str("DT :)");
		//send_Uart(13);
	}
	
	// The rxflag is set by radio_rxhandler function below indicating that a
	// new packet is ready to be read.
	if (rxflag==1)	//Если поступили новые данные
	{
		//printf("новые данные. \n\r");
		//if (Radio_Receive(&packet) == RADIO_RX_MORE_PACKETS) // Receive packet.
		//{
			//printf("есть пакет \n\r");
			// if there are no more packets on the radio, clear the receive flag;
			// otherwise, we want to handle the next packet on the next loop iteration.
			//rxflag = 0;
			//send_Uart_str(packet.payload.message.messagecontent);
			//			send_Uart_str("R :(");
			//			send_Uart(13);
		//}
		if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS) // Receive packet.
		{
			//printf("нет пакета \n\r");
			// if there are no more packets on the radio, clear the receive flag;
			// otherwise, we want to handle the next packet on the next loop iteration.
			rxflag = 0;
			//printf(packet.payload.message.messagecontent);
			send_Uart_str(packet.payload.message.messagecontent);
			//			send_Uart_str("R :(");
			//			send_Uart(13);
		}
		if (packet.type == ACK)
		{
			//send_Uart_str("R :)");
			//send_Uart(13);
			//digitalWrite(LED_PIN, LOW); // Turn off the led.
		}
	}
}
#endif