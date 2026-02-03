
/*
* NombreProgra.asm
*
* Creado: 
* Autor : 
* Descripción: 
*/
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
    // Configurar Entradas y Salidas 
	// Output
	SBI DDRB, DDB0	// DDRB = 1 -> Salida 
	CBI	PORTB,PORTB0	// PORTB = 0 -> Inicialmente apagado
	// Input
	CBI DDRD, DDD5	//PD5 =  Entrada
	CBI PORTD, PORTD5	// Deshabilitar pull up 



/****************************************/
// Loop Infinito
MAIN_LOOP:
	IN	R16, PIND	// Leyendo PIND y guardando en R16
	ANDI	R16, 0b00100000
	BRNE	MAIN_LOOP
	SBI		PINB, PINB0	// Toggle a PB0 
    RJMP    MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines

/****************************************/
// Interrupt routines

/****************************************/