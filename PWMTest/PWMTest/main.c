/*
 * PWMTest.c
 *
 * Created: 9/4/2026 15:20:57
 * Author : Cristian Juárez
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#include "PWM/PWM0.h"
void setup();


int main(void)
{
	uint8_t duty = 127;
	setup();
	initPWM0();
	
    while (1) 
    {
	updateDutyCycle0A(duty);
	updateDutyCycle0B(duty);
	duty++;
	_delay_ms(1);
    }
}

void setup()
{
	CLKPR = (1<<CLKPCE);
	CLKPR = (1<<CLKPS2);	
}
