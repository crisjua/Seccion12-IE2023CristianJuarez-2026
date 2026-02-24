/*
* Interrupciones.asm
*
* Creado: 23/02/2026
* Autor : Cristian Juárez
* Descripción: 
*/;Contador de decenas, cada vez que el contador TMR0 llegue a 10 se debe de resetear e incremetar el contador de decenas
;Y en un segundo display se debe de mostrar esto de manera que se muestren las decenas de segundo.
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.INCLUDE "M328PDEF.INC"

.CSEG
.ORG 0x00
    JMP SETUP

.ORG OVF0addr
    JMP ISR_TIMER0

//Tabla de 7 segmentos
SEG:
.DB 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07
.DB 0x7F,0x6F

// Registros que se utilizaran
; R20 ? contador 10 ms
; R21 ? unidades
; R23 ? decenas
; R22 ? segmentos unidades
; R24 ? segmentos decenas

;================= SETUP =======================
SETUP:
    ; Stack
    LDI R16, LOW(RAMEND)
    OUT SPL, R16
    LDI R16, HIGH(RAMEND)
    OUT SPH, R16

    ; Deshabilitar USART
    LDI R16, 0
    STS UCSR0B, R16

    ; PORTD 7 segmentos
    LDI R16, 0xFF
    OUT DDRD, R16
    CLR R16
    OUT PORTD, R16

    ; PB2 = unidades, PB3 = decenas
    SBI DDRB, PB2
    SBI DDRB, PB3
    CBI PORTB, PB2
    CBI PORTB, PB3

    ; Inicializar contadores
    CLR R20
    CLR R21
    CLR R23

    CALL INIT_T0
    SEI

;================= LOOP ========================
MAIN_LOOP:
    // Inician apagados los display
    CBI PORTB, PB2
    CBI PORTB, PB3

    // Unidades
    CALL DISPLAY_UNI
    OUT PORTD, R22
    SBI PORTB, PB2
    CALL DELAY_MUX
    CBI PORTB, PB2

    // Decenas
    CALL DISPLAY_DEC
    OUT PORTD, R24
    SBI PORTB, PB3
    CALL DELAY_MUX
    CBI PORTB, PB3

    RJMP MAIN_LOOP

// Display
DISPLAY_UNI:
    LDI ZH, HIGH(SEG<<1)
    LDI ZL, LOW(SEG<<1)
    ADD ZL, R21
    LPM R22, Z
    RET

DISPLAY_DEC:
    LDI ZH, HIGH(SEG<<1)
    LDI ZL, LOW(SEG<<1)
    ADD ZL, R23
    LPM R24, Z
    RET

// Timer 0
INIT_T0:
    ; Prescaler 1024
    LDI R16, (1<<CS02)|(1<<CS00)
    OUT TCCR0B, R16

    ; Overflow cada ~10 ms
    LDI R16, 99
    OUT TCNT0, R16

    ; Habilitar interrupción
    LDI R16, (1<<TOIE0)
    STS TIMSK0, R16
    RET

// ISR
ISR_TIMER0:
    LDI R16, 99
    OUT TCNT0, R16
    SBI TIFR0, TOV0

    INC R20
    CPI R20, 100
    BRNE FIN_ISR

    CLR R20
    INC R21			//Incrementa las Unidades
    CPI R21, 10
    BRNE FIN_ISR

    CLR R21
    INC R23			//Incrementa Decenas
    CPI R23, 6		
    BRNE FIN_ISR

    CLR R23			// Si llega a 60 segundos resetea el contador

FIN_ISR:
    RETI

// Delay del MUX
DELAY_MUX:
    LDI R18, 50
D1:
    LDI R19, 200
D2:
    DEC R19
    BRNE D2
    DEC R18
    BRNE D1
    RET