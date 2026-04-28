/*
 * UARTTEST.c
 *
 * Created: 16/4/2026 15:27:37
 * Author : crist
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
void initUART();
void writeChar(char c);
void writeString(char* string);

int main(void)
{
	cli();
	DDRB |= (1<<DDB5);
	PORTB &= ~(1<<PORTB5);
	DDRB |= (1<<DDB5);
	PORTB &= ~(1<<PORTB5);
	
    writeChar("H");
	writeChar("O");
	writeChar("L");
	writeChar("A");
    while (1) 
    {
		
    }
void initUART()
{
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	UCSR0A = 0;
	
	UCSR0B = (1<<RXCIE0) | (1 << RXEN0) | (1<<TXEN0 );
	
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	
	UBRR0 = 103;
}
void writeChar(char c)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}
void writeString(char* string)
{
	for(uint8_t = 0; *(string + i) != "\0"; i++){
		writeChar(string[i]);
	}

ISR(USART_RX_vect){
	uint8_t bufferRX = UDR0;
	writeChar(bufferRX);
	if(bufferRX == "a")
	{
		PORTB |= (1<<PORTB5)
		PORTB |= (1<<PORTB5)
	
	}
}	

}

