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
	SerialPort serialPort(9600);
	sei();
	unsigned char readBuffer[100];
	uint8_t bytesRead;
	
	while (true)
	{
		if (serialPort.available())
		{
			bytesRead = serialPort.readBytes(readBuffer, 10);
			serialPort.writeBytes(readBuffer, bytesRead);
		}
	}
}

