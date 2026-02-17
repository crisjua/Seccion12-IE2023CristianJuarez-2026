/*
* Botones y Timer 0.asm
*
* Creado: 10/02/2026
* Autor : Cristian Juárez
* Descripción: 
*/;Contador binarios de 4 bits con un incremento cada 100 ms
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.dseg
.org    SRAM_START
//variable_name:     .byte   1   // Memory alocation for variable_name:     .byte   (byte size)

.cseg
.org 0x0000


 /****************************************/
// Configuración de la pila
LDI     R16, LOW(RAMEND)
OUT     SPL, R16
LDI     R16, HIGH(RAMEND)
OUT     SPH, R16

/****************************************/
// Configuracion MCU
SETUP:
	LDI		R16, 0x3F
	OUT		DDRB, R16 // Primeros dos entrada y los siguientes 6 salida
	LDI		R16, 0x00
	OUT		PORTB, R16 // Todo desactivado

	CALL	TIMER

	LDI		R20, 0
	LDI		R30, 0

/****************************************/
// Loop Infinito
MAIN_LOOP:
	IN		R16, TIFR0		// Lee el registro de banderas de interrupción
	CPI		R16, (1 << TOV0) // Compara bandera de Overflow esta activa
	BRNE	MAIN_LOOP // Espera la interrupcion

	CPI		R20, 16	 
	BREQ	ZERO

	LDI		R16, 100 // Carga el valor hasta donde cuenta el timer
	OUT		TCNT0, R16 //iniciar en el valor especifico

	SBI		TIFR0, TOV0 // Borrar bandera de Overflow

	INC		R30
	CPI		R30, 100
	BRNE	MAIN_LOOP
	CLR		R30

	INC		R20
	MOV		R25, R20
	OUT		PORTB, R25

	RJMP	MAIN_LOOP
	

// Interrupt routines
ZERO:
	LDI		R20, 0
	RET

TIMER:
	LDI		R16, (1 << CS01) | (1 << CS00) 	// Preescalador de 256
	OUT		TCCR0B, R16

	LDI		R16, 100		// Valor de Overflow
	OUT		TCNT0, R16		// Cargo valor Inicial
	RET
/****************************************/