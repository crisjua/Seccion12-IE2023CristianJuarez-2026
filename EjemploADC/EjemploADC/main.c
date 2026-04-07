/*
 * EjemploADC.c
 *
 * Created: 19/3/2026 15:44:26
 * Author : crist
 */ 
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>

/****************************************/
// Function prototypes
void setup();
void initADC();
void initTMR1();
/****************************************/
// Main Function
int main(void)
{
	cli();
	
	sei();
	setup();
	initADC();
	// Iniciar conv. ADC y Habilitar interrupciones
	ADCSRA |= (1<<ADSC) | (1<<ADIE);
	TIMSK1 = (1<<TOEI1);
	
	 
	while (1)
	{
	}
}
/****************************************/
// NON-Interrupt subroutines
void setup()
{
	//Preescaler_CPU = 16 F_cpu = 1MHZ
	CLKPR = (1<<CLKPCE);
	CLKPR = (1<<CLKPS2);
	// Configurar Salidas (DDRD)
	DDRD = 0xFF; // Todo el puerto D como salida
	PORTD = 0x00; // Todo el puerto D apagado
	UCSR0B = 0x00; // Apgar PD0 Y PD1
}
void initADC()
{
	// Borrar ADMUX
	ADMUX = 0;
	//Vref = AVcc; Just. a la izq; ADC = ADC6;
	ADMUX |= (1<<REFS0) | (1<<ADLAR) | (1<<MUX2) | (1<<MUX1);
	// Borrar ADCSRA
	ADCSRA = 0;
	// Habilitar ADC y Prescaler = 8 -> 1Mhz = 125 kHz
	ADCSRA |= (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);
	
}
void initTMR1()
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B = (1<<CS11) | (1<<CS01);
}
/****************************************/
// Interrupt routines

ISR(ADC_vect)
{
	PORTD = ADCH;
	ADCSRA |= (1<<ADSC);
}

ISR(TIMER0_OVF_vect)
{
	TCNT1 = T1value;
	ADCSRA |= (1<<ADSC);
	
}