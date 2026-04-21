/*
 * Laboratorio5.c
 * Created: 14/4/2026 
 * Author : Cristian Juárez 
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

// Librerías de servos (NO se tocan)
#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"

// Librería del LED (nueva)
#include "LED/LEDPWM.h"

// ADC
void initADC(void);
uint8_t readADC(uint8_t canal);

int main(void)
{
	cli();

	// ---------------- PINES ----------------
	DDRB |= (1 << PORTB2); // Servo1
	DDRD |= (1 << PORTD3); // Servo2
	DDRC = 0;              // Entradas ADC

	// ---------------- PWM SERVOS  ----------------
	initFastPWM1(settedUp, 8);
	channel(channelB, nop);
	topValue(39999);

	initFastPWM2(nop, 256);
	channel2(channelB, nop);

	// ---------------- PWM LED (modularizado) ----------------
	initTimer0_LEDPWM();

	// ---------------- ADC ----------------
	initADC();

	sei();

	while (1)
	{
		// Servo 1 (PC0)
		uint8_t valorPC0 = readADC(0);
		convertServo(valorPC0, channelB);

		// Servo 2 (PC1)
		uint8_t valorPC1 = readADC(1);
		convertServo2(valorPC1, channelB);

		// LED (PC2)
		updateLEDBrightness(readADC(2));

		_delay_ms(10);
	}
}

// ---------------- ADC ----------------
void initADC(void)
{
	ADMUX = 0;
	ADMUX |= (1 << REFS0);
	ADMUX |= (1 << ADLAR);

	ADCSRA = 0;
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	DIDR0 |= (1 << ADC0D) | (1 << ADC1D) | (1 << ADC2D);
}

uint8_t readADC(uint8_t canal)
{
	canal &= 0x07;
	ADMUX = (ADMUX & 0xF0) | canal;

	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));

	return ADCH; // 8 bits
}