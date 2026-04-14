/*
 * PWM0.c
 *
 * Created: 9/4/2026 15:48:29
 *  Author: crist
 */ 

#include "PWM"
void initPWM0()
{
	DDRD |= (1<<DDD6);
	TCCR0A = 0;
	TCCR0B = 0;
	
	if (invertido)
	{
		TCCR0A |= (1<<WGM01); // FAST PWM
	} 
	else
	{
		TCCR0A |= (1<<WGM01) | (1<<WGM00); // FAST PWM
	}
	// No invertido
	TCCR0A |= (1<<COM0A1); // No invertido PD6
	
	
	TCCR0A |= (1<<WGM01) | (1<<WGM00); // FAST PWM
	
	TCCR0B |= (1<<CS01); // Prescaler = 8
	
}
void updateDutyCycle0A(uint8_t ciclo)
{
	OCR0A = ciclo;
}
void updateDutyCycle0B(uint8_t ciclo)
{
	OCR0B = ciclo;
}