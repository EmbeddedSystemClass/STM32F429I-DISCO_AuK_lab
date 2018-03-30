/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"


bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
	  //TODO
	  ringBuffer->data = dataBuffer;
	  ringBuffer->size = dataBufferSize;
	  ringBuffer->head = 0;
	  ringBuffer->tail = 0;
	 
	  return true;
	}
	
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	 ringBuffer->head = 0;
	 ringBuffer->tail = 0;
	     if(ringBuffer->head ==0 && ringBuffer->tail ==0)
	       {
	          return true; 
	       }
	    
	 
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  assert(ringBuffer);	
	//TODO
	if(RingBuffer_GetLen(ringBuffer)==0)
	{
	    return true;
	}
	else
	{
	    return false;
	}
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	    if(ringBuffer->head>=ringBuffer->tail)
	    {
	        
	        return (ringBuffer->head-ringBuffer->tail);   
	       
	    }
		else
		{
		    return (ringBuffer->head + ringBuffer->tail);
		}
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	    return ringBuffer->size;
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	
		if(RingBuffer_GetLen(ringBuffer)>= ringBuffer->size)
		{
		  
		   return false;
		}
		else
		{
		    if (ringBuffer->head>=ringBuffer->size)
		    {
		        ringBuffer->head = 0;
		    }
		    
		    ringBuffer->data[ringBuffer->head] = c;
		    ringBuffer->head +=1;
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
     
        if(RingBuffer_IsEmpty(ringBuffer)==1)
        {
	      return false;
        }
        else
        {
        
        if (ringBuffer->tail>=ringBuffer->size)
		    {
		        ringBuffer->tail = 0;
		    }
         *c=ringBuffer->data[ringBuffer->tail];
	      ringBuffer->tail +=1;
          return true;  
        }
		
	}
	return false;
}
