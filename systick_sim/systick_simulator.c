// systick_simulator code
// students can view but not update!
// Larry Kiser copyright 2021

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "systick_simulator.h"


static int previous_enabled_state = 0 ;		// allows us to see when we go from disabled to enabled.
static enum simulator_control hardware_pointer = simulation ;	// tracks if we are in simulator or hardware mode
static systick_registers_t simulated_systick_registers ;		// permanent storage for the simulated hardware registers
static systick_registers_t *p_systick_registers = &simulated_systick_registers ;


// helper function for the check_for_corruption routine to avoid duplication
static void print_and_quit( char *error )
{
	puts( error ) ;
	puts( "Exiting program due to this error" ) ;
	exit( 1 ) ;
} 

// test for invalid updates to reserved areas of the registers.
// also forbid use of tick interrupt flag in CSR
// and stop with error on corruption.
static void check_for_corruption( void )
{
	if ( p_systick_registers->CSR & 0xfffefff8 )
		print_and_quit( "Unused bits in CSR were incorrectly set" ) ;
	if ( p_systick_registers->CSR & 0xfffefff2 )
		print_and_quit( "TICKINT bit in CSR was incorrectly set" ) ;
	if ( p_systick_registers->CVR & 0xff000000 )
		print_and_quit( "Value over 24 bits loaded into the CVR" ) ;
	if ( p_systick_registers->RVR & 0xff000000 )
		print_and_quit( "Value over 24 bits loaded into the RVR" ) ;

}


// switch to point to the actual hardware on a Cortex M4
// Be sure to call get_systick_registers after changing this setting!
void use_hardware( void ) 
{
	p_systick_registers = (systick_registers_t *)0xE000E010 ;	// address of the first register
	hardware_pointer = hardware ;
}

// switch to using the hardware simulator
void use_simulator( void ) 
{
	hardware_pointer = simulation ;
	reset_systick() ;
	p_systick_registers = &simulated_systick_registers ;
}

// clear out the simulated registers (set to 0) and related state variable.
void reset_systick( void )
{
	memset( p_systick_registers, 0, sizeof( simulated_systick_registers ) ) ; // clear settings
	previous_enabled_state = 0 ;
}

// returns 1 if CSR has clock source bit set, 0 if not set
uint32_t is_processor_clock_set( void )
{
	uint32_t result = 0 ;

	check_for_corruption() ;
	if ( p_systick_registers->CSR & 4 )
		result = 1 ;

	return result ;
}


// returns 1 if systick is enabled, 0 if disabled
uint32_t is_systick_enabled( void )
{
	uint32_t result = 0 ;

	check_for_corruption() ;
	if ( p_systick_registers->CSR & 1 )
		result = 1 ;

	return result ;
}

// returns current value of the RVR
uint32_t get_RVR( void )
{
	check_for_corruption() ;
	return p_systick_registers->RVR ;
}

// helper function for run_simulation to clear countflag and copy RVR to CVR
static void test_for_new_enable( void )
{
	check_for_corruption() ;
	if ( previous_enabled_state == 0 && p_systick_registers->CSR & 1 )
	{
		p_systick_registers->CSR &= 0xfffeffff ;	// clear bit 16 (countflag)
		p_systick_registers->CVR = p_systick_registers->RVR ;	// init countdown counter
	}
	previous_enabled_state = p_systick_registers->CSR & 1 ;		// set previous to current enabled state
}

// run the simulated systick hardware for the passed number of cycles.
// Note that this number can be zero! Which means just look at the changed
// status bits -- especially the enable bit in the CSR.
void run_simulation( uint32_t number_of_clock_cycles )
{
	uint32_t current_value ;

	check_for_corruption() ;
	test_for_new_enable() ;

	if ( number_of_clock_cycles == 0 &&
		 p_systick_registers->CSR & 5 )			// handle special case of a newly enabled timer but no clock ticks yet
		p_systick_registers->CVR = 
			p_systick_registers->RVR ;			// copy the RVR to the CVR since that is when happens when we enable systick
		
	else if ( number_of_clock_cycles > 0 &&		// only update counter if running one or more cycles!
		p_systick_registers->CSR & 5 )			// if clocksource is processor clock and it is enabled
	{
		if ( hardware_pointer == simulation )
		{
			// this section of code decrements the CVR and does the wrap around as needed.
			current_value = p_systick_registers->CVR ;
			if ( number_of_clock_cycles < current_value )	// all we have to do is decrement it
				p_systick_registers->CVR -= number_of_clock_cycles ;
			else
			{
				// count down to 0 has occurred so set the countflag and update the CVR to it's new value
				// which may be 0 or a number slightly less than the RVR value.
				p_systick_registers->CSR |= 0x10000 ;		// set the COUNTFLAG in the CSR
				// load the CVR with the RVR but remove the number of cycles that were
				// greater than the current CVR.
				p_systick_registers->CVR = p_systick_registers->RVR - number_of_clock_cycles +
											p_systick_registers->CVR ;
			}
	
		}
	}
}

// returns pointer to the hardware or to the simulated hardware 
systick_registers_t *get_systick_registers( void )
{
	return p_systick_registers ;
} 

