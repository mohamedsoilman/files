/*
* TAS_main.c
*
* Created: 21-Jun-20 1:01:34 AM
*  Author: m.fathy
*/

/* include OS headers */#include "FreeRTOS.h"#include "task.h"#include "queue.h"#include "semphr.h"/* include Our Drivers */#include "board.h"#include "UART.h"/* Proto. */void T_Low(void* pvData);void T_med(void* pvData);void T_High(void* pvData);void system_init(void);/* OS Services Decl.*/xSemaphoreHandle bsBtnPressed;int main(void){	/* Init Pr. */	system_init();	/* OS Services Create and Init.*/	bsBtnPressed=xSemaphoreCreateMutex();	//vSemaphoreCreateBinary(bsBtnPressed,1);	/* Tasks Creat. */	xTaskCreate(T_Low,"T_Low",100,NULL,1,NULL);	/* Start OS or Sched. */	vTaskStartScheduler();}void T_Low(void* pvData){	/* recData is a local or temp message */	while(1)	{		Led_On();		vTaskDelay(2000);		Led_Off();		vTaskDelay(2000);		xSemaphoreTake(bsBtnPressed,portMAX_DELAY);		xTaskCreate(T_High,"T_High",100,NULL,3,NULL);		xTaskCreate(T_med,"T_med",100,NULL,2,NULL);		xSemaphoreGive(bsBtnPressed);	}}void T_High(void* pvData){	while(1)	{		Led_On();		_delay_ms(500);		Led_Off();		_delay_ms(500);		/* xSemaphoreTake return true is the semaphore is available		 * and here in the beginning it's return 0 cz T_Low have the semaphore		 */		if (xSemaphoreTake(bsBtnPressed,portMAX_DELAY))
		{
			while(1);
		}		vTaskDelay(5);	}}void T_med(void* pvData){	while(1)	{		Led_On();		_delay_ms(7000);		Led_Off();		vTaskDelay(7000);	}}void system_init(void){	Led_Init();	//Uart_init(Uart_0,9600);	//Uart_sendstr(Uart_0,"Started ...\r\n");}
