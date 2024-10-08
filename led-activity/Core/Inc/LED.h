#ifndef __NUCLEO476_LED_H
#define __NUCLEO476_LED_H

#include "stm32l476xx.h"

extern int NUCLEO_GREEN_LED_PIN;
extern int NUCLEO_RED_LED_PIN;

void LED_Init(void);

void LED_On(int);
void LED_Off(int);
void LED_Toggle(int);

#endif /* __NUCLEO476_LED_H */
