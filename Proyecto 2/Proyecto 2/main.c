/*
 * Proyecto 2.c
 *
 * Created: 28/4/2026 
 * Author : Cristian Juárez 
 */ 
//==============================================================
//                      LIBRERIAS
//==============================================================

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdlib.h>

#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"

//==============================================================
//                      DEFINICION DE PINES
//==============================================================

#define adelanteDC	PINB0
#define servo1		PINB1
#define servo2		PINB2
#define velDC		PINB3
#define atrasDC		PINB4

#define potServo1	PINC0
#define potServo2	PINC1
#define potServo3	PINC2
#define potVelDC	PINC3

#define bridge1		PIND2
#define servo3		PIND3
#define bridge2		PIND4

#define led1		PIND5
#define led2		PIND6

#define savePos		PIND7

//==============================================================
//                      CONSTANTES
//==============================================================

#define manualMode	1
#define eepromMode	2
#define usartMode	3

#define address1	0x06
#define address2	0x05
#define address3	0x02
#define address4	0x03

//==============================================================
//                      VARIABLES
//==============================================================

volatile char bufferRX;

uint8_t count = 0;
uint8_t currentMode = manualMode;

uint8_t pos1 = 0;
uint8_t pos2 = 0;
uint8_t pos3 = 0;
uint8_t velMotor = 0;

// UART BUFFER
char buffer[10];
uint8_t rxIndex = 0;

//==============================================================
//                      PROTOTIPOS
//==============================================================

void initADC(void);
void initUART9600(void);
void writeText(char* text);

//==============================================================
//                      MAIN
//==============================================================

int main(void){

	cli();

	//==========================================================
	// PORTB
	//==========================================================

	// MOTOR DC DIRECCION
	DDRB |= (1 << adelanteDC) | (1 << atrasDC);

	PORTB &= ~((1 << adelanteDC) | (1 << atrasDC));

	// SERVOS
	DDRB |= (1 << servo1) | (1 << servo2);

	//==========================================================
	// PORTC
	//==========================================================

	DDRC = 0x00;

	//==========================================================
	// PORTD
	//==========================================================

	PORTD |= (1 << savePos);

	DDRD &= ~(1 << savePos);

	DDRD |= (1 << bridge1) |
	(1 << bridge2) |
	(1 << led1) |
	(1 << led2);

	//==========================================================
	// INICIALIZACIONES
	//==========================================================

	initADC();

	initUART9600();

	// PWM1
	initFastPWM1(settedUp, 8);

	channel(channelA, nop);
	channel(channelB, nop);

	topValue(39999);

	// PWM2
	initPWM2A(no_invertido, 1024);
	initPWM2B(no_invertido, 1024);

	sei();

	//==========================================================
	// MENSAJE INICIAL
	//==========================================================

	writeText("Conectado\n");

	// LED modo manual
	PORTD |= (1 << led1);

	PORTD &= ~(1 << led2);

	// START ADC
	ADCSRA |= (1 << ADSC);

	//==========================================================
	// LOOP
	//==========================================================

	while(1){

	}
}

//==============================================================
//                      ADC
//==============================================================

void initADC(void){

	ADMUX = 0;

	ADMUX |= (1 << REFS0);

	ADMUX |= (1 << ADLAR);

	ADCSRA = 0;

	ADCSRA |= (1 << ADEN);

	ADCSRA |= (1 << ADIE);

	ADCSRA |= (1 << ADPS2) |
	(1 << ADPS1) |
	(1 << ADPS0);

	DIDR0 |= (1 << ADC0D) |
	(1 << ADC1D) |
	(1 << ADC2D) |
	(1 << ADC3D);
}

//==============================================================
//                      ADC ISR
//==============================================================

ISR(ADC_vect){

	ADCSRA |= (1 << ADIF);

	// SOLO MODO MANUAL
	if(currentMode == manualMode){

		//==========================================
		// SERVO1
		//==========================================

		if(count == 0){

			count = 1;

			ADMUX = (ADMUX & 0xF0);

			pos1 = ADCH;

			convertServo(ADCH, channelB);
		}

		//==========================================
		// SERVO2
		//==========================================

		else if(count == 1){

			count = 2;

			ADMUX = (ADMUX & 0xF0) | 1;

			pos2 = ADCH;

			convertServo(ADCH, channelA);
		}

		//==========================================
		// SERVO3
		//==========================================

		else if(count == 2){

			count = 3;

			ADMUX = (ADMUX & 0xF0) | 2;

			pos3 = ADCH;

			updateDutyCB2(ADCH);
		}

		//==========================================
		// VELOCIDAD
		//==========================================

		else if(count == 3){

			count = 0;

			ADMUX = (ADMUX & 0xF0) | 3;

			velMotor = ADCH;

			OCR2A = ADCH;
		}
	}

	ADCSRA |= (1 << ADSC);
}

//==============================================================
//                      UART
//==============================================================

void initUART9600(void){

	// RX INPUT
	DDRD &= ~(1 << DDD0);

	// TX OUTPUT
	DDRD |= (1 << DDD1);

	// NORMAL MODE
	UCSR0A = 0;

	// ENABLE RX TX
	UCSR0B = 0;

	UCSR0B |= (1 << RXCIE0) |
	(1 << RXEN0) |
	(1 << TXEN0);

	// 8 BITS
	UCSR0C = 0;

	UCSR0C |= (1 << UCSZ01) |
	(1 << UCSZ00);

	// 9600 BAUD
	UBRR0 = 103;
}

//==============================================================
//                      UART TX
//==============================================================

void writeText(char* text){

	for(uint8_t i = 0; text[i] != '\0'; i++){

		while(!(UCSR0A & (1 << UDRE0)));

		UDR0 = text[i];
	}
}

//==============================================================
//                      UART RX ISR
//==============================================================

ISR(USART_RX_vect){

	char c = UDR0;

	//==========================================================
	// FIN DE COMANDO
	//==========================================================

	if(c == '\n'){

		buffer[rxIndex] = '\0';

		char comando = buffer[0];

		int valor = atoi(&buffer[1]);

		//======================================================
		// CAMBIO DE MODOS
		//======================================================

		if(comando == '1'){

			currentMode = manualMode;

			PORTD |= (1 << led1);

			PORTD &= ~(1 << led2);
		}

		else if(comando == '2'){

			currentMode = eepromMode;

			PORTD |= (1 << led2);

			PORTD &= ~(1 << led1);

			// LEER EEPROM

			pos1 = eeprom_read_byte((uint8_t*)address1);

			pos2 = eeprom_read_byte((uint8_t*)address2);

			pos3 = eeprom_read_byte((uint8_t*)address3);

			velMotor = eeprom_read_byte((uint8_t*)address4);

			convertServo(pos1, channelB);

			convertServo(pos2, channelA);

			updateDutyCB2(pos3);

			OCR2A = velMotor;
		}

		else if(comando == '3'){

			currentMode = usartMode;

			PORTD |= (1 << led1) | (1 << led2);
		}

		//======================================================
		// CONTROL UART
		//======================================================

		if(currentMode == usartMode ||
		currentMode == manualMode){

			// SERVO1
			if(comando == 'A'){

				convertServo(valor, channelB);
			}

			// SERVO2
			else if(comando == 'B'){

				convertServo(valor, channelA);
			}

			// SERVO3
			else if(comando == 'C'){

				updateDutyCB2(valor);
			}

			// VELOCIDAD
			else if(comando == 'D'){

				OCR2A = valor;
			}

			// GUARDAR EEPROM
			else if(comando == 'S'){

				eeprom_update_byte((uint8_t*)address1, pos1);

				eeprom_update_byte((uint8_t*)address2, pos2);

				eeprom_update_byte((uint8_t*)address3, pos3);

				eeprom_update_byte((uint8_t*)address4, velMotor);
			}

			// RECUPERAR EEPROM
			else if(comando == 'R'){

				pos1 = eeprom_read_byte((uint8_t*)address1);

				pos2 = eeprom_read_byte((uint8_t*)address2);

				pos3 = eeprom_read_byte((uint8_t*)address3);

				velMotor = eeprom_read_byte((uint8_t*)address4);

				convertServo(pos1, channelB);

				convertServo(pos2, channelA);

				updateDutyCB2(pos3);

				OCR2A = velMotor;
			}

			// MOTOR ADELANTE
			else if(comando == 'F'){

				PORTB |= (1 << adelanteDC);

				PORTB &= ~(1 << atrasDC);
			}

			// MOTOR ATRAS
			else if(comando == 'T'){

				PORTB |= (1 << atrasDC);

				PORTB &= ~(1 << adelanteDC);
			}

			// MOTOR STOP
			else if(comando == 'P'){

				PORTB &= ~(1 << adelanteDC);

				PORTB &= ~(1 << atrasDC);
			}
		}

		rxIndex = 0;
	}

	//==========================================================
	// GUARDAR CARACTER
	//==========================================================

	else{

		buffer[rxIndex++] = c;

		// PROTECCION OVERFLOW
		if(rxIndex >= 9){

			rxIndex = 0;
		}
	}
}