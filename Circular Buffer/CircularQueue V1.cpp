// CircularBufferV3.cpp : Defines the entry point for the console application.
// Circular queue size must be power of two
// To distinquish between full and empty queue one element of queue is never used to store new element
// So effective queue storage size is BUFFER_SIZE - 1
// Author: Ivan Antunoviæ


#include "stdafx.h"
#include <iostream>

using namespace std;

#define BUFFER_SIZE 8 
#define BUFFER_MASK (BUFFER_SIZE - 1)

class CircularBuffer
{
	private:
		int availableItems;
	public:
		int head;
		int tail;
		int buffer[BUFFER_SIZE];
		int dequeuedElement;

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

		bool dequeue()
		{
			if (isEmpty())
			{
				return false;
			}
			head = (head + 1) & BUFFER_MASK;
			dequeuedElement = buffer[head];
			return true;
		}
};

class SerialCommuncation
{
	public:

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
	CircularBuffer circularQueue;
	SerialCommuncation serial;

	int buffer[BUFFER_SIZE] = { 0 }, i = 0;

	circularQueue.enqueue('1');
	circularQueue.enqueue('2');
	circularQueue.enqueue('3');
	circularQueue.enqueue('4');
	circularQueue.enqueue('5');
	circularQueue.enqueue('6');
	circularQueue.enqueue('7');
	circularQueue.enqueue('8');
	circularQueue.enqueue('9');

	if (circularQueue.isAvailable())
	{
		circularQueue.getAvailableElements();
		std::cout << "There are elements available in the buffer" << std::endl;
	}

	serial.println(circularQueue, serial.read(circularQueue));

	/*
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	circularQueue.dequeue();
	buffer[i++] = queue.dequeuedElement;
	*/
	return 0;
}

