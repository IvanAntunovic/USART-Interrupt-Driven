/*
 * LCD_I2C_4bit.cpp
 *
 * Created: 5/20/2017 11:51:13 AM
 * Author : fairenough
 */ 

#define PCF8574A_ADDRESS	0x3F
#include <avr/io.h>
#include <string.h>

#include "SerialPort.h"

int main(void)
{
	SerialPort serialPort;
	SerialPort serialPorta;
	serialPort.open(9600, 0);
	DDRA = 0xFF;
	int8_t retVal;

	uint8_t buffer[] = {'A', 'B', '\n', '\r'};
	uint8_t rxBuffer[4];
	while (1)
	{	
		//serialPort.writeString("hello\n\r");
	//	serialPort.write(0);
	//	serialPort.writeString("\n\r");
		if (serialPort.available())
		{
			retVal = serialPort.readBytes(rxBuffer, 10);
			serialPort.writeString("ReadBytes return value: ");
			serialPort.write(retVal);
			serialPort.writeString("\n\r");
			serialPort.writeString("Rx Interrupt Status Code: ");
			serialPort.write(serialPort.getRxInterruptStatusCode());
			serialPort.writeString("\n\r");
			retVal = serialPort.writeBytes(rxBuffer, 10);
			serialPort.writeString("\n\r");
			serialPort.writeString("WriteBytes return value: ");
			serialPort.write(retVal);
			serialPort.writeString("\n\r");
		}
	}
}

