#include <stdint.h>
#include <stdio.h>
#include "ring_buffer.h"


bool RingBuffer_UnitTest(void)
{
	RingBuffer buffer;
	char dataBuff[100];
	char put, get;
	
	///////////////////////////////////////////////////////////
	printf("Test 1. Checking RingBuffer_Init function... ");

	// Initialize the ring buffer
	if (true == RingBuffer_Init(&buffer, dataBuff, sizeof(dataBuff))) {
		printf("passed\n\r");
	} else {
		printf("FAILED!\n\r");
		printf("ERROR: Unable to initialize the buffer.\n\r");
		
		return false;
	}
	
	///////////////////////////////////////////////////////////
	printf("Test 2. Checking RingBuffer_PutChar...       ");
	
	// try to fill the buffer completely with data
	for (size_t i = 0; i < RingBuffer_GetCapacity(&buffer); i++) {
		put = i % 0x7f;
		if (RingBuffer_PutChar(&buffer, put) == false){
			  printf("FAILED!\n\r");
				printf("ERROR: Unable to put char no %d into buffer.\n\r", i);
				return false;
		}		
	}
	// checking overflow behavior - this call should fail because the buffer is full
  if (RingBuffer_PutChar(&buffer, put) == true) {	
	  printf("FAILED!\n\r");
		printf("ERROR: Overflow not detected.\n\r");
		return false; 
	}
	// if all OK:
	printf("passed\n\r");
	
	///////////////////////////////////////////////////////////
	printf("Test 3. Checking RingBuffer_GetLen...        ");
	
	// at this point RingBuffer_GetLen should return sizeof(dataBuff) because the buffer is full
	if (RingBuffer_GetLen(&buffer) != RingBuffer_GetCapacity(&buffer)) {
	  printf("FAILED!\n\r");
		printf("ERROR: Wrong return value.\n\r");		
		return false;
	} else {
	  printf("passed\n\r");
	}		

	///////////////////////////////////////////////////////////
	printf("Test 4. Checking RingBuffer_IsEmpty...       ");
	
	// at this point RingBuffer_IsEmpty should return false because the buffer is not empty
	if (true == RingBuffer_IsEmpty(&buffer)){
	  printf("FAILED!\n\r");
		printf("ERROR: Wrong return value.\n\r");		
		return false;
	} else {
	  printf("passed\n\r");
	}		
	
	///////////////////////////////////////////////////////////
	printf("Test 5. Checking RingBuffer_GetChar...       ");

	// read each character from buffer and check it
	for (size_t i = 0; i < RingBuffer_GetCapacity(&buffer); i++) {
		if (RingBuffer_GetChar(&buffer, &get) == false){
			  printf("FAILED!\n\r");
				printf("ERROR: Unable to get char no %d from buffer.\n\r", i);
				return false;
		} else {
			if (get != i % 0x7f) {
			  printf("FAILED!\n\r");
				printf("ERROR: Wrong char no %d in buffer.\n\r", i);
				return false;
			}
		}
	}
	// checking underflow behavior
  if (RingBuffer_GetChar(&buffer, &get) == true) {	
	  printf("FAILED!\n\r");
		printf("ERROR: Underflow not detected.\n\r");
		return false; 
	}
	
	// if all OK:
	printf("passed\n\r");
	
	///////////////////////////////////////////////////////////
	printf("Test 6. Checking RingBuffer_Clear...         ");
	
	// fill the buffer again
	for (size_t i = 0; i < RingBuffer_GetCapacity(&buffer); i++) {
		put = i % 0x7f;
		RingBuffer_PutChar(&buffer, put);
	}
	// clear the buffer
	if (RingBuffer_Clear(&buffer) == false) {
	  printf("FAILED!\n\r");
		printf("ERROR: Wrong return value.\n\r");
		return false;
	}
	// at this point the buffer should be clear, thus RingBuffer_GetLen should return 0
	if (RingBuffer_GetLen(&buffer) != 0) {
	  printf("FAILED!\n\r");
		printf("ERROR: Buffer length is non-zero after clear.\n\r");
		return false;
	}

	printf("passed\n\r");
	
	///////////////////////////////////////////////////////////
	printf("Test 7. Checking interleaved RingBuffer_PutChar and RingBuffer_GetChar... ");
	
	for (size_t i = 0; i < 1000; i++){
		put = i % 0x7f;
		if (RingBuffer_PutChar(&buffer, put) == false){
			printf("FAILED!\n\r");	
			printf("ERROR: unable to put char into buffer.\n\r");
			return false;
		}
		
		if (RingBuffer_GetChar(&buffer, &get)){
			if (put != get){
				printf("FAILED!\n\r");	
				printf("ERROR: read char is different than previously written. Test failed at i=%i.\n\r", i);
				return false;
			}
		} else {
			printf("FAILED!\n\r");	
			printf("ERROR: unable to get char from buffer.\n\r");
			return false;
		}
	}
	
	printf("passed\n\r");
	// all tests PASSED!
	return true;
}
