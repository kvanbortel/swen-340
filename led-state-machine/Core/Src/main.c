// main.c with led_activity


// include project specific header files
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "state-machine.h"

	
int main(void){

	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	
	// application run function
	run_led_activity() ;
}

