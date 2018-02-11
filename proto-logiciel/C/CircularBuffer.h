#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <iostream>    
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
extern "C" {
#include "debug_utils.h"
}
class CircularBuffer{
        private:
                void **buffer;
                void (*cb_free)(void*);
                int head;
                int tail;
                int maxSize;
                int size;
                pthread_mutex_t mtx;
        public:
                /*Size of buffer is 2^n*/
                /*the function pointer is a function to free the overwritten data*/
                CircularBuffer(int n,void(*f)(void*));
                ~CircularBuffer();
                void push(void* const toPush);
                /*Get element at given index without removing it*/
                void *get(const int idx);
                int getSize();
};

#endif
