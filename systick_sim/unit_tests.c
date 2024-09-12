// unit_tests.c
// Larry Kiser October 30, 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "systick.h"
#include "unit_tests.h"

// Simple boolean assert function for unit testing
int assert( int test_result, char error_format[], ... ) {
	va_list arguments ;
	static int test_number = 1 ;
	int result = 1 ;	// return 1 for test passed or 0 if failed
	
	if ( ! test_result ) {
		va_start( arguments, error_format ) ;
		printf( "Test # %d failed: ", test_number ) ;
		vprintf( error_format, arguments ) ;
		printf( "\n" ) ;
		result = 0 ;
	}
	test_number++ ;
	return result ;
}

//////////////////////////////////////////////////////////////////////////
// Begin unit tests //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// do the unit tests
int test()
{
	int passcount = 0 ;
	int failcount = 0 ;
	
	use_simulator() ;

	// Test 1
	assert( clock_enable_test() == 1,
		"clock enable test failed" )
		? passcount++ : failcount++ ;
	// Test 2
	assert( set_reload_register_test( 10 ) == 1,
		"set_reload_register_test failed" )
		? passcount++ : failcount++ ;
	// Test 3
	assert( enable_timer_test( 5 ) == 1,
		"enable_timer_test failed" )
		? passcount++ : failcount++ ;
	// Test 4
	enable_timer_test( 3 ) ;		// set up for 3 cycle count down to 0
	assert( run_simulation_test() == 1,
		"run_simulation_test failed" )
		? passcount++ : failcount++ ;
	
	// Test 5
	// Test 6
	// Test 7
	// Test 8
	// Test 9
	// Test 10
	// Test 11
	// Test 12
	// Test 13
	// Test 14
	// Test 15
	// Test 16
	// Test 17
	// Test 18
	// Test 19
	
		
	printf( "\nSummary of unit tests:\n%d tests passed\n%d tests failed\n\n", passcount, failcount ) ;
	
	return failcount ;
}
