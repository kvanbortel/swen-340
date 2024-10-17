/*
 * led_activity.c
 */

#include <stdio.h>
#include <string.h>

// Custom Include files
#include "UART.h"
#include "led_activity.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "LED.h"

#define MAX_COMMAND_LENGTH (7)

struct LED green_LED;
struct LED red_LED;

static volatile uint8_t one_second_elapsed = 0;
static uint32_t counter = 0;

static void init_systick()
{
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

void SysTick_Handler(void)
{
	counter++;
	if (counter >= 1000) {
		one_second_elapsed = 1;
		counter = 0;
	}
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
	char error_msg[] = "Invalid command\r\n";

	if (strcmp(command, "HELP") == 0) {
		prompt_menu();
	}
	else if (strcmp(command, "RON") == 0 && (!red_LED.isOn || red_LED.isFlashing)) {
		LED_On(RED_LED);
		red_LED.isOn = 1;
		red_LED.isFlashing = 0;
	}
	else if (strcmp(command, "ROFF") == 0 && (red_LED.isOn || red_LED.isFlashing)) {
		LED_Off(RED_LED);
		red_LED.isOn = 0;
		red_LED.isFlashing = 0;
	}
	else if (strcmp(command, "GON") == 0 && (!green_LED.isOn || green_LED.isFlashing)) {
		LED_On(GREEN_LED);
		green_LED.isOn = 1;
		green_LED.isFlashing = 0;
	}
	else if (strcmp(command, "GOFF") == 0 && (green_LED.isOn || green_LED.isFlashing)) {
		LED_Off(GREEN_LED);
		green_LED.isOn = 0;
		green_LED.isFlashing = 0;
	}
	else if (strcmp(command, "RFLASH") == 0 && !red_LED.isFlashing) {
		red_LED.isFlashing = 1;
	}
	else if (strcmp(command, "GFLASH") == 0 && !green_LED.isFlashing) {
		green_LED.isFlashing = 1;
	}
	else if (strcmp(command, "ALLOFF") == 0) {
		LED_Off(RED_LED);
		red_LED.isOn = 0;
		red_LED.isFlashing = 0;
		LED_Off(GREEN_LED);
		green_LED.isOn = 0;
		green_LED.isFlashing = 0;
	}
	else {
	    USART_Write( USART2, (uint8_t*)error_msg, strlen(error_msg));
	}
}

void run_led_activity()
{
	uint8_t one_char = 0 ;

	init_systick() ;
	LED_Init();
	char command[MAX_COMMAND_LENGTH] = {0};
	int cmd_index = 0;

	red_LED.isOn = 0;
	red_LED.isFlashing = 0;

	green_LED.isOn = 0;
	green_LED.isFlashing = 0;

	prompt_menu();
	prompt_input();

	while (1)
	{
		// Toggle flashing LEDs every second
		if (one_second_elapsed) {
			if (red_LED.isFlashing) {
				LED_Toggle(RED_LED);
			}
			if (green_LED.isFlashing) {
				LED_Toggle(GREEN_LED);
			}
			one_second_elapsed = 0;
		}

		one_char = USART_Read_Nonblocking( USART2 );

		if (one_char != 0) {
			// Enter key is pressed
			if (one_char == '\r' || one_char == '\n') {
				uint8_t newline[] = "\r\n";
				USART_Write( USART2, newline, 2 ) ;
                command[cmd_index] = '\0'; // Null-terminate the command string
                handle_command(command);
                cmd_index = 0; // Reset command index for next input
                memset(command, 0, MAX_COMMAND_LENGTH*sizeof(char)); // Clear the command buffer
            	prompt_input();
			}
			// Backspace is pressed
		    else if (one_char == '\b') {
		        if (cmd_index > 0) {
		            cmd_index--;
		            command[cmd_index] = '\0';

		            // Visually backspace
		            uint8_t backspace[] = "\b \b";
		            USART_Write(USART2, backspace, 3);
		        }
		    }
			else if (cmd_index < MAX_COMMAND_LENGTH - 1) {
                command[cmd_index++] = one_char; // Store the character in the command buffer
                USART_Write(USART2, &one_char, 1);
			}
		}
	}
}

