#pragma once
#ifndef   _SERIALPORT_H_
#define	  _SERIALPORT_H_

#include "SerialCommReceiver.h"
#include "SerialCommTransmitter.h"
#include <avr/interrupt.h>


#if defined (__cplusplus)
extern "C" {
	void UART0_UDRE_vect (void) __attribute__ ((signal));
}
# else
	 extern "C" UART0_UDRE_vect((__signal__,__INTR_ATTRS))
#endif  /* __cplusplus */

extern "C" void UART0_UDRE_vect (void) __attribute__ ((signal, __INTR_ATTRS));
extern "C" void USART0_RX_vect(void) __attribute__((signal, __INTR_ATTRS));

class SerialPort
{
	private:
		unsigned int baudRate;
		SerialCommReceiver serialReceiver;
		SerialCommTransmitter serialTransmitter;
	public:
		static SerialPort* serialPortPointer;

	public:
		SerialPort(uint16_t baudRate);
		
		// Interrupt handling
		friend void UART0_UDRE_vect(void);
		friend void USART0_RX_vect(void);
		void interruptUDREHandler(void);
		void interruptRXHandler(void);
		
		// Methods for input stream
		uint8_t readBytes(uint8_t* buffer, uint8_t length);
		
		// Methods for output stream
		uint8_t writeBytes(uint8_t* bytes, uint8_t length);
		uint8_t write(const char* string);
		uint8_t write(int value);
		uint8_t availableForWrite();
		uint8_t available();
		
};

#endif