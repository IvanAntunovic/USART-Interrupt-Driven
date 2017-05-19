#ifndef  _SERIALCOMMRECEIVER_H_
#define	 _SERIALCOMMRECEIVER_H_

#include <stdlib.h>
#include "Readable.h"
#include "CircularQueue.h"
//#include "FreeRTOS.h"
//#include "semphr.h"

class SerialCommReceiver : public Readable
{
	private:
		CircularQueue rxQueue;
		//SemaphoreHandle_t rxBinarySemaphore;

	public:
		SerialCommReceiver();
		inline bool readByte(uint8_t* byte);
		uint8_t readBytes(uint8_t* buffer, uint8_t length);
		uint8_t available();
		friend class SerialPort;
};

#endif
