/*
 * Laboratorio6.c
 *
 * Created: 21/4/2026 
 * Author : Cristian Juárez 
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Prototipos
void ConfigurarUART9600(void);
void EnviarCaracterUART(char c);
void cadena(char txt[]);
void MostrarEnPuertos(uint8_t dato);
void EnviarHex(uint8_t valor);
void EnviarDecimal(uint8_t valor);

volatile uint8_t datoRecibido = 0;

int main(void) {
	// Configurar puertos (PB y PD como salidas)
	DDRB = 0xFF;
	DDRD = 0xFF;

	// Inicializar UART
	ConfigurarUART9600();
	sei();

	// Mensaje inicial
	cadena("\nSistema UART - Visualizacion en PD2-PD7\n");
	cadena("Envie caracteres para verlos en:\n");
	cadena("- PB1-PB0: 2 bits mas significativos\n");
	cadena("- PD7-PD2: 6 bits menos significativos\n\n");
	cadena("Hola Mundo\n");

	while(1) {
		if(datoRecibido) {
			MostrarEnPuertos(datoRecibido);

			// Mostrar valor ASCII recibido
			cadena("Caracter recibido: ");
			cadena("ASCII decimal = ");
			EnviarDecimal(datoRecibido);
			cadena(" , Hex = 0x");
			EnviarHex(datoRecibido);
			cadena("\n");

			datoRecibido = 0;
		}
		_delay_ms(10);
	}
}

void ConfigurarUART9600(void) {
	DDRD &= ~(1<<DDD0); // PD0 (RX) como entrada
	DDRD |= (1<<DDD1);  // PD1 (TX) como salida

	UBRR0 = 103; // 9600 bps a 16MHz
	UCSR0A = 0;
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void EnviarCaracterUART(char c) {
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

void cadena(char txt[]) {
	uint8_t i = 0;
	while(txt[i] != '\0') {
		EnviarCaracterUART(txt[i]);
		i++;
	}
}

// Función para mostrar en PD2-PD7 y PB0-PB1
void MostrarEnPuertos(uint8_t dato) {
	// PD7-PD2 = bits [5:0] del dato (6 bits LSB)
	PORTD = (dato & 0x3F) << 2;

	// PB1-PB0 = bits [7:6] del dato (2 bits MSB)
	PORTB = (dato >> 6) & 0x03;

	// Mantener PD1 (TX) en estado alto
	PORTD |= (1<<PD1);
}

// Nueva función para enviar un byte en hexadecimal
void EnviarHex(uint8_t valor) {
	uint8_t nibble;

	// Nibble alto
	nibble = (valor >> 4) & 0x0F;
	EnviarCaracterUART(nibble > 9 ? nibble - 10 + 'A' : nibble + '0');

	// Nibble bajo
	nibble = valor & 0x0F;
	EnviarCaracterUART(nibble > 9 ? nibble - 10 + 'A' : nibble + '0');
}

// Nueva función para enviar un número en decimal
void EnviarDecimal(uint8_t valor) {
	char buffer[4]; // hasta 3 dígitos + terminador
	uint8_t i = 0;

	if(valor == 0) {
		EnviarCaracterUART('0');
		return;
	}

	while(valor > 0) {
		buffer[i++] = (valor % 10) + '0';
		valor /= 10;
	}

	// imprimir en orden correcto
	while(i > 0) {
		EnviarCaracterUART(buffer[--i]);
	}
}

// Interrupción de recepción UART
ISR(USART_RX_vect) {
	datoRecibido = UDR0;
}
