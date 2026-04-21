/*
 * LEDPWM.c
 *
 * Created: 20/4/2026 
 *  Author: Cristian Juárez
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LEDPWM.h"

// Variables 
volatile uint8_t pwmCounter = 0;
volatile uint8_t pwmValue = 0;

void initTimer0_LEDPWM(void)
{
	TCCR0A = (1 << WGM01); // CTC
	TCCR0B = (1 << CS00);  // Sin prescaler

	OCR0A = 255;
	TIMSK0 = (1 << OCIE0A);

	DDRB |= (1 << LED_PIN);
}

// ISR 
ISR(TIMER0_COMPA_vect)
{
	pwmCounter++;

	if (pwmCounter == 0)
	PORTB |= (1 << LED_PIN);

	if (pwmCounter == pwmValue)
	PORTB &= ~(1 << LED_PIN);
}

void updateLEDBrightness(uint8_t brightness)
{
	pwmValue = brightness;
}