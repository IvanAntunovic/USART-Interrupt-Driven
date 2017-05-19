#pragma once
#ifndef _CIRCULARQUEUE_H_
#define _CIRCULARQUEUE_H_

#define BUFFER_SIZE 64
#define BUFFER_MASK (BUFFER_SIZE - 1)

class CircularQueue
{
	private:
		volatile unsigned char head;
		volatile unsigned char tail;
		volatile unsigned char buffer[BUFFER_SIZE];
		volatile unsigned char availableItems;

	public:
		CircularQueue();
		bool isAvailable();
		unsigned char getAvailableItems();
		bool isEmpty();
		bool isFull();
		bool enqueue(unsigned char element);
		bool dequeue(unsigned char* byte);
};

#endif // DEBUG

