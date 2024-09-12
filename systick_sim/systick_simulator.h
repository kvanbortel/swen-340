// systick_simulator for Cortex M4
// Includes ability to switch to the actual hardware.
// Larry Kiser copyright 2021

typedef unsigned int uint32_t ;

enum simulator_control
{
	simulation,
	hardware 
} ;

typedef struct
{
	uint32_t CSR ;		// control and status register
	uint32_t RVR ;		// reload value register
	uint32_t CVR ;		// current value register
	const uint32_t CALIB ;	// unused

} systick_registers_t ;

// function prototypes
void reset_systick( void ) ;
void use_hardware( void ) ;
void use_simulator( void ) ;
void run_simulation( uint32_t number_of_clock_cycles ) ;
systick_registers_t *get_systick_registers( void ) ;
uint32_t is_processor_clock_set( void ) ;
uint32_t is_systick_enabled( void ) ;
uint32_t get_RVR( void ) ;

