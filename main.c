#include "stm32f4xx_hal.h"
#include "delay.h"
#include "ring_buffer_unit_test.h"
#include "ring_buffer.h"
#include "usart.h"
#include "core.h"
#include "usart_unit_test.h"
#include "cli.h"
#include "FreeRTOS.h"
#include "task.h"
#include "trace.h"

//#define __GPIOG_CLK_ENABLE()     (RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOGEN))

extern int ticks;
int executionTime=0;


/* L03-Zadanie 4*/
typedef struct {
        GPIO_TypeDef* gpio;    // GPIO port
        uint16_t      pin;     // GPIO pin
        TickType_t    ticks;   // delay expressed in system ticks
} BlinkParams;

static BlinkParams bp1 = { .gpio = GPIOG, .pin = GPIO_PIN_13, .ticks = 500};
static BlinkParams bp2 = { .gpio = GPIOG, .pin = GPIO_PIN_14, .ticks = 1000};

/////////////////////////////////////////////////


/////// L03-Zadanie3 ////////////////////
 // 1. Dodac implementacje drugiego zadania, które bedzie mrugac dioda LED (GPIOG.14) 
//	z inna czestotliwoscia oraz uruchomic to zadanie w systemie FreeRTOS. Nazwijmy dla porzadku funkcje bedaca implementacja tego zadania jako: taskLED2(), a samo zadanie: "led2".


//2. Dodac implementacje watku, który okresowo (co powiedzmy 10ms) wywolywal bedzie funkcje CLI_Proc(),
//tym samym realizujac obsluge procesora komend. Nazwijmy dla porzadku funkcje bedaca implementacja tego zadania jako: taskCLI(), a samo zadanie: "cli".

void taskLED(void* params)
{
    // Toggle the LED on pin PG.13
    while (1) {
        // toggle the LED
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
        // introduce some delay
        vTaskDelay(500);
    } // while (1)
} /* taskLED */

void taskLED2(void* params)
{
	while(true)
	{ HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_14);
		vTaskDelay(1500);
	}	
}

void taskCLI(void* params)
{while(1){
	CLI_Proc();
	vTaskDelay(5);}
}
	


void taskLED3(void* params)
{
  // Blink the LED based on the provided arguments
  while (params) {
    // Toggle LED
    HAL_GPIO_TogglePin(((BlinkParams*)params)->gpio, ((BlinkParams*)params)->pin);
    // Introduce some delay
    vTaskDelay(((BlinkParams*)params)->ticks);
  } // while (params)
 
  // Delete the task in case the provided argument is NULL
  vTaskDelete(NULL);
 
} /* taskLED */

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





//void SysTick_Handler(void)
//{
//   ticks++;        
//} /* SysTick_Handler */


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
{  HAL_Init();
	 LED_Init();
	 USART_Init();
	 TRACE_Init();
	
		
	/* RTOS - ZAD 1 */

	//	vTaskStartScheduler();		
		
		
 /* CLI - ADDING TO LIST*/
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

if(CLI_AddCommand(&item_LED) == false){
    USART_WriteString("ERROR in adding new item.\n\r");
}


if(CLI_AddCommand(&item_LED2) == false){
    USART_WriteString("ERROR in adding new item.\n\r");
}


CLI_PrintAllCommands();


//L03-Z03
//if (pdPASS != xTaskCreate(taskLED, "led", configMINIMAL_STACK_SIZE, NULL, 3, NULL)) {
//        printf("ERROR: Unable to create task!\n");
//    } 
//  if(pdPASS!=xTaskCreate(taskLED2,"led2",configMINIMAL_STACK_SIZE,NULL,3,NULL)) printf("ERROR: Unable to create task!\n");     
//	USART_WriteString("SEX");
  
//L04-Z04 
if (pdPASS != xTaskCreate(taskLED3, "led1", configMINIMAL_STACK_SIZE, &bp1, 3, NULL)) {
  printf("ERROR: Unable to create task!\n");
}
if (pdPASS != xTaskCreate(taskLED3, "led2", configMINIMAL_STACK_SIZE, &bp2, 3, NULL)) {
  printf("ERROR: Unable to create task!\n");
}
if(pdPASS!=xTaskCreate(taskCLI,"cli",configMINIMAL_STACK_SIZE,NULL,3,NULL)) printf("ERROR: Unable to create task!\n");  		
  
	
//L03-Z05
TaskHandle_t taskHandle;
// create FreeRTOS task and save it's handle in taskHandle
if (pdPASS != xTaskCreate(taskLED, "led1", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle)) {
    printf("ERROR: Unable to create task!\n");
}
// bind the newly created task with trace signal 1
TRACE_BindTaskWithTrace(taskHandle, 1);
  vTaskStartScheduler();
 // while (1) {
//	CLI_Proc();}		
	
	
}  
  


