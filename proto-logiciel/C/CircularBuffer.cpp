#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(int n,void(*_f)(void*)){
        DEBUG_ENTERING();
        maxSize=1<<n;
        DEBUG_PRINT(("Maximum number of elements: %d\n",maxSize));
        size=0;
        buffer=(void**)malloc(maxSize*sizeof(void*));
        cb_free=_f;
        head=0;
        tail=0;
        pthread_mutex_init(&mtx,NULL);
        DEBUG_LEAVING();
}

CircularBuffer::~CircularBuffer(){
        DEBUG_ENTERING();
        if(cb_free!=NULL)
                for(int i=0;i<size;i++)
                        cb_free(get(i));
        free(buffer);
        DEBUG_LEAVING();
}

/*Empty ascending*/
void CircularBuffer::push(void * const toPush){
        DEBUG_ENTERING();
        pthread_mutex_lock(&mtx);
        /*free the old data?*/
        if(cb_free!=NULL&&size==maxSize)
                cb_free(buffer[head]);
        else
                size++;
        buffer[head]=toPush;
        head=(maxSize-1)&(head+1);
        DEBUG_PRINT(("Number of elements in buffer: %d\n",size));
        DEBUG_PRINT(("Position of head: %d\n",head));

        pthread_mutex_unlock(&mtx);
        DEBUG_LEAVING();
}


void *CircularBuffer::get(const int idx){
        void *ptr; 
        pthread_mutex_lock(&mtx);
        if(idx<size){
                ptr=buffer[(head-idx-1)&(maxSize-1)];
        }else{
                ptr=NULL;
        }
        pthread_mutex_unlock(&mtx);
        return ptr;
}

int CircularBuffer::getSize(){
        DEBUG_ENTERING();
        DEBUG_PRINT(("Number of elements in buffer: %d\n",size));
        DEBUG_LEAVING();
        return size;
}

