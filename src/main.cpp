/*
 * SerialCommV4.cpp
 *
 * Created: 3/6/2017 7:11:35 PM
 * Author : fairenough
 */ 
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include "avr/io.h"
#include "avr/interrupt.h"
#include <util/delay.h>
#include "SerialPort.h"
#include <stdlib.h>

int main (void)
{
	SerialPort serialPort;
	SerialPort serialPort2;
	sei();
	unsigned char readBuffer[20];
	uint8_t bytesRead;
	
	serialPort.open(9600, 0);
	serialPort2.open(9600, 1);
	while (true)
	{
		if (serialPort2.available())
		{
			uint8_t writeByte;
			serialPort2.readBytes(&writeByte, 1);
			serialPort2.writeBytes(&writeByte, 1);
			if (writeByte == '0')
			{
				serialPort2.close();
			}
		}
		
		if (serialPort.available())
		{
			uint8_t writeByte;
			serialPort.readBytes(&writeByte, 1);
			serialPort.writeBytes(&writeByte, 1);
			if (writeByte == '0')
			{
				serialPort.close();
			}
		}
		
	}
}

