#pragma once
#ifndef _WRITABLE_H_
#define _WRITABLE_H_

#include <stdint.h>

class Writable
{
	public:
		~Writable() {}
		virtual bool writeByte(uint8_t byte) = 0;
		virtual uint8_t write(uint8_t* bytes, uint8_t length) = 0;
		virtual uint8_t write(const char* string) = 0;
		virtual uint8_t write(int value) = 0;
		virtual uint8_t availableForWrite() = 0;
};

#endif // !_TRANSMITTER_H_
