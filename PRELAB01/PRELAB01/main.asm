/*
* Anirrebote y Sumador.asm
*
* Creado: 3/02/2026
* Autor : Cristian Juárez
* Descripción: 
*/;El programa consiste en un contador binario de 4 bits, utilizando dos pushbutton para aumentar y decrementar el contador
;con antirrebote
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.dseg
.org    SRAM_START
//variable_name:     .byte   1   // Memory alocation for variable_name:     .byte   (byte size)

.cseg
.org 0x0000

.equ	SUBIR = PC3
.equ	BAJAR = PC2
.equ	SUBIR1 = PC1
.equ	BAJAR1 = PC0
.equ	SUMAR = PC4
 /****************************************/
// Configuración de la pila
LDI     R16, LOW(RAMEND)
OUT     SPL, R16
LDI     R16, HIGH(RAMEND)
OUT     SPH, R16

/****************************************/
// Configuracion MCU
SETUP:
	LDI		R16, 0x00
	STS		UCSR0B, R16 // Desactivar USART para usar como I/O
	
	// Puerto D
	LDI		R16, 0xFF
	OUT		DDRD, R16 // Puerto D Configurado como salida
	LDI		R16, 0x00
	OUT		PORTD, R16 // Apagado todo

	// Puerto B
	LDI		R16, 0xFF
	OUT		DDRB, R16 // Puerto B Configurado como salida
	LDI		R16, 0x00
	OUT		PORTB, R16 // Apagado Todo 

	// Prescalador 
	LDI		R16, (1<<CLKPCE)
	STS		CLKPR, R22			//Activamos el Prescalador
	LDI		R16, (1 << CLKPS2)
	STS		CLKPR, R22			//Prescalador de 1 Mhz
		
	// Puerto C
	LDI		R16, 0x00
	OUT		DDRC, R16 // Puerto C Configurado como entrada
	LDI		R16, 0xFF
	OUT		PORTC, R16 // Pull-up Activo

	// Valores de Salidas
	LDI		R17, 0x00
	LDI		R20, 0x00


/****************************************/
// Loop Infinito
MAIN_LOOP:
	SBI		DDRD, PD1
	CBI		PORTD, PD1

	SBIS	PINC, SUBIR // si pc3 es 0 saltar
	CALL	INCREMENTO1	
	SBIS	PINC, BAJAR // si pc2 es 0 saltar
	CALL	DECREMENTO1
	SBIS	PINC, SUBIR1 // si pc1 es 0 saltar
	CALL	INCREMENTO2
	SBIS	PINC, BAJAR1 //si pc0 es 0 saltar
	CALL	DECREMENTO2
	SBIS	PINC, SUMAR
	CALL	SUMANDO
	RJMP    MAIN_LOOP

INCREMENTO1:
	CALL	DELAY
	CALL	DELAY
	CALL	DELAY
	INC		R17
	CALL	DELAY
	CALL	DELAY
	CALL	DELAY
	CALL	UNION
	RET
DECREMENTO1:
	CALL	DELAY
	CALL	DELAY
	CALL	DELAY
	DEC		R17
	CALL	DELAY
	CALL	DELAY
	CALL	DELAY
	CALL	UNION
	RET
INCREMENTO2:
	CALL	DELAY
	CALL	DELAY
	CALL	DELAY
	INC		R20
	CALL	DELAY
	CALL	DELAY
	CALL	DELAY
	CALL	UNION
	RET
DECREMENTO2:
	CALL	DELAY
	CALL	DELAY
	CALL	DELAY
	DEC		R20
	CALL	DELAY
	CALL	DELAY
	CALL	DELAY
	CALL	UNION
	RET

UNION:
	ANDI	R17, 0x0F // Utilizar solo los primeros 4 bits
	ANDI	R20, 0x0F
	SWAP	R20		 // Cabiar los primeros bits por los ultimos
	MOV		R21, R17
	OR		R21, R20 // Unir ambos registros
	SWAP	R20		 // Regresar registro a su estado original
	OUT		PORTD, R21 // Mostrar en Salida
	RET
SUMANDO:
	CALL	DELAY
	MOV		R23, R17
	ADD		R23, R20
	ANDI	R23, 0x1F
	OUT		PORTB, R23 // Se habilita overflow
	RET

	
/****************************************/
// NON-Interrupt subroutines
DELAY:
	LDI		R18, 255
	LDI		R19, 255
BUCLE:
	DEC		R18
	CPI		R18, 255
	BRNE	BUCLE
	DEC		R19
	CPI		R19, 255
	BRNE	BUCLE
	DEC		R19
	CPI		R19, 255
	DEC		R19
	CPI		R19, 255
	DEC		R19
	CPI		R19, 255
	DEC		R19
	CPI		R19, 255
	DEC		R19
	CPI		R19, 255
	RET
/****************************************/
// Interrupt routines

/****************************************/