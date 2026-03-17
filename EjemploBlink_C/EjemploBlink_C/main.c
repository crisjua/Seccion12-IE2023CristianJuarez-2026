/*
 * EjemploBlink_C.c
 *
 * Created: 12/3/2026 15:20:12
 * Author : crist
*/
// Encabezado (Libraries)
#include <avr/io.h>
#include <stdint.h>
/****************************************/
// Function prototypes
void setup();
void delay();
/****************************************/
// Main Function
int main(void)
{
	/* Replace with your application code */
	setup();
	while (1)
	{
		//PORTC ^= (1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0); toggle
		//PINC |= (1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0); toggle
		PORTC |= (1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0);
		delay();
		PORTC &= ~((1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0));
		delay();
	}
}
/****************************************/
// NON-Interrupt subroutines
void setup()
{
	// Configurar prescaler = 16 -> 16 MHZ / 16 = 1 MHZ
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS2);
	// Configurar OUTPUT
	DDRC |= (1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
	PORTC &= ~((1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0));
}
void delay()
{
	for (volatile uint8_t i = 0; i < 255; i++)
	{
		for(volatile uint8_t j = 0; j < 255; j ++)
		{	
		}
	}
}
/****************************************/
// Interrupt routines



