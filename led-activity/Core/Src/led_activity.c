/*
 * led_activity.c
 */

#include <stdio.h>
#include <string.h>

// Custom Include files
//#include "LED.h"
#include "UART.h"
#include "led_activity.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "LED.h"

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
}

static void delay_systick()
{
	// Using the SysTick global structure pointer do the following:
	//
	// Check for the COUNTFLAG to be set. Return as soon as it is set..
	// This loop for the COUNTFLAG is a blocking call but not for more than 1 millisecond.
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {}
}

void prompt_menu(void) {
	printf("***REMOTE LED CONTROL MENU***\n");
	printf("Available User Commands\n");
	printf("RON - Turn on RED LED");
	printf("ROFF - Turn off RED LED");
	printf("GON - Turn on GREEN LED");
	printf("GOFF - Turn off GREEN LED");
	printf("RFLASH - STart flashing RED LED");
	printf("GFLASH - Start flashing GREEN LED");
	printf("ALLOFF - TURNOFF LEDs");
}

void handle_command(char *command) {
	command = strupr(command);
	if (strcmp(command, "RON") == 0) {
		LED_On(NUCLEO_RED_LED_PIN);
	}
	else if (strcmp(command, "ROFF") == 0) {
		LED_Off(NUCLEO_RED_LED_PIN);
	}
	else if (strcmp(command, "GON") == 0) {
		LED_On(NUCLEO_GREEN_LED_PIN);
	}
	else if (strcmp(command, "GOFF") == 0) {
		LED_Off(NUCLEO_RED_LED_PIN);
	}
	else if (strcmp(command, "RFLASH") == 0) {

	}
	else if (strcmp(command, "GFLASH") == 0) {

	}
	else if (strcmp(command, "ALLOFF") == 0) {
		LED_Off(NUCLEO_RED_LED_PIN);
		LED_Off(NUCLEO_GREEN_LED_PIN);
	}
	else {
		printf("Invalid command");
	}
}


void run_led_activity()
{
	uint8_t one_char = 0 ;
	int delay_count = 0;

	init_systick() ;
	LED_Init();
	char *command = "\n";
	int command_done = 0;

	prompt_menu();

	while (1)
	{
		delay_systick();

		one_char = USART_Read_Nonblocking( USART2 );

		if (one_char != 0) {
			if (one_char == '\r' || one_char == '\n') {
				uint8_t newline[] = "\r\n";
				USART_Write( USART2, newline, 2 ) ;
				command += '\n';
				command_done = 1;
			}
			else {
				USART_Write( USART2, &one_char, 1 ) ;
				command += one_char;
			}
		}


		if (command_done) {
			handle_command(command);
		}
		command_done = 0;

		delay_count++;

		if (delay_count >= 1000) {
			LED_Toggle(NUCLEO_RED_LED_PIN);
			LED_Toggle(NUCLEO_GREEN_LED_PIN);
			delay_count = 0;
		}

	}
}

