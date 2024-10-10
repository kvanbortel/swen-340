/*
 * demo.c
 *
 *  Created on: Feb 14, 2021
 *      Author: larry kiser
 *  Update to starter code for non-blocking assignment.
 *
 *  Updated on: Sept 20, 2021
 *      Author: Mitesh Parikh
 */

#include <stdio.h>
#include <string.h>

#include "LED.h"
#include "UART.h"
#include "demo.h"
#include "stm32l4xx.h"

#include <ctype.h>

static volatile uint8_t one_second_elapsed = 0;
static uint32_t counter = 0;

#if 0
static void delay_loop( int value )
{
	// spin loop consuming CPU to spend time.
	for (int i = 0; i < value; i++)
		;
}
#endif

static void init_systick()
{
	// Use the SysTick global structure pointer to do the following in this
	// exact order with separate lines for each step:
	//
	// Disable SysTick by clearing the CTRL (CSR) register.
	WRITE_REG(SysTick->CTRL, 0);

	// Set the LOAD (RVR) to 80,000 to give us a 1 millisecond timer.
	WRITE_REG(SysTick->LOAD, 80000);

	// Set the clock source bit in the CTRL (CSR) to the internal clock.
	SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);

	// Set the enable bit in the CTRL (CSR) to start the timer.
	SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

	SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk); // Enable SysTick interrupt
}

#if 0
static void delay_systick()
{
	// Using the SysTick global structure pointer do the following:
	//
	// Check for the COUNTFLAG to be set. Return as soon as it is set..
	// This loop for the COUNTFLAG is a blocking call but not for more than 1 millisecond.
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {}
}
#endif

void SysTick_Handler(void)
{
	counter++;
	if (counter >= 1000) {
		one_second_elapsed = 1;
		counter = 0;
	}
}

void run_demo()
{
	uint8_t one_char = 0 ;

	init_systick() ;
	LED_Init();

	while (1)
	{
		// Change this to delay of only 1 millisecond.

		one_char = USART_Read_NonBlocking( USART2 ) ;	// change this to a non-blocking call

		// Only echo the character if we got a character
		// If we get an Enter key then also write out a '\n'
		if (one_char != 0) {
			if (one_char == '\r' || one_char == '\n') {
				uint8_t newline[] = "\r\n";
				USART_Write( USART2, newline, 2 ) ;
			}
			else {
				USART_Write( USART2, &one_char, 1 ) ;
			}
		}

		if (one_second_elapsed) {
			LED_Toggle();
			one_second_elapsed = 0;
		}

	}
}

