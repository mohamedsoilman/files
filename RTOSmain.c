/*
* TAS_main.c
*
* Created: 21-Jun-20 1:01:34 AM
*  Author: m.fathy
*/

/* include OS headers */#include "FreeRTOS.h"#include "task.h"#include "queue.h"#include "semphr.h"#include "event_groups.h"#include "ADC.h"#include "LCD.h"/* include Our Drivers */#include "board.h"#include "UART.h"/* Proto. */void T_Temp(void* pvData);void T_Alarm(void* pvData);void system_init(void);void INT0_Init(void);
void interr_handle(void);#define E_Alarm (1<<0)EventGroupHandle_t   egEvent;EventBits_t          ebValues;INT8U res=0;void main(void){	system_init();	LCD_displayString("Welcome To RTOS");
	LCD_displayStringRowColumn(1,0,"simple project:2");	_delay_ms(2000);	LCD_clearScreen();	egEvent=xEventGroupCreate();	xTaskCreate(T_Temp,"T_Temp",100,NULL,1,NULL);	xTaskCreate(T_Alarm,"T_Alarm",100,NULL,2,NULL);	vTaskStartScheduler();}void INT0_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	MCUCR |= (1<<ISC00) | (1<<ISC01);   // Trigger INT0 with the raising edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}void T_Temp(void* pvData){	while(1){		res=(ADC_Read(0)*150UL/1023);		LCD_displayStringRowColumn(0,0,"temperature : ");		LCD_intgerToString(res);		if (res >=50)		{			/* fire */			xEventGroupSetBits(egEvent,E_Alarm);		}		else if(res <50)		{			xEventGroupClearBits(egEvent,E_Alarm);		}	}}void T_Alarm(void* pvData){	while(1){				ebValues = xEventGroupWaitBits(egEvent,		(E_Alarm),		0, 		0, 		portMAX_DELAY);		if(ebValues&E_Alarm){			Leds_Toggle();			BUz_Toggle();			vTaskDelay(1000);			Leds_Toggle();			BUz_Toggle();			vTaskDelay(1000);		}	}}void system_init(void){	Leds_Init();	BUz_Init();	ADC_Init();	LCD_init();	INT0_Init();}

void interr_handle(void){	LCD_clearScreen();	LCD_displayStringRowColumn(0,0,"Thx for watching");	_delay_ms(1000);	LCD_displayStringRowColumn(1,0,"   while(1).....");	while(1);}
ISR(INT0_vect)
{
	interr_handle();
}




