// State Machine Example Code for SWEN 340 LED flashing project
// This has two state machines with one command string parser.
// It is an intentionally incomplete implementation.
// Larry Kiser, March 8, 2023


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "LED.h"

static volatile uint8_t one_second_elapsed = 0;
static uint32_t counter = 0;

#define print_array(str) USART_Write(USART2, (uint8_t *)(str), sizeof(str) - 1)

// The following enums go into a state machine specific header file if used in more than one .c file
enum LED_states
{
	state_off,
	state_on,
	state_flashing,
	state_unknown
} ;

// NOTE -- these enum values must match the
//         command_strings indices.
enum command_events
{
	help = 0,	// enums start at 0 unless modified.
	ron = 1,
	roff = 2,
	gon = 3,
	goff = 4,
	rflash = 5,
	gflash = 6,
	alloff = 7,
	no_event	// must correspond to the NULL command string index
} ;

// NOTE -- the index of these strings must match the values of
//			the command_events enum values.
char *command_strings[] =
{
	"HELP",
	"RON",
	"ROFF",
	"GON",
	"GOFF",
	"RFLASH",
	"GFLASH",
	"ALLOFF",
	// terminate this array with a NULL pointer to make it easy to use this array.
	NULL
} ;

// check for match on name and return the corresponding command event
// or no_event if there is no match.
enum command_events check_name( char *possible_command )
{
	for ( enum command_events index = help ; command_strings[ index ] ; index++ ) {
		if ( strcmp( possible_command, command_strings[ index ] ) == 0 ) {
			return index ;
		}
	}

	return no_event ;
}

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
	const char menu_str[] =
		"\r\n***REMOTE LED CONTROL MENU***\r\n"
		"Available User Commands\r\n"
		"RON - Turn on RED LED\r\n"
		"ROFF - Turn off RED LED\r\n"
		"GON - Turn on GREEN LED\r\n"
		"GOFF - Turn off GREEN LED\r\n"
		"RFLASH - Start flashing RED LED\r\n"
		"GFLASH - Start flashing GREEN LED\r\n"
		"ALLOFF - TURNOFF LEDs\r\n";
    print_array(menu_str);
}

void prompt_input(void) {
	char str[] = ">> ";
    print_array(str);
}

enum command_events check_for_event()
{
	char error_msg[] = "Invalid command\r\n";
	enum { MAX_LENGTH = 20 };

	static char current_command[ MAX_LENGTH + 1 ] ;
	enum command_events event = no_event;

	// call non blocking read to build the current command buffer including processing deletes
	static int cmd_index = 0;
	uint8_t one_char = 0 ;
	one_char = USART_Read_Nonblocking( USART2 );

	if (one_char != 0) {
		// Enter key is pressed
		if (one_char == '\r' || one_char == '\n') {
			char newline[] = "\r\n";
			print_array(newline);
            current_command[cmd_index] = '\0'; // Null-terminate the command string
            cmd_index = 0; // Reset command index for next input
        	event = check_name( current_command ) ;
        	if (event == no_event) { // Handle an invalid input
        	    print_array(error_msg);
        	    prompt_input();
        	}
            memset(current_command, 0, MAX_LENGTH*sizeof(char)); // Clear the command buffer
		}
		// Backspace is pressed
	    else if (one_char == '\b') {
	        if (cmd_index > 0) {
	            cmd_index--;
	            current_command[cmd_index] = '\0';

	            // Visually backspace
	            char backspace[] = "\b \b";
	            print_array(backspace);
	        }
	    }
		else if (cmd_index < MAX_LENGTH - 1) {
            current_command[cmd_index++] = one_char; // Store the character in the command buffer
            USART_Write(USART2, &one_char, 1);
		}
	}
	return event;
}

void red_led_state_machine (enum command_events new_event, bool global_clock)
{
    static bool flashing = false;
    switch (new_event)
    {
        case ron:
            flashing = false;
            LED_On(RED_LED);
            break;
        case roff:
        case alloff:
            flashing = false;
            LED_Off(RED_LED);
            break;
        case rflash:
            flashing = true;
        default:
        	;
    }

    if (flashing) {
    	if (global_clock)
    		LED_On(RED_LED);
		else
			LED_Off(RED_LED);
    }
}

void green_led_state_machine (enum command_events new_event, bool global_clock)
{
    static bool flashing = false;
    switch (new_event)
    {
        case gon:
            flashing = false;
            LED_On(GREEN_LED);
            break;
        case goff:
        case alloff:
            flashing = false;
            LED_Off(GREEN_LED);
            break;
        case gflash:
            flashing = true;
        default:
        	;
    }

    if (flashing) {
    	if (global_clock)
    		LED_On(GREEN_LED);
		else
			LED_Off(GREEN_LED);
    }
}

void run_led_activity()
{
	init_systick() ;

	prompt_menu();
	prompt_input();

	bool global_clock = false;

	while (1)
	{
		// Toggle flashing LEDs every second
		if (one_second_elapsed) {
			global_clock = !global_clock;
			one_second_elapsed = 0;
		}

		enum command_events new_event = check_for_event() ;
		if (new_event == help)
		{
			prompt_menu();
		}
		red_led_state_machine( new_event, global_clock) ;
		green_led_state_machine(new_event, global_clock);
		if (new_event != no_event)
		{
        	prompt_input();
		}
	}
}
