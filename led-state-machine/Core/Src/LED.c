#include "LED.h"


int GREEN_LED = 6;
int RED_LED = 7;

// project specific manually connected LEDs on breadboard shield.
#define ARDUINO_D12 (6)				// GPIO PA6 -- hard wired to Arduino D12
#define ARDUINO_D11 (7)				// GPIO PA7 -- hard wired to Arduino D11

// GPIO Output initialization for GPIO banks A, B, C, and D only
// Expects GPIO to be GPIOA, GPIOB, GPIOC, or GPIOD.
// pin is expected to be 0 through 15
// Note -- if the clock is not enabled the 16 GPIO pins are non-functional!
void GPIO_Output_Init( GPIO_TypeDef *GPIO, uint32_t pin )
{
	// Enable the peripheral clocks of for GPIO
	if ( GPIO == GPIOA )
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN ;
	else if ( GPIO == GPIOB )
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN ;
	else if ( GPIO == GPIOC )
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN ;
	else if ( GPIO == GPIOD )
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN ;
	else
		return ;

	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIO->MODER &= ~( (uint32_t)3 << ( 2 * pin ) ) ;  // clear to input mode (00)
	GPIO->MODER |= (uint32_t)1 << ( 2 * pin ) ;      //  Output(01)

	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIO->OSPEEDR &= ~( (uint32_t)3 << ( 2 * pin ) ) ;	// set to cleared which is low speed
	GPIO->OSPEEDR |=   (uint32_t)3 << ( 2 * pin ) ;  // High speed

	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	GPIO->OTYPER &= ~( (uint32_t)1 << pin ) ;       // Push-pull

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIO->PUPDR   &= ~( (uint32_t)3 << ( 2 * pin ) ) ;  // No pull-up, no pull-down
}

// set up PA6 and PA7 to drive LEDs
void LED_Init(void){
	GPIO_Output_Init( GPIOA, ARDUINO_D12 ) ;		// init PA6 as output (D12)
	GPIO_Output_Init( GPIOA, ARDUINO_D11 ) ;		// init PA7 as output (D11)
}

//******************************************************************************************
// Turn LED On
//******************************************************************************************
void LED_On(int LED_PIN) {
	if (LED_PIN == GREEN_LED)
		GPIOA->ODR |= (uint32_t)1 << ARDUINO_D12 ;	// PA6
	else if (LED_PIN == RED_LED)
		GPIOA->ODR |= (uint32_t)1 << ARDUINO_D11 ;	// PA7
}

//******************************************************************************************
// Turn LED Off
//******************************************************************************************
void LED_Off(int LED_PIN){
	if (LED_PIN == GREEN_LED)
		GPIOA->ODR &= ~( (uint32_t)1 << ARDUINO_D12 ) ;	// PA6
	else if (LED_PIN == RED_LED)
		GPIOA->ODR &= ~( (uint32_t)1 << ARDUINO_D11 ) ;	// PA7
}

//******************************************************************************************
// Toggle LED 
//******************************************************************************************
void LED_Toggle(int LED_PIN){
	if (LED_PIN == GREEN_LED)
		GPIOA->ODR ^= (uint32_t)1 << ARDUINO_D12 ;	// PA6
	else if (LED_PIN == RED_LED)
		GPIOA->ODR ^= (uint32_t)1 << ARDUINO_D11 ;	// PA7
}
