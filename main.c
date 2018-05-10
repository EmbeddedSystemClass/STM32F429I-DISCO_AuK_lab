#include "stm32f4xx_hal.h"
#include "delay.h"
#include "ring_buffer_unit_test.h"
#include "ring_buffer.h"
#include "usart.h"
#include "core.h"
#include "usart_unit_test.h"
#include "cli.h"

#define __GPIOG_CLK_ENABLE()     (RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOGEN))



extern int ticks;
int executionTime=0;
/**
 * This function configures the LED pins
 */
  


void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
 
  // GPIO Ports Clock Enable
  __GPIOG_CLK_ENABLE();
 
  // Configure GPIO pin PG13
  GPIO_InitStruct.Pin   = GPIO_PIN_13;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;        // push-pull output
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;              // pull-down enabled
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;             // analog pin bandwidth limited
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	GPIO_InitStruct.Pin   = GPIO_PIN_14;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;        // push-pull output
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;              // pull-down enabled
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;             // analog pin bandwidth limited
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
        
} /* LED_Init */




void SysTick_Handler(void)
{
   ticks++;        
} /* SysTick_Handler */

//void commandLED(){
//	HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
//}
void commandLED(char *args){
    if(strcmp(args, "on") == 0){
        // turn the LED on
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
    } else if(strcmp(args, "off") == 0){
        // turn the LED off
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
    } else {
        USART_WriteString("Unrecognized argument. Available arguments are \"on\" and \"off\"\n\r");
    }
}
 
void commandLED2(char *args){
    if(strcmp(args, "on") == 0){
        // turn the LED on
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
    } else if(strcmp(args, "off") == 0){
        // turn the LED off
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
    } else {
        USART_WriteString("Unrecognized argument. Available arguments are \"on\" and \"off\"\n\r");
    }
}

int main(void)
{
	 char c;
	char c2[10];
   USART_Init();
	
//	USART_WriteString("Hello world\n\r");
	//USART_WriteData("Hello world\n\r",10);
	//USART_PutChar('c');	
	
  // Initialize STM32Cube HAL library
  HAL_Init();
  // Initialize LED pins
  LED_Init();
 // USART_UnitTest();
  // Blink the LED on pin PG.13
CLI_CommandItem wrong_item = { .callback = NULL,
                            .commandName = "WrongItemThisShouldn'tBeAdded",
                            .description = NULL};
        
CLI_CommandItem item_LED = { .callback = commandLED,
                          .commandName = "led1",
                          .description = NULL};

CLI_CommandItem item_LED2 = { .callback = commandLED2,
                          .commandName = "led2",
                          .description = NULL};
 

 
if(CLI_AddCommand(&wrong_item) == false){
    USART_WriteString("ERROR in adding new item.\n\r");
}
//commandLED(1);

if(CLI_AddCommand(&item_LED) == false){
    USART_WriteString("ERROR in adding new item.\n\r");
}
//commandLED(2);

if(CLI_AddCommand(&item_LED2) == false){
    USART_WriteString("ERROR in adding new item.\n\r");
}//commandLED(3);
//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);

CLI_PrintAllCommands();

  while (1) {
	CLI_Proc();
	 // USART_PutChar('c');	
	//if(USART_GetChar(&c)){
  //  USART_PutChar(c);
	//	 USART_PutChar(c);
//	}
	
//	if(USART_ReadData(c2,10))
//	{
//		USART_WriteString(c2);	
//	USART_PutChar('\n');		
	//}
    // turn the LED on
 //   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
    // introduce some delay
 //   delay(100);
    // turn the LED off
 //   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
  //  // introduce some delay
  //  delay(100);
//	printf("Program execution time: %ds\n\r", executionTime++);
  } // while (1)   
	
}  
  


