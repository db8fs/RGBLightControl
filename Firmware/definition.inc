; ////////////////////////////////////////////////////////////////////////////
; //! @file			definition.inc
; //! @description	This file defines constants, register names, etc.pp.
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////

; ----------------------------------------------------------------------------
; Clock settings
; ----------------------------------------------------------------------------

.EQU	F_CPU	= 12000000  	//3686400
.EQU	BAUD	= 57600

; ----------------------------------------------------------------------------
; Register renaming
; ----------------------------------------------------------------------------
	
.DEF	clock_seconds_register 		= r2
.DEF	hsi2rgb_p					= r3
.DEF	hsi2rgb_q					= r4
.DEF	hsi2rgb_t					= r5
.DEF	pushbutton_debounce			= r8
.DEF	pushbutton_debounce_temp	= r9

.DEF	temp						= r14
.DEF	param						= r15
.DEF	akku 						= r16
.DEF	counter						= r17
.DEF 	hue 						= r18
.DEF	intensity 					= r19
.DEF	red							= r20
.DEF	green 						= r21
.DEF 	blue 						= r22
.DEF	clock_seconds_counter_low	= r23
.DEF	clock_seconds_counter_high	= r24
.DEF	led_counter					= r25



; ----------------------------------------------------------------------------
; Constant definitions for readability
; ----------------------------------------------------------------------------

.EQU	NUMBER_OF_LEDS			= 4 // number of rgb leds (LED0 is not counted)
.EQU 	NUMBER_OF_PUSHBUTTONS 	= 3

.EQU 	STATUS_LED_ENABLED		= 1
.EQU 	LED_HARD_CYCLE_INTERVALL = 30

.EQU	LED_CONFIGURATION_CYCLE_HUE_HARD			= 0x00
.EQU 	LED_CONFIGURATION_CYCLE_HUE_SMOOTH			= 0x01
.EQU	LED_CONFIGURATION_CYCLE_INTENSITY_SMOOTH	= 0x02
.EQU	LED_CONFIGURATION_CONSTANT_COLOR			= 0x03

.EQU	LED_COLOR_MODE_RGB 							= 0x00
.EQU 	LED_COLOR_MODE_HSI							= 0xFF

.EQU	PUSHBUTTON_NO_ACTION						= 0x00
.EQU	PUSHBUTTON_ACTION_ON_PRESSED_EVENT		 	= 0x01
.EQU	PUSHBUTTON_ACTION_ON_RELEASED_EVENT			= 0x02
.EQU	PUSHBUTTON_ACTION_ON_BOTH_EVENTS			= 0x03


; ----------------------------------------------------------------------------
; UART definitions
; ----------------------------------------------------------------------------

.EQU 	UART_STATE_READY			= 0x00
.EQU	UART_STATE_MAGIC_BYTE		= 0x01
.EQU	UART_STATE_LED_SELECT		= 0x02
.EQU	UART_STATE_COMMAND			= 0x03
.EQU	UART_STATE_LED1_VALUE1		= 0x04
.EQU	UART_STATE_LED1_VALUE2		= 0x05
.EQU	UART_STATE_LED1_VALUE3		= 0x06
.EQU	UART_STATE_LED2_VALUE1		= 0x07
.EQU	UART_STATE_LED2_VALUE2		= 0x08
.EQU	UART_STATE_LED2_VALUE3		= 0x09
.EQU	UART_STATE_LED3_VALUE1		= 0x0A
.EQU	UART_STATE_LED3_VALUE2		= 0x0B
.EQU	UART_STATE_LED3_VALUE3		= 0x0C
.EQU	UART_STATE_LED4_VALUE1		= 0x0D
.EQU	UART_STATE_LED4_VALUE2		= 0x0E
.EQU	UART_STATE_LED4_VALUE3		= 0x0F
.EQU	UART_STATE_CHECKSUM			= 0x10

.EQU	UART_MESSAGE_LENGTH			= 16

.EQU	UART_MESSAGE_MAGIC_BYTE		= 0xFA


; ----------------------------------------------------------------------------
; I/O port configuration
; ----------------------------------------------------------------------------


; // i/o port definition RGB-LED 0 (status LED)

.EQU	LED0_RED_PORT		= PORTD
.EQU	LED0_GREEN_PORT		= PORTD
.EQU	LED0_BLUE_PORT		= PORTD

.EQU	LED0_RED_DDR		= DDRD
.EQU	LED0_GREEN_DDR		= DDRD
.EQU	LED0_BLUE_DDR		= DDRD

.EQU	LED0_RED_PIN		= PD5
.EQU	LED0_GREEN_PIN		= PD6
.EQU	LED0_BLUE_PIN		= PD7


; // i/o port definition RGB-LED 1 

.EQU	LED1_RED_PORT 		= PORTC
.EQU	LED1_GREEN_PORT		= PORTC
.EQU	LED1_BLUE_PORT 		= PORTC

.EQU	LED1_RED_DDR		= DDRC
.EQU	LED1_GREEN_DDR		= DDRC
.EQU	LED1_BLUE_DDR 		= DDRC


.EQU	LED1_RED_PIN 		= PC0
.EQU	LED1_GREEN_PIN 		= PC2
.EQU	LED1_BLUE_PIN 		= PC1


; // i/o port definition RGB-LED 2

.EQU	LED2_RED_PORT 		= PORTC
.EQU	LED2_GREEN_PORT		= PORTC
.EQU	LED2_BLUE_PORT 		= PORTC

.EQU	LED2_RED_DDR		= DDRC
.EQU	LED2_GREEN_DDR		= DDRC
.EQU	LED2_BLUE_DDR 		= DDRC

.EQU	LED2_RED_PIN 		= PC3
.EQU	LED2_GREEN_PIN 		= PC4
.EQU	LED2_BLUE_PIN 		= PC5


; // i/o port definition RGB-LED 3

.EQU	LED3_RED_PORT 		= PORTC
.EQU	LED3_GREEN_PORT		= PORTC
.EQU	LED3_BLUE_PORT 		= PORTB

.EQU	LED3_RED_DDR		= DDRC
.EQU	LED3_GREEN_DDR		= DDRC
.EQU	LED3_BLUE_DDR 		= DDRB

.EQU	LED3_RED_PIN 		= PC6
.EQU	LED3_GREEN_PIN 		= PC7
.EQU	LED3_BLUE_PIN 		= PB0

; // i/o port definition RGB-LED 4

.EQU	LED4_RED_PORT 		= PORTB
.EQU	LED4_GREEN_PORT		= PORTB
.EQU	LED4_BLUE_PORT 		= PORTB

.EQU	LED4_RED_DDR		= DDRB
.EQU	LED4_GREEN_DDR		= DDRB
.EQU	LED4_BLUE_DDR 		= DDRB

.EQU	LED4_RED_PIN 		= PB1
.EQU	LED4_GREEN_PIN 		= PB2
.EQU	LED4_BLUE_PIN 		= PB3


; // i/o port definition pushbutton

.EQU	PUSHBUTTON1_PORT	= PINA
.EQU	PUSHBUTTON1_DDR		= DDRA
.EQU	PUSHBUTTON1_PIN		= PA2


.EQU	PUSHBUTTON2_PORT	= PINA
.EQU	PUSHBUTTON2_DDR		= DDRA
.EQU	PUSHBUTTON2_PIN		= PA1


.EQU	PUSHBUTTON3_PORT	= PINA
.EQU	PUSHBUTTON3_DDR		= DDRA
.EQU	PUSHBUTTON3_PIN		= PA0
