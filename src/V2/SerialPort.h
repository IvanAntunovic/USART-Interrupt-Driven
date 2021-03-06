#pragma once
#ifndef   _SERIALPORT_H_
#define	  _SERIALPORT_H_

#include "SerialCommReceiver.h"
#include "SerialCommTransmitter.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

//#if defined (__cplusplus)
//extern "C" {
	//void UART0_UDRE_vect (void) __attribute__ ((signal));
	//void UART1_UDRE_vect (void) __attribute__ ((signal));
//}
//# else
	 //extern "C" UART0_UDRE_vect((__signal__,__INTR_ATTRS));
	 //extern "C" UART1_UDRE_vect((__signal__,__INTR_ATTRS))
//#endif  /* __cplusplus */


class SerialPort
{
	private:
		SerialCommReceiver* serialReceiver;
		SerialCommTransmitter* serialTransmitter;
		uint8_t portSelected;

	public:
		void open(uint16_t baudRate, uint8_t portSelected);
		void close();
		int8_t getRxInterruptStatusCode(void);
		// Methods for input stream
		uint8_t available();
		int8_t readBytes(uint8_t* buffer, uint8_t length);
		// Methods for output stream
		int8_t writeBytes(uint8_t* bytes, uint8_t length);
		int8_t writeString(const char* string);
		int8_t write(int value);
		uint8_t availableForWrite();
	private:
		void integerToArray();
};

#endif