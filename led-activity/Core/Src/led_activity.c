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

#define MAX_COMMAND_LENGTH (7)

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
	const char *menu_str =
		"\r\n***REMOTE LED CONTROL MENU***\r\n"
		"Available User Commands\r\n"
		"RON - Turn on RED LED\r\n"
		"ROFF - Turn off RED LED\r\n"
		"GON - Turn on GREEN LED\r\n"
		"GOFF - Turn off GREEN LED\r\n"
		"RFLASH - STart flashing RED LED\r\n"
		"GFLASH - Start flashing GREEN LED\r\n"
		"ALLOFF - TURNOFF LEDs\r\n";

    USART_Write( USART2, (uint8_t *)menu_str, strlen(menu_str));
}

void prompt_input(void) {
	char str[] = ">> ";
    USART_Write( USART2, (uint8_t*)str, 2);
}

void handle_command(char *command) {
	command = strupr(command);
	if (strcmp(command, "HELP") == 0) {
		prompt_menu();
	}
	else if (strcmp(command, "RON") == 0) {
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
	char command[MAX_COMMAND_LENGTH] = {0};
	int cmd_index = 0;

	prompt_menu();
	prompt_input();

	while (1)
	{
		delay_systick();

		one_char = USART_Read_Nonblocking( USART2 );

		if (one_char != 0) {
			if (one_char == '\r' || one_char == '\n') {
				uint8_t newline[] = "\r\n";
				USART_Write( USART2, newline, 2 ) ;
                command[cmd_index] = '\0'; // Null-terminate the command string
                handle_command(command);
                cmd_index = 0; // Reset command index for next input
                memset(command, 0, MAX_COMMAND_LENGTH*sizeof(char)); // Clear the command buffer
            	prompt_input();
			}
			else if (cmd_index < MAX_COMMAND_LENGTH - 1) {
                command[cmd_index++] = one_char; // Store the character in the command buffer
                USART_Write(USART2, &one_char, 1);
			}
		}

		delay_count++;

		if (delay_count >= 1000) {
			LED_Toggle(NUCLEO_RED_LED_PIN);
			LED_Toggle(NUCLEO_GREEN_LED_PIN);
			delay_count = 0;
		}

	}
}

