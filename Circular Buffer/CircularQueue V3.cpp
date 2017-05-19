// CircularBufferV5.cpp : Defines the entry point for the console application.
// Circular queue size must be power of two
// To distinquish between full and empty queue one element of queue is never used to store new element
// So effective queue storage size is BUFFER_SIZE - 1
// Author: Ivan Antunovic


#include "stdafx.h"
#include <iostream>

using namespace std;

#define BUFFER_SIZE 8 
#define BUFFER_MASK (BUFFER_SIZE - 1)

static unsigned char UDR1;
bool isInterruptEnabled = false;

/*
unsigned char USART1_Receive(void)
{
unsigned char tmptail;

/* Wait for incoming data */
//while (USART_RxHead == USART_RxTail);
/* Calculate buffer index */
//tmptail = (USART_RxTail + 1) & USART_RX_BUFFER_MASK;
/* Store new index */
//USART_RxTail = tmptail;
/* Return data */
//return USART_RxBuf[tmptail];
//}
/*
void USART1_Transmit(unsigned char data)
{
unsigned char tmphead;
*/
/* Calculate buffer index */
//tmphead = (USART_TxHead + 1) & USART_TX_BUFFER_MASK;
/* Wait for free space in buffer */
//while (tmphead == USART_TxTail);
/* Store data in buffer */
//USART_TxBuf[tmphead] = data;
/* Store new index */
//USART_TxHead = tmphead;
/* Enable UDRE interrupt */
//UCSR1B |= (1 << UDRIE);
//}


class CircularBuffer
{
private:
	int availableItems;
public:
	int head;
	int tail;
	unsigned char buffer[BUFFER_SIZE];
	int dequeuedElement;

	friend void RX_ISR();
	friend void TX_ISR();

	CircularBuffer()
	{
		head = 0;
		tail = 0;
		dequeuedElement = 0;
		availableItems = 0;
		for (int i = 0; i < BUFFER_SIZE; ++i)
		{
			buffer[i] = 0;
		}
	}

	bool isAvailable()
	{

		if (head - tail)
		{
			return true;
		}
		return false;
	}

	int getAvailableElements()
	{
		int size;
		if (head == 0 && tail == 0)
			size = 0;
		else
			size = head > tail ? (BUFFER_SIZE - head + tail) : (tail - head);

		return size;
	}

	bool isEmpty()
	{
		if (head - tail == 0)
		{
			return true;
		}
		return false;
	}

	bool isFull(int tempTail)
	{

		if (this->head - tempTail)
		{
			return false;
		}
		return true;
	}

	bool enqueue(int element)
	{
		int tempTail = (tail + 1) & BUFFER_MASK;
		if (isFull(tempTail))
		{
			return false;
		}
		tail = tempTail;
		buffer[tail] = element;
		return true;
	}

	bool dequeue(unsigned char* byte)
	{
		if (isEmpty())
		{
			return false;
		}
		head = (head + 1) & BUFFER_MASK;
		*byte = buffer[head];
		return true;
	}
};

/*
void RX_ISR(CircularBuffer& rxQueue)
{
unsigned char data;
unsigned char tmphead;

// Read the received data
data = UDR1;
rxQueue.enqueue(data);
}
*/

void RX_ISR(CircularBuffer& rxQueue)
{
	int tempTail = (rxQueue.tail + 1) & BUFFER_MASK;
	unsigned char* byte = (unsigned char*)malloc(sizeof(unsigned char));
	if (!rxQueue.isFull(tempTail))
	{
		if (rxQueue.dequeue(byte))
		{
			UDR1 = *byte;
		}
	}
}


void UDRE_ISR(CircularBuffer& txQueue)
{
	unsigned char* byte = (unsigned char*)malloc(sizeof(unsigned char)); // je li ova varijabla treba biti static volatile ?
						 /* Check if all data is transmitted */
	if (txQueue.dequeue(byte))
	{
		UDR1 = *byte;
	}
	else
	{
		isInterruptEnabled = false;
	}
	
}


class SerialCommuncation
{
public:

	/*
	unsigned char* read(CircularBuffer& queue)
	{
	int numAvailableElems = queue.getAvailableElements();
	unsigned char* buffer = (unsigned char*)calloc(numAvailableElems, sizeof(unsigned char));

	for (int i = 0; i < numAvailableElems; ++i)
	{
	queue.dequeue();
	*buffer++ = queue.dequeuedElement;
	}
	*buffer = NULL;
	return (buffer - numAvailableElems);
	}
	*/

	bool readByte(CircularBuffer& queue, unsigned char* byte)
	{
		if (!queue.isEmpty())
		{
			queue.dequeue(byte);
			return true;
		}
		return false;
	}

	unsigned char* read(CircularBuffer& queue)
	{
		unsigned char bytesWrittenCnt = 0;
		unsigned char* byte = (unsigned char*)malloc(sizeof(unsigned char));

		// PROBLEM: ne znamo koliko cemo dugo cekati na read jer se kroz citanje može konstantno dešavati 
		// receive interrupt, te stoga ne znamo koliko veliki buffer moramo alocirati
		unsigned char rxBuffer[BUFFER_SIZE];

		while (readByte(queue, byte))
		{
			rxBuffer[bytesWrittenCnt++] = *byte;
		}

		return rxBuffer;
	}


	bool writeByte(CircularBuffer& txQueue, unsigned char byte)
	{
		// wait untill all data is sent to the buffer
		// Problem: što ako je RED popunjen, a svi bytovi nisu writeani
		//while (!txQueue.isEmpty());

		if (txQueue.enqueue(byte))
			return true;
		return false;

	}

	int write(CircularBuffer& txQueue, unsigned char* bytes)
	{
		int bytesWrittenCnt = 0;
		// wait untill all data is send to the buffer
		// Problem: što ako je RED popunjen, a svi bytovi nisu writeani
		isInterruptEnabled = false;
		while (bytes[bytesWrittenCnt] != NULL)
		{
			if (writeByte(txQueue, bytes[bytesWrittenCnt]))
				bytesWrittenCnt++;
		}

		isInterruptEnabled = true;
		return bytesWrittenCnt;
	}

	void println(CircularBuffer& queue, unsigned char* bytes)
	{
		while (*bytes != NULL)
		{
			cout << *bytes++;
		}
		cout << endl;
	}
};


int main()
{
	CircularBuffer rxQueue;
	CircularBuffer txQueue;
	SerialCommuncation serial;

	int buffer[BUFFER_SIZE] = { 0 }, i = 0;

	rxQueue.enqueue('1');
	rxQueue.enqueue('2');
	rxQueue.enqueue('3');
	rxQueue.enqueue('4');
	rxQueue.enqueue('5');
	rxQueue.enqueue('6');
	rxQueue.enqueue('7');
	rxQueue.enqueue('8');
	rxQueue.enqueue('9');

	if (rxQueue.isAvailable())
	{
		rxQueue.getAvailableElements();
		std::cout << "There are elements available in the buffer" << std::endl;
	}

	serial.println(rxQueue, serial.read(rxQueue));
	// Moramo indicirati kraj buffera ? 
	unsigned char txBuffer[BUFFER_SIZE] = { '1' , '2' , '3' , '4', '5', '6', '7', NULL };

	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	serial.write(txQueue, txBuffer);

	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	//serial.write(txQueue, (unsigned char*)'1');
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	//serial.write(txQueue, (unsigned char*)'2');
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);
	if (isInterruptEnabled)
		UDRE_ISR(txQueue);


	return 0;
}
