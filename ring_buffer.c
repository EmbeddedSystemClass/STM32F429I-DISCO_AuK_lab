//#include <assert.h>
//#include "ring_buffer.h"
//bool RingBuffer_Init(RingBuffer *ringBuffer, char *_dataBuffer, size_t _dataBufferSize){
//	assert(ringBuffer);
//	assert(_dataBuffer);
//	assert(_dataBufferSize > 0);
//	if ((ringBuffer) && (_dataBuffer) && (_dataBufferSize > 0)) {
//	 ringBuffer->dataBuffer=_dataBuffer;
//	 ringBuffer->dataBufferSize=_dataBufferSize;
//	 ringBuffer->writePos=0;
//	 ringBuffer->readPos=0;
//	 return true;
//	}
//	return false;
//}

//size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer){
//	assert(ringBuffer);
//	if (ringBuffer) return ringBuffer->dataBufferSize;
//	else return 0;
//}

//size_t RingBuffer_GetLen(const RingBuffer *ringBuffer){
//	assert(ringBuffer);
//	return (size_t)ringBuffer->writePos-ringBuffer->readPos;
//}

//bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer){
//  assert(ringBuffer);	
//   if(RingBuffer_GetLen(ringBuffer)==0) return true;
//   else return false;
//}

//bool RingBuffer_Clear(RingBuffer *ringBuffer){
//	assert(ringBuffer);
//	if (ringBuffer) {
//		ringBuffer->writePos=0;
//		ringBuffer->readPos=0;
//		return true;
//	}
//	else return false;
//}

//bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c){
//	assert(ringBuffer);
//	    if(RingBuffer_GetLen(ringBuffer)>= ringBuffer->dataBufferSize)  return false;
//        if (ringBuffer->writePos>=ringBuffer->dataBufferSize) ringBuffer->writePos=0;
//    ringBuffer->dataBuffer[ringBuffer->writePos]=c;
//    ringBuffer->writePos++;
//    return true;
//}

//bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c){
//	assert(ringBuffer);
//	assert(c);
//	if(RingBuffer_IsEmpty(ringBuffer)==1) return false;
//	if (ringBuffer->readPos==ringBuffer->dataBufferSize) ringBuffer->readPos=0;
//	
//	if (ringBuffer->readPos<ringBuffer->dataBufferSize)
//	{   *c=ringBuffer->dataBuffer[ringBuffer->readPos];
//	    ringBuffer->readPos++;
//	    return true;
//	}
//}
/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"


bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
	    ringBuffer->buffer = dataBuffer;    
		ringBuffer->tail = 0;       //head's index (pointer)
		ringBuffer->length = 0;     //amount of data stored
		ringBuffer->buff_size = dataBufferSize;
		return true;
	}
	
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		ringBuffer->tail = 0;      //reset tail index and length
		ringBuffer->length = 0;    //however just setting the length to 0 would be sufficient
		return true;
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  assert(ringBuffer);	
	if(ringBuffer->length != 0){    //just check if there is any data stored
	    return false;
	}
	
	return true;
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		return ringBuffer->length;  //return how much data is stored
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	    return (ringBuffer->buff_size); //return buffer size
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	    if (ringBuffer->length < 100){                  //check for overflow
	        int index = (ringBuffer->tail + ringBuffer->length) % (ringBuffer->buff_size); //calculate the "head" index value
	        ringBuffer->buffer[index] = c;   //put data to the buffer
	        ringBuffer->length++; //increment the lenght of the stored data
	        return true;
	    }
	    
	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);
	
  if ((ringBuffer) && (c)) {
      if (ringBuffer->length > 0){  //check for underflow
        *c = ringBuffer->buffer[ringBuffer->tail]; //store data from buffer's tail to *c
        ringBuffer->tail = (ringBuffer->tail + 1) % (ringBuffer->buff_size); //store new tail value
		ringBuffer->length--; //decrement the length
		return true;
      }
	}
	return false;
}
