/*
 * EjemploTMR0_C.c
 *
 * Created: 12/3/2026 15:41:42
 * Author : crist
 */

/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define TCNT0_value 100
uint8_t counter = 0;
/****************************************/
// Function prototypes
 void setup();
 void initTMR0();
/****************************************/
// Main Function

int main(void)
{
	cli();
	setup();
	//habilitar interrupciones por overflow del timer 0
	TIMSK0 |= (1<<TOEI);
	//Habilitar int globales
	sei();
	while (1)
	{
		
	}
}

/****************************************/
// NON-Interrupt subroutines
void setup()
{
	CLKPR = (1<<CLKPCE);
	CLKPR = (1<<CLKPS2);
	//Configurar salidas
	DDRC = 0X0F;
	PORTC = 0X00;
	// Configurar Timer 0 
	initTMR0();
}
void initTMR0()
{
	TCCR0A &= ~((1<<WGM01)| (1<<WGM00));
	TCCR0B &= ~(1<<WGM02);
	// Configurar preescaler = 64
	TCCR0B &= ~(1<<CS02);
	TCCR0B |= ((1<<CS01)| (1<<CS00));
	// iniciar TCN0
	TCNT0 = TCNT0_value;
}
/****************************************/
// Interrupt routines

ISR(TIMER0_OVF_vect)
{
	TCNT0 = TCNT0_value;
	counter++;
	if (counter == 50)
	{
		PORTC++;
		PORTC &= 0x0F;
		counter = 0;
	}
}

