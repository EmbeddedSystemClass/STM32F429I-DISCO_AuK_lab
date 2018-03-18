#include "stm32f4xx_hal.h"
#include "delay.h"

volatile int ticks;

void delay(int ms)
{
	int timestop =ticks+ms; 
	while (ticks<timestop)
	{	
	}	
} 
