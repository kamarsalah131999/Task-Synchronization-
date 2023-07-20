/*
 * Binary_semaphore.c
 *
 * Created: 7/19/2023 4:12:21 AM
 * Author : kamar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void task1 (void *pv);
SemaphoreHandle_t  my_sem;

int main(void)
{
   /*create task*/
   xTaskCreate(task1,"k",100,NULL,1,NULL);
   
   /*intialize interrupt*/
   sei();
   GICR|=(1<<INT0);                                               // external interrupt
   MCUCR|=(1<<ISC01);                                             //detect interrrupt  in rising edge
   
   /*create semaphore*/
   my_sem= xSemaphoreCreateBinary();
  
  /*start OS*/
  vTaskStartScheduler(); 
   
}

void task1 (void *pv)
{
	char y;
	DDRD|=(1<<7);                                                                  //pin 7 is output
	while(1)
	{
		y=xSemaphoreTake(my_sem,10000);                                           // required semaphore
		if(y==pdTRUE)                                                             //semaphore is available
		{
			PORTD^=(1<<7);                                                        // toggle led
			_delay_ms(2000);
		}
	}
		
}

ISR(INT0_vect)
{
	xSemaphoreGiveFromISR(my_sem,NULL);
}