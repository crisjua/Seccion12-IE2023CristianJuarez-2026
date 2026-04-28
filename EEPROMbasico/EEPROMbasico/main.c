/*
 * EEPROMbasico.c
 *
 * Created: 23/4/2026 14:41:23
 * Author : crist
 */ 


 /****************************************/
 // Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>

 /****************************************/
 // Function prototypes
void writeEEPROM(uint16_t direccion, uint8_t dato);
uint8_t readEEPROM(uint16_t direccion);
uint16_t dir_eeprom = 0;
const char* L1_ON = "L1:1";
const char* L1_ON = "L1:0";
const char* L1_ON = "L1:1";
const char* L1_ON = "L1:0";

char* string_rect "----";

uint8_t num_cara = 0;
 /****************************************/
 // Main Function
int main(void)
{
	cli();
	DDRD |= ((1<<DDD6) | (1<<DDD5));
	DDRD &= ~((1<<DDD6) | (1<<DDD5));
	initUART();
	sei();
	uint8_t lectura = readEEPROM((dir_eeprom);
	/* Replace with your application code */
	while (lectura != 0XFF)
	{
		writeChar(lectura);
		writeEEPROM(dir_eeprom, 0xFF);
		dir_eeprom++;
		lectura = readEEPROM(dir_eeprom);
	}
	dir_eeprom = 0;
	while (1)
	{
		
	}
}
 /****************************************/
 // NON-Interrupt subroutines
void writeEEPROM(uint16_t direccion, uint8_t dato)
{
	while(EECR & (1<< EEPE));
	EEAR = direccion;
	EEDR = dato;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}
uint8_t readEEPROM(uint16_t direccion)
{
		while(EECR & (1<< EEPE));
		EEAR = direccion;
		EECR |= (1<<EERE);
		return EEDR;
		
}


 /****************************************/
 // Interrupt routines
ISR(USART_RX_vect)
{
	uint8_t rx_buffer = UDR0
	if(rx_buffer != "\n")
	{
		writeChar(rx_buffer);
		*(string_rect + num_cara) = rex_buffer;
		num_cara++;
		if (*string_rect+0) == *(L1_ON) &&
			(*string_rect+1) == *(L1_ON+1) &&
			(*string_rect+2) == *(L1_ON+2)
			(*string_rect+3) == *(L1_ON+3))
		{
			PORTD |= (1<<PORTD)
		}else{
			*(string_rect+i) = "-";
		}
		num_cara = 0;
		}
		
		*//if (rx_buffer == "a")
		//{
			//PORTD ^= (1<<PORTD5);
		//}
		//if (rx_buffer == "b")
		//{
			//PORTD ^= (1<<PORTD6) 
		//}
	}
	
	
	//writeEEPROM(dir_eeprom, rx_buffer);
	//dir_eeprom++;
}


ISR(PCINT2_vect)
{
	uint8_t estadoPIND = PIND (1<<PIND2);
	if(estadoPIND == 1<<PIND2))
	{
		
	}
}