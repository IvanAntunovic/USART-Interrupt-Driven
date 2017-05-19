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


ISR(USART0_UDRE_vect)
{
	SerialPort::serialPortPointer->interruptUDREHandler();
}

ISR(USART0_RX_vect)
{
	SerialPort::serialPortPointer->interruptRXHandler();
}

SerialPort* SerialPort::serialPortPointer = NULL;

SerialPort::SerialPort(uint16_t baudRate)
{
	uint16_t baud_setting = (F_CPU / 4 / baudRate - 1) / 2;
	
	this->baudRate = baudRate;
	serialPortPointer = this;

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

unsigned char SerialPort::available()
{
	return SerialPort::serialReceiver.available();
}

unsigned char SerialPort::availableForWrite()
{
	return SerialPort::serialTransmitter.availableForWrite();
}

void SerialPort::interruptUDREHandler(void)
{
	uint8_t tempByte;
	
	if (serialTransmitter.txQueue.dequeue(&tempByte))
	{
		UDR0 = tempByte;
	}
	else
	{	
		//Disable USART Data Register Interrupt 
		UCSR0B &= ~(1<<UDRIE0);
	}
}


void SerialPort::interruptRXHandler(void)
{
	uint8_t readByte;
	
	readByte = UDR0;
	//Check if queue is not full
	if (!this->serialReceiver.rxQueue.isFull())
	{
		// add new item to queue
		if (!this->serialReceiver.rxQueue.enqueue(readByte))
		{
			// Flag ERROR - receive queue overflow!
		}
		//xSemaphoreGiveFromISR(this->serialReceiver.rxBinarySemaphore, NULL);
	}
}

uint8_t SerialPort::readBytes(uint8_t* buffer, uint8_t length)
{
	return SerialPort::serialReceiver.readBytes(buffer, length);
}

uint8_t SerialPort::writeBytes(uint8_t* bytes, uint8_t length)
{
	uint8_t bytesWritten;
	
	bytesWritten = 0;
	bytesWritten = SerialPort::serialTransmitter.write(bytes, length);
	return bytesWritten;
}

uint8_t SerialPort::write(const char* string)
{
	return serialTransmitter.write(string);
}

uint8_t SerialPort::write(int value)
{
	return serialTransmitter.write(value);
}