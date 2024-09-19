// systick code to be updated by students
// Larry Kiser copyright 2021

#include "systick.h"


// Test 1
// STUDENT: Implement this as described
int clock_enable_test( void )
{
	int result = 0 ; 		// default to failure

	// STUDENT: first get the pointer to the systick registers using the get systick registers function.
	// 			next set the clock source to the processor clock
	systick_registers_t *p_registers = get_systick_registers();
	p_registers->CSR |= 1 << 2; // according to manual, bit 2 (from 0) is CLKSOURCE,
								// 0 = external clock; 1 = processor clock
								// so shift a 1 two places and | it
	
	// STUDENT now confirm that it is set to processor clock by using the is_processor_clock_set function
	//         if we do get that is set correctly return 1 from this function
	result = is_processor_clock_set();

	return result ;
}
  

// Test 2
// STUDENT: Implement this as described -- NOTE -- implement Test 1 FIRST!
int set_reload_register_test( uint32_t reload_value )
{
	int result = 0 ; 		// default to failure

	// STUDENT: Use the clock_enable_test first to set up the processor clock
	//			if the clock enable test is successful do the rest of the code.


		// STUDENT: first get the pointer to the systick registers using the get systick registers function.
		//			now set the reload register to the passed reload_value
	systick_registers_t *p_registers = get_systick_registers();
	p_registers->RVR = reload_value;

		// STUDENT now confirm that the RVR value is correct using the get_RVR()
	result = (reload_value == get_RVR());

	return result ;
}

// Test 3
// STUDENT: Implement this as described. This depends Test 2 which depends on Test 1
int enable_timer_test( uint32_t reload_value )
{
	int result = 0 ;		// default to failure

	// STUDENT: call Test 2 with the passed reload_value
	//			if it passes do the rest of the code
	if (!set_reload_register_test(reload_value))
		return result;

		// STUDENT: first get the pointer to the systick registers using the get systick registers function.
		//			now set the enable bit in the CSR
		//			Now call the run_simulation function with 0 for the number of clock cycles which causes
		//			the simulator to process the fact that we have enabled systick but have not yet
		//			processed any clock cycles.
		//			Read the SysTick manual -- what is supposed to happen to the CVR and RVR registers when
		//			the enable bit is set? Use the registers pointer with the CVR and RVR member registers to
		//			verify that the values are correct (do not use the get_RVR function).
	systick_registers_t *p_registers = get_systick_registers();
	p_registers->CSR |= 1; // 0th but is ENABLE; 1 = enabled and 0 = disabled
	run_simulation(0);
	// When ENABLE is set to 1, the counter loads the RELOAD value from the SYST_RVR register
	// and then counts down.
	// The SYST_CVR register contains the current value of the SysTick counter.
	if (p_registers->CVR == reload_value && p_registers->RVR == reload_value)
		result = 1;

	return result ;
}

// Test 4 
// STUDENT: Implement this as described
int run_simulation_test( void ) 
{
	int result = 0 ;		// default to failure

	reset_systick() ;		// start fresh

	// STUDENT: call enable_timer_test with 3 cycle count. If it returns success then continue
	if (!enable_timer_test(3))
		return result;

		// STUDENT: first get the pointer to the systick registers using the get systick registers function.
		// 		    now run the simulation for 2 cycles. What should be the values of the RVR and CVR registers?
		//			Write tests to check those two values are correct. Verify that the COUNTFLAG bit in the CSR
		//			is still a 0.
	systick_registers_t *p_registers = get_systick_registers();
	run_simulation(2);
	// RVR doesn't change
	// CVR should be 1. It was 3 and 2 cycles ran.
	// COUNTFLAG is 16th bit; Returns 1 if timer counted to 0 since last time this was read. Should be 0.
	if (p_registers->RVR != 3 || p_registers->CVR != 1 || (p_registers->CSR & (1 << 16)))
		return result;


			// STUDENT: now run the simulation for one more clock cycle. Verify that the COUNTFLAG bit is now
			//			set. What are the expected changes in the RVR and CVR registers? Test that the
			//			values of the RVR and CVR registers are expected.
	run_simulation(1);
	// RVR stays the same
	// CVR was 1 and 1 cycle ran so now it is back to 3
	// CSR should be 1
	if (p_registers->RVR == 3 && p_registers->CVR == 3 && (p_registers->CSR & (1 << 16)))
		result = 1;

	return result ;
}


