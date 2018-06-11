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
#include "semphr.h"
//#define __GPIOG_CLK_ENABLE()     (RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOGEN))

extern int ticks;
int executionTime=0;

//zd7

	SemaphoreHandle_t mySemaphore;
	SemaphoreHandle_t myMutex;
	static QueueHandle_t myQueue;
	static int QueueCount = 0;
	//int mySemaphore;
	
	
	
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

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
 
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
 
  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
 
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}



//zad8
void USART_POLL_WriteString(const char *string)
{
  size_t i=0;
  // for each character in the given string
  while (string[i]) {
    // write the current character to USART's data register (DR)
    USART1->DR = string[i];
    // wait until status register (SR) indicates that the transmitter is empty again (TXE)
    while((USART1->SR & UART_FLAG_TXE) == 0) {
      ;
    }
    // move to the next character
    i++;
  }
}
 
//zad7
/**
 * Configures EXTI Line2 (connected to PG2 pin) in interrupt mode
 */
static void EXTI2_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
        
  // Enable GPIOG clock
  __GPIOG_CLK_ENABLE();
  
  // Configure PG2 pin as input with EXTI interrupt on the falling edge and pull-up
  GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure); 
 
  // Enable and set EXTI Line2 Interrupt to the lowest priority
  HAL_NVIC_SetPriority(EXTI2_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}
 
/**
 * This function handles External line 2 interrupt request.
 */
void EXTI2_IRQHandler(void)
{
	
  // Check if EXTI line interrupt was detected
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)  {
    // Clear the interrupt (has to be done for EXTI)
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
    // Toggle LED
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
	
  }
}


void taskHELLO(void* params)
{
  while (1) {
    	if (myMutex != NULL ) 
			{
        //branie semafora
				if( xSemaphoreTake( myMutex, ( TickType_t ) 100000 ) == pdTRUE )
        {
         
			//wypisanie
            USART_POLL_WriteString("hello worldregv\n");

            //oddanie smeafora
            xSemaphoreGive( myMutex );
        }
			}
  }
}
void taskHELLO2(void* params)
{
  while (1) {
		if (myMutex != NULL ) 
			{

				if( xSemaphoreTake( myMutex, ( TickType_t ) 100000 ) == pdTRUE )
        {
           
            USART_POLL_WriteString("hello world2\n");

            xSemaphoreGive( myMutex );
        }
			}
    
  }
}
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
//zad-9
void taskTXQ(void* params)
{
	while(true)
	{ 
		//to do
		//if (xQueueSend(myQueue, &QueueCount, 1000) != pdPASS) {
		xQueueSend(myQueue, &QueueCount, 1000);
    // Failed to put new element into the queue, even after 1000 ticks.
		//}
		QueueCount++;
		//HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_14);
		vTaskDelay(1000);
	}	
}
void taskRXQ(void* params)
{
	while(true)
	{ 
		
		int element;
		if (xQueueReceive(myQueue, &element, 1000 ) == pdTRUE ) 
	{
  // element was received successfully
		char bufor[50];
		sprintf(bufor,"Element numer:  %d \n\r", element-16777216);//sprawdzic czemu trzeba odjac
		USART_WriteString(bufor);
		USART_WriteString("\n\r");
	//	printf("Got %d from myQueue", myQueue);
	} else {
  // unable to receive elements from the queue in the given time = 1000 ticks
}
		//HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_14);
		//vTaskDelay(1500);
	}	
}
//lab3-Z04
//void taskLED3(void* params)
//{
//  // Blink the LED based on the provided arguments
//  while (params) {
//    // Toggle LED
//    HAL_GPIO_TogglePin(((BlinkParams*)params)->gpio, ((BlinkParams*)params)->pin);
//    // Introduce some delay
//    vTaskDelay(((BlinkParams*)params)->ticks);
//  } // while (params)
// 
//  // Delete the task in case the provided argument is NULL
//  vTaskDelete(NULL);
// 
//} /* taskLED */

void taskCLI(void* params)
{while(1){
	CLI_Proc();
	vTaskDelay(5);}
}
	



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

//L03-Z6 DO ZROBIENIA !! - przerobic taskDelay uzywajac vTaskDelayUntil, sprawdzic stanem logicznym
void activity(void)
{
    static volatile uint32_t period = 400000;
    volatile uint32_t i;
        
    // this loop executes 400000 or 80000 times,
    // which makes this activity once longer once shorter in time
    for (i = 0; i < period; i++) {
        __asm__ volatile ("NOP");
    }
        
    if (period == 400000) {
        period = 80000;
    } else {
        period = 400000;
    }
}



//void taskDELAY(void* params)
//{
//    
//    TickType_t start, stop;
//        
//    // we are going to use these two trace signals: 
//    TRACE_SetLow(5);
//    TRACE_SetLow(6);
//        
//    while (1) {         
//        // trace signal 5 toggles on every loop
//        TRACE_Toggle(5);
//        // save activity start time
//        start   = xTaskGetTickCount();
//        // trace 6 goes high at the start of activity
//        TRACE_SetHigh(6);
//        activity();
//        // trace 6 goes low at the end of activity
//        TRACE_SetLow(6);
//        // save activity end time
//        stop    = xTaskGetTickCount();
//        // delay task execution until 500ms - duration of activity
//        vTaskDelay(500 - (stop-start));
//    }
//}

void taskDELAY(void* params)
{
    
    TickType_t start, stop;
        
    // we are going to use these two trace signals: 
    TRACE_SetLow(5);
    TRACE_SetLow(6);
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500;
	xLastWakeTime = xTaskGetTickCount();
    while (1) {         
        // trace signal 5 toggles on every loop
        TRACE_Toggle(5);
        // save activity start time
        start   = xTaskGetTickCount();
        // trace 6 goes high at the start of activity
        TRACE_SetHigh(6);
        activity();
        // trace 6 goes low at the end of activity
        TRACE_SetLow(6);
        // save activity end time
        stop    = xTaskGetTickCount();
        // delay task execution until 500ms - duration of activity
		//vTaskDelay(500 - (stop-start));
		
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}



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

int main (void)
{
  // Initialize STM32Cube HAL library
  HAL_Init();
  // Initialize LED pins
  LED_Init();
 
  // Create mutex  
  myMutex= xSemaphoreCreateMutex();
  // Create queue
  myQueue= xQueueCreate(100, 1);
 
  // Create RTOS tasks
  if (pdPASS != xTaskCreate(taskLED, "led", 100, NULL, 3, NULL )) {
    // should never get here, unless there is a memory allocation problem
  }
 
  // start FreeRTOS scheduler - the RTOS takes control over the microcontroller
  vTaskStartScheduler();
 
} /* main */ 

//int main(void)
//{  
//	HAL_Init();
//	SystemClock_Config();
//	LED_Init();
//	USART_Init();
//	//TRACE_Init();
//	EXTI2_Init();
//	mySemaphore = xSemaphoreCreateBinary();
//	xSemaphoreGive(mySemaphore);
//	myMutex= xSemaphoreCreateMutex();
//	myQueue= xQueueCreate(100, 1);
//	if (myMutex != NULL ) {
//        // The mutex was created successfully and can now be used
//	}
//	
//		
//	/* RTOS - ZAD 1 */

//	//	vTaskStartScheduler();		
//		
//		
// /* CLI - ADDING TO LIST*/
//CLI_CommandItem wrong_item = { .callback = NULL,
//                            .commandName = "WrongItemThisShouldn'tBeAdded",
//                            .description = NULL};
//        
//CLI_CommandItem item_LED = { .callback = commandLED,
//                          .commandName = "led1",
//                          .description = "Turn on or off led1"};

//CLI_CommandItem item_LED2 = { .callback = commandLED2,
//                          .commandName = "led2",
//                          .description = "Turn on or off led2"};
// 

// 
//if(CLI_AddCommand(&wrong_item) == false){
//    USART_WriteString("ERROR in adding new item.\n\r");
//}

//if(CLI_AddCommand(&item_LED) == false){
//    USART_WriteString("ERROR in adding new item.\n\r");
//}


//if(CLI_AddCommand(&item_LED2) == false){
//    USART_WriteString("ERROR in adding new item.\n\r");
//}


//CLI_PrintAllCommands();


////L03-Z03
////if (pdPASS != xTaskCreate(taskLED, "led", configMINIMAL_STACK_SIZE, NULL, 3, NULL)) {
////        printf("ERROR: Unable to create task!\n");
////    } 
////  if(pdPASS!=xTaskCreate(taskLED2,"led2",configMINIMAL_STACK_SIZE,NULL,3,NULL)) printf("ERROR: Unable to create task!\n");     
////	USART_WriteString("SEX");
//  
////L04-Z04 
////if (pdPASS != xTaskCreate(taskLED3, "led1", configMINIMAL_STACK_SIZE, &bp1, 3, NULL)) {
////  printf("ERROR: Unable to create task!\n");
////}
////if (pdPASS != xTaskCreate(taskLED3, "led2", configMINIMAL_STACK_SIZE, &bp2, 3, NULL)) {
////  printf("ERROR: Unable to create task!\n");
////}
////if(pdPASS!=xTaskCreate(taskCLI,"cli",configMINIMAL_STACK_SIZE,NULL,3,NULL)) printf("ERROR: Unable to create task!\n");  		
////zad8
//	TaskHandle_t taskHandle[4];
//	//TaskHandle_t taskHandle2;
//	
////if (pdPASS != xTaskCreate(taskHELLO, "taskHELLO", configMINIMAL_STACK_SIZE, NULL, 3, NULL)) 
////	{
////		printf("ERROR: Unable to create task!\n");
////    } if (pdPASS != xTaskCreate(taskHELLO2, "taskHELLO2", configMINIMAL_STACK_SIZE, NULL, 3, NULL)) 
////	{
////		printf("ERROR: Unable to create task!\n");
////    } 
//	//zad9
//	     if (pdPASS != xTaskCreate(taskTXQ, "taskTXQ", configMINIMAL_STACK_SIZE, NULL, 3, NULL)) 
//	{
//		printf("ERROR: Unable to create task!\n");
//  }
//	     if (pdPASS != xTaskCreate(taskRXQ, "taskRXQ", configMINIMAL_STACK_SIZE, NULL, 3, NULL)) 
//	{
//		printf("ERROR: Unable to create task!\n");
//    }
//	
////	if (pdPASS != xTaskCreate(taskHELLO, "taskHELLO", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle[0])) {
////    printf("ERROR: Unable to create task!\n");}
////	if (pdPASS != xTaskCreate(taskHELLO2, "taskHELLO2", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle[1])) {
////    printf("ERROR: Unable to create task!\n");}
//	//zad9
//		if (pdPASS != xTaskCreate(taskTXQ, "taskTXQ", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle[2])) {
//    printf("ERROR: Unable to create task!\n");}
//	if (pdPASS != xTaskCreate(taskRXQ, "taskRXQ", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle[3])) {
//    printf("ERROR: Unable to create task!\n");}
//		
////	TRACE_BindTaskWithTrace(taskHandle[0], 2);
////	TRACE_BindTaskWithTrace(taskHandle[1], 2);
//	TRACE_BindTaskWithTrace(taskHandle[2], 2);
//	TRACE_BindTaskWithTrace(taskHandle[3], 2);	
//	vTaskStartScheduler();
////L03-Z05

////TaskHandle_t taskHandle;
////TaskHandle_t taskHandle2;
////TaskHandle_t taskHandle3;
////TaskHandle_t taskHandle4;
////// create FreeRTOS task and save it's handle in taskHandle
////if (pdPASS != xTaskCreate(taskLED, "led1", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle)) {
////    printf("ERROR: Unable to create task!\n");}

////if (pdPASS != xTaskCreate(taskLED2, "led2", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle2)) {
////    printf("ERROR: Unable to create task!\n");
////}

////if (pdPASS != xTaskCreate(taskCLI, "cli", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle3)) {
////    printf("ERROR: Unable to create task!\n");
////}
////if (pdPASS != xTaskCreate(taskDELAY, "taskDELAY", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandle4)) {
////    printf("ERROR: Unable to create task!\n");
////}
////// bind the newly created task with trace signal 1

////TRACE_BindTaskWithTrace(taskHandle, 2);
////TRACE_BindTaskWithTrace(taskHandle2, 3);
////TRACE_BindTaskWithTrace(taskHandle3, 4);
////TRACE_BindTaskWithTrace(taskHandle4, 5);
////  vTaskStartScheduler();
//while (1) {
//	CLI_Proc();}		
//	

//}  
//  


