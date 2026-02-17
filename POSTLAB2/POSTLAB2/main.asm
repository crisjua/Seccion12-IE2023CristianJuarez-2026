/*
* POSTLAB2.asm
*
* Creado: 12/02/2026
* Autor : Cristian Juárez
* Descripción: 
*/; Contador automatico de los 4 bits que al estar igual al de el 7 segmentos se debe reiniciar el contador
// de segundos y cambiar el estado de una LED de 0 a 1 durante un ciclo entero de manera que pueda variar
//el perido de encendido y apagado del indicador utilizando los botones.
/****************************************/
.INCLUDE "M328PDEF.INC"
.CSEG
.ORG 0X00

;STACK POINTER SPL SPH
LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R17, HIGH(RAMEND)
OUT SPH, R17


T7S: .DB 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71


SETUP:
	
	LDI		R22, 15		;Tope Maximo
	LDI		R24, 0		;Tope minimo


	//Desabilitar puerto RX y TX para uso de I/O
	LDI		R16, 0x00	
	STS		UCSR0B, R16

	LDI		R16, 0b11110000 // Los utimos puertos de C estan puestos como salida y los primeros en entrada
	OUT		DDRC, R16
	LDI		R16, 0b00001111  // Los ultimos empiezan apagados y los primeros tienen pull up
	OUT		PORTC, R16

	//Configuración Salidas D7S

	LDI		R16, 0b0111_1111	// Configura el puerto D como salida
	OUT		DDRD, R16			//output display

	LDI		R18, 0				//T7S
	LDI		R21, 0				// Valor Inicial
	LDI		ZH, HIGH(T7S << 1)
	LDI		ZL, LOW(T7S << 1)
	ADD		ZL, R18
	LPM		R18, Z				// Cargamos la memoria a Z

	LDI		R16, 0x3F			// 7segmentos en 0
	OUT		DDRB, R16			// Mostramos
	LDI		R16, 0x00
	OUT		PORTB, R16		//Iniciamos en 0


	CALL TIMER

	LDI		R20, 0
	LDI		R30, 0

MAIN_LOOP:

	OUT		PORTD, R18


	SBIS	PINC, PC0		// Salta a la siguiente instrucción si el boton PC0 esta presionado 
	CALL	DECREMENTAR
	SBIS	PINC, PC1		// Salta a la siguiente instrucción si el boton PC1 esta presionado 
	CALL	INCREMENTAR

	IN		R26, TIFR0		//Lee el registro de bander de interrupción
	CPI		R26, (1<<TOV0) // Compara bandera de overflow si esta activa
	BRNE	MAIN_LOOP  // espera el overflow

	CPI		R20, 16		// si R20 es 16,  0- 15
	BREQ	ZERO		//Realizar salto a zero

	LDI		R26, 100	// El valor de overflow
	OUT		TCNT0, R26  // iniciar en un valor especifico

	SBI		TIFR0, TOV0 // borra la bandera de desbordamiento

	INC		R30
	CPI		R30, 100
	BRNE	MAIN_LOOP   // Si la comparación es igual regresa a Main_Loop
	CLR		R30

	INC		R20
	MOV		R25, R20	// Copiamos el registro R20 a R25 
	OUT		PORTB, R25	// Mostramos el Resultado en PORTB

	//VER ALARMA
	CP		R21, R20	// Compara lo de la salida y si son iguales pasa a siguiente instrucción
	BRNE	MAIN_LOOP
	SBI		PINC, PC4   //toggle

	MOV		R23, R20	// Copiamos registro R20 a R23
	CP		R21, R23	
	BRNE	MAIN_LOOP
	LDI		R20, 0

	RJMP	MAIN_LOOP



DELAYBOUNCE:
	IN		R26, TIFR0
	CPI		R26, (1<< TOV0)		//si la bandera no esta seteada reiniciarla
	BRNE	DELAYBOUNCE
	LDI		R26, 100			// Cargamos el valor de overflow de nuevo
	OUT		TCNT0, R26
	SBI		TIFR0, TOV0
	INC		R30
	CPI		R30, 100
	BRNE	DELAYBOUNCE
	CLR		R30

	SBIS	PINC, (PC1 & PC0)	// Si el registro es 1 se adelanta a la siguiente instrucción
	RJMP	DELAYBOUNCE
	RJMP	MAIN_LOOP



INCREMENTAR:
	CPSE	R21, R22		// Compara R21 y R22, salta a la siguiente instrucción si R21 = R22
	INC		R21				//Incrementa D7S
	MOV		R18, R21
	LDI		ZH, HIGH(T7S << 1)
	LDI		ZL, LOW(T7S << 1)
	ADD		ZL, R18
	LPM		R18, Z
	OUT		PORTD, R18
	RJMP	DELAYBOUNCE


DECREMENTAR:
	CPSE	R21, R24		// Compara R21 y R24, salta a la siguiente instrucción si R21 = R24
	DEC		R21				//Decrementa D7S
	MOV		R18, R21
	LDI		ZH, HIGH(T7S << 1)
	LDI		ZL, LOW(T7S << 1)
	ADD		ZL, R18
	LPM		R18, Z
	OUT		PORTD, R18
	RJMP	DELAYBOUNCE


ZERO:
	LDI		R20, 0			//Carga 0 a R20

TIMER:
	LDI		R16, (1<<CS02) | (1<<CS00)		// Prescalador de 1024
	OUT		TCCR0B, R16

	LDI		R16, 100		//Valor de Overflow
	OUT		TCNT0, R16		//Cargo valor Inicial
	RET