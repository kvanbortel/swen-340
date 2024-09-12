// struct_activity -- Using structs and pointers to structs exercise
// L. Kiser June 4, 2020
// M. Parikh Sept 6, 2021

// Compile on nitron or other gcc environments -- NO WARNINGS on your compile!
// gcc -o struct_activity -Wall struct_activity.c
// To run it:
// ./struct_activity

// compiler includes go here
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// project includes go here
#include "struct_activity.h"

// Global and static global declarations go here -- use static to hide the symbol from other .c files!
static POINT_t global_point ;

// Add your declaration for the PRODUCT_t static global here.


// parameters -- p_point is a pointer to the dot on the screen
//				move_amount is the delta shift for the x coordinate.
// Goal: Do review the two valid approaches for accessing
//              the x member of the struct from the passed in pointer.
static void move_x( POINT_t *p_point, double move_amount )
{
	// Explore the two different ways to use a pointer to a struct.
	
	// One way of addition move_amount to x 
	(*p_point).x += move_amount ;			// does the same as the previous line.
	
	// Try another way to add move_amount to x
	// hint: Use arrow operator
	
}

int main( int arg_count, char **arg_array )
{
	POINT_t origin ;
	
	// Add your declaration of PRODUCT_t here for the stack based case.

	// Initialize members of Origin
	origin.x = 10 ;
	origin.y = 20 ;
	
	printf( "x: %f\n", origin.x ) ;
	move_x( &origin, 5 ) ;
	printf( "x: %f\n", origin.x ) ;
	printf( "size of the struct %lu\n", sizeof(origin) ) ;

	// Now let's see where in memory this located:
	printf( "Location of global static origin in global memory 0x%lx \n", (unsigned long)&global_point ) ;
	printf( "Location of origin on the stack 0x%lx \n", (unsigned long)&origin ) ;
	printf( "Location of origin.version on the stack 0x%lx \n", (unsigned long)&origin.version ) ;
	printf( "Location of origin.x on the stack 0x%lx \n", (unsigned long)&origin.x ) ;
	printf( "Location of origin.y on the stack 0x%lx \n", (unsigned long)&origin.y ) ;

	// Add printf statements here for your static global PRODUCT_t and for your stack based PRODUCT_t
	// Do one printf for the address of each member of PRODUCT_t for both the static global and stack based struct.
	// This means a total of 4 address printf statements for each of the instances of PRODUCT_t
	// One for the struct itself and one for each of the three members.

	return 0 ; // return success
}

