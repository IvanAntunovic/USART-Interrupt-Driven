#include "SerialPort.h"

#ifndef F_CPU
#define		F_CPU	1000000UL
#endif
#if defined(__AVR_ATmega324p__)
		#define		F_CPU	1000000UL
//#elif defined(__AVR_ATmega2560__)
	//#ifndef			F_CPU
		//#define			F_CPU	16000000UL

#endif

#if (BUFFER_SIZE & BUFFER_MASK)
	#error RX buffer size is not a power of 2
#endif 
#if (BUFFER_SIZE & BUFFER_MASK)
	#error TX buffer size is not a power of 2
#endif

SerialPort* SerialPort::sInstance[] = {NULL};

void SerialPort::open(uint16_t baudRate, uint8_t portSelected)
{
	uint16_t baud_setting = (F_CPU / 4 / baudRate - 1) / 2;
	
	this->portSelected = portSelected;
	this->serialTransmitter = new SerialCommTransmitter(portSelected);
	this->serialReceiver = new SerialCommReceiver();
	
	if (portSelected == 0)
	{
		SerialPort::sInstance[portSelected] = this;

		/* Initialize USART */
			
		/* Set the baud rate */
		UBRR0H = (unsigned char) (baud_setting >> 8);
		UBRR0L = (unsigned char) baud_setting;
			
		/* Enable USART receiver and transmitter */
		UCSR0B = ((1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0));
			
		/* Double the USART Transmission Speed */
		UCSR0A = (1 << U2X0);
			
		/* Set frame format: 8 data bits 1 stop bit */
		UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	}
	else if (portSelected == 1)
	{
		SerialPort::sInstance[portSelected] = this;

		/* Initialize USART */
		
		/* Set the baud rate */
		UBRR1H = (unsigned char) (baud_setting >> 8);
		UBRR1L = (unsigned char) baud_setting;
		
		/* Enable USART receiver and transmitter */
		UCSR1B = (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);
		
		/* Double the USART Transmission Speed */
		UCSR1A = (1 << U2X1);
		
		/* Set frame format: 8 data bits 1 stop bit */
		UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
	}
	sei();
}

void SerialPort::close()
{
	delete this->serialTransmitter;
	delete this->serialReceiver;
	
	if (this->portSelected == 0)
	{
		UCSR0B = ~((1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0));
	}
	else if (this->portSelected == 1)
	{
		UCSR1B &= ~((1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1));
	}
}

unsigned char SerialPort::available()
{
	return SerialPort::serialReceiver->available();
}

unsigned char SerialPort::availableForWrite()
{
	return SerialPort::serialTransmitter->availableForWrite();
}

uint8_t SerialPort::readBytes(uint8_t* buffer, uint8_t length)
{
	return SerialPort::serialReceiver->readBytes(buffer, length);
}

uint8_t SerialPort::writeBytes(uint8_t* bytes, uint8_t length)
{
	uint8_t bytesWritten;
	
	bytesWritten = 0;
	bytesWritten = SerialPort::serialTransmitter->write(bytes, length);
	
	return bytesWritten;
}

uint8_t SerialPort::write(const char* string)
{
	return serialTransmitter->write(string);
}

uint8_t SerialPort::write(int value)
{
	return serialTransmitter->write(value);
}

void SerialPort::interrupt_UDRE0_Handler(void)
{
	uint8_t tempByte;
	
	//Check if there is available data to transmit
	if (serialTransmitter->txQueue.dequeue(&tempByte))
	{
		UDR0 = tempByte;
	}
	else
	{
		//If all data is transmitted 
		//Disable USART Data Register Interrupt
		//If we do not disable ISR, the program will keep jumping in this routine
		UCSR0B &= ~(1<<UDRIE0);
	}
}

void SerialPort::interrupt_RX0_Handler(void)
{
	uint8_t readByte;
	
	readByte = UDR0;
	if (!this->serialReceiver->rxQueue.isFull())
	{
		// add new item to queue
		if (!this->serialReceiver->rxQueue.enqueue(readByte))
		{
			// Flag ERROR - receive queue overflow!
		}
		//xSemaphoreGiveFromISR(this->serialReceiver.rxBinarySemaphore, NULL);
	}
	else
	{
		//Handle buffer overflow
	}
}

void SerialPort::interrupt_UDRE1_Handler(void)
{
	uint8_t tempByte;
	
	//Check if there is available data to transmit
	if (serialTransmitter->txQueue.dequeue(&tempByte))
	{
		UDR1 = tempByte;
	}
	else
	{
		//If all data is transmitted 
		//Disable USART Data Register Interrupt
		//If we do not disable ISR, the program will keep jumping in this routine
		UCSR1B &= ~(1<<UDRIE1);
	}
}

void SerialPort::interrupt_RX1_Handler(void)
{
	uint8_t readByte;
	
	readByte = UDR1;
	//Check if queue is not full
	if (!this->serialReceiver->rxQueue.isFull())
	{
		// add new item to the queue
		if (!this->serialReceiver->rxQueue.enqueue(readByte))
		{
			// Flag ERROR - receive queue overflow!
		}
		//xSemaphoreGiveFromISR(this->serialReceiver.rxBinarySemaphore, NULL);
	}
}

ISR(USART0_UDRE_vect)
{
	SerialPort::sInstance[0]->interrupt_UDRE0_Handler();
}

ISR(USART0_RX_vect)
{
	SerialPort::sInstance[0]->interrupt_RX0_Handler();
}

ISR(USART1_UDRE_vect)
{
	SerialPort::sInstance[1]->interrupt_UDRE1_Handler();
}

ISR(USART1_RX_vect)
{
	SerialPort::sInstance[1]->interrupt_RX1_Handler();
}