#include "SerialPort.h"

#ifndef F_CPU
#define		F_CPU	1000000UL
#endif

#if defined(__AVR_ATmega324p__)
	#define		F_CPU	1000000UL
#endif

#if defined(__AVR_ATmega2560__)
	#define			F_CPU	16000000UL
#endif

#if (BUFFER_SIZE & BUFFER_MASK)
	#error RX buffer size is not a power of 2
#endif 
#if (BUFFER_SIZE & BUFFER_MASK)
	#error TX buffer size is not a power of 2
#endif

void SerialPort::open(uint16_t baudRate, uint8_t portSelected)
{
	uint16_t baud_setting = (F_CPU / 4 / baudRate - 1) / 2;
	
	this->portSelected = portSelected;
	this->serialTransmitter = new SerialCommTransmitter(portSelected);
	this->serialReceiver = new SerialCommReceiver(portSelected);
	
	if (portSelected == 0)
	{
		/* Initialize USART0 */
			
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
		/* Initialize USART1 */
		
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

int8_t SerialPort::getRxInterruptStatusCode(void)
{
	return this->serialReceiver->getInterruptStatusCode();
}

unsigned char SerialPort::available()
{
	return SerialPort::serialReceiver->available();
}

unsigned char SerialPort::availableForWrite()
{
	return SerialPort::serialTransmitter->availableForWrite();
}

int8_t SerialPort::readBytes(uint8_t* buffer, uint8_t length)
{
	return SerialPort::serialReceiver->readBytes(buffer, length);
}

int8_t SerialPort::writeBytes(uint8_t* bytes, uint8_t length)
{
	return SerialPort::serialTransmitter->writeBytes(bytes, length);
}

int8_t SerialPort::writeString(const char* string)
{
	uint8_t bytesWritten;
	
	for (bytesWritten = 0; *string; bytesWritten++)
	{
		this->serialTransmitter->writeByte(*string++);
	}
	return bytesWritten;
}

int8_t SerialPort::write(int integerVal)
{
	uint8_t numberOfDigits;
	uint8_t* buffer = NULL;
	int tempValue;
	int8_t retVal = -1;
	
	tempValue = integerVal;
	for (numberOfDigits = 0; tempValue != 0; numberOfDigits++)
	{
		tempValue = tempValue / 10;
	}
	
	if (integerVal < 0)
	{
		// We need to allocate extra byte for '-' sign
		numberOfDigits++;	
	}

	buffer = new uint8_t[numberOfDigits + 1];
	if (buffer != NULL)
	{
		itoa(integerVal, (char*)buffer, 10);
		memset(&buffer[numberOfDigits + 1], '\0', 1);
		retVal = writeBytes(buffer, numberOfDigits);
		delete[] buffer;
	}
	return retVal;
}
