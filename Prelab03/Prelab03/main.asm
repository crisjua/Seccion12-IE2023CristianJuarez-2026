/*
* Interrupciones.asm
*
* Creado: 17/02/2026
* Autor : Cristian Juárez
* Descripción: 
*/;Contador binario de 4 bits utilizando 2 pushbuttons y 4 leds Utilizando interrupciones on-change y pull ups internos
;Un boton Incrementa y el otro Decrementa
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)


.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.Cseg
.org    0x00
		JMP SETUP		// RESET VECTOR

.org	0x0008
		JMP ISR_PCINT1	// ISR: Vector de dirección puertos C
// Configuracion MCU
SETUP:
	//STACK
	LDI     R16, LOW(RAMEND)
	OUT     SPL, R16
	LDI     R16, HIGH(RAMEND)
	OUT     SPH, R16
	


	LDI		R16, 0xFF	// Puerto B como salida
	OUT		DDRB, R16
	LDI		R16, 0x00
	OUT		PORTB, R16

	LDI		R16, 0xFF	// Puerto D como salida
	OUT		DDRB, R16

	LDI		R16, 0x00	// Entradas en el puerto C
	OUT		DDRC, R16
	LDI		R16, 0b00111111	// Activamos Pull- up
	OUT		PORTC, R16

	LDI		R16, (1 << PCINT8) | (1<<PCINT9)	// Activación de puertos PC0 Y PC1
	STS		PCMSK1, R16

	LDI		R16, (1 << PCIE1)
	STS		PCICR, R16		// Cargar Interrupción en los puertos

	SEI
	LDI		R20, 0x00

MAIN_LOOP:
	OUT		PORTB, R20		//SHOW COUNT	0b0000_0100
	RJMP	MAIN_LOOP

ISR_PCINT1:
	PUSH	R16				//Guarda valores de registro y banderas
	IN		R16, SREG
	PUSH	R16
	
	IN		R18, PINC

	SBRC	R18, PC0		//Lectura pin PC0, si es 0 saltar
	JMP		CHECKPC1
	CALL	DELAY
	INC		R20
	CPI		R20, 16			//Limite del Contador
	BRNE	EXIT
	LDI		R20, 0
	JMP		EXIT

CHECKPC1:
	SBRS	R18, PC1		// Si PC1 = 1 (no presionado) salir
	RJMP	DO_DEC
	RJMP	EXIT

DO_DEC:
	DEC		R20
	CPI		R20, 0xFF
	BRNE	EXIT
	LDI		R20, 15

EXIT:
	SBI		PCIFR, PCIF1	//Apaga la bandera de interrupción del puerto

	POP		R16				//Restaura valores de resgitros y banderas
	OUT		SREG, R16
	POP		R16
	RETI

DELAY:
	LDI		R24, 0x00
	LDI		R25, 0x00
	LDI		R26, 0XEE
BUCLE:
	INC		R24
	CPI		R24, 0
	BRNE	BUCLE
	INC		R25
	CPI		R25, 0
	BRNE	BUCLE
	INC		R26
	CPI		R26, 0
	BRNE	BUCLE
	RET

