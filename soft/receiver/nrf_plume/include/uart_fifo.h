#ifndef UART_FIFO_H__
#define UART_FIFO_H__

/*
 * see 
 *http://stackoverflow.com/questions/6822548/correct-way-of-implementing-a-uart-receive-buffer-in-a-small-arm-microcontroller
 */

/* Buffer read / write macros                                                 */
#define RINGFIFO_RESET(ringFifo)		\
    {ringFifo.rdIdx = ringFifo.wrIdx = 0;}
#define RINGFIFO_WR(ringFifo, dataIn)					\
    {ringFifo.data[ringFifo.mask & ringFifo.wrIdx++] = (dataIn);}
#define RINGFIFO_RD(ringFifo, dataOut)					\
    {									\
	ringFifo.rdIdx++;						\
	dataOut = ringFifo.data[ringFifo.mask & (ringFifo.rdIdx-1)];	\
    }
#define RINGFIFO_EMPTY(ringFifo)		\
    (ringFifo.rdIdx == ringFifo.wrIdx)
#define RINGFIFO_FULL(ringFifo)						\
    ((ringFifo.mask & ringFifo.rdIdx) == (ringFifo.mask & (ringFifo.wrIdx+1)))
#define RINGFIFO_COUNT(ringFifo)			\
    (ringFifo.mask & (ringFifo.wrIdx - ringFifo.rdIdx))

/* buffer type                                                                */

#define RINGFIFO_DECL(__name__, __size__, __data__)		\
    struct {							\
	const uint32_t size;					\
	const uint32_t mask;					\
	uint32_t wrIdx;						\
	uint32_t rdIdx;						\
	__data__ data[__size__];				\
    } __name__ = {						\
	__size__,						\
	__size__ - 1ul,						\
	0,							\
	0,							\
	{0}							\
    }
#endif
