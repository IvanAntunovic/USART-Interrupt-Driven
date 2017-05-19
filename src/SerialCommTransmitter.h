#pragma once
#ifndef  _SERIALCOMMTRANSMITTER_H_
#define  _SERIALCOMMTRANSMITTER_H_

#include "CircularQueue.h"
#include "Writable.h"
#include <stdlib.h>


class SerialCommTransmitter : public Writable
{
	private:
		CircularQueue txQueue;

	public:
		bool writeByte(uint8_t byte);
		uint8_t write(uint8_t* bytes, uint8_t length);
		uint8_t write(const char* string);
		uint8_t write(int value);
		uint8_t availableForWrite();
		friend class SerialPort;
};

#endif // !_SERIALCOMMTRANSMITTER_H_




