; ////////////////////////////////////////////////////////////////////////////
; //! @file 		main.asm
; //! @description	Definition of the program entry points and central loop
; //!				routine of the µC.
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////

.INCLUDE "m16def.inc"
.INCLUDE "definition.inc"


; ////////////////////////////////////////////////////////////////////////////
; // Comment this to check for invalid command messages. If uncommented we 
; // will take everything from the UART (that starts with the magic byte) 
; // as a valid command.
; ////////////////////////////////////////////////////////////////////////////

#define _NO_CHECKSUM_CALCULATION_


; ////////////////////////////////////////////////////////////////////////////
; // EEPROM configuration data definition
; ////////////////////////////////////////////////////////////////////////////

.ESEG
	EEPROM_PUSHBUTTON_CONFIGURATION:			.DB		PUSHBUTTON_ACTION_ON_BOTH_EVENTS,\
														PUSHBUTTON_ACTION_ON_BOTH_EVENTS,\
														PUSHBUTTON_ACTION_ON_BOTH_EVENTS
	
	EEPROM_PUSHBUTTON_PRESSED_CALLBACK_TABLE:	.DW 	0x0001,\
														0x0002,\
														0x0003
	
	EEPROM_PUSHBUTTON_RELEASED_CALLBACK_TABLE:	.DW		0x0001,\
														0x0002,\
														0x0003
	
	
; ////////////////////////////////////////////////////////////////////////////
; // SRAM configuration data definition
; ////////////////////////////////////////////////////////////////////////////

.DSEG
	PWM_Counter:						.BYTE	1
	
	PWM_RED:							.BYTE	NUMBER_OF_LEDS
	PWM_GREEN:							.BYTE	NUMBER_OF_LEDS
	PWM_BLUE:							.BYTE	NUMBER_OF_LEDS
	
	PWM_STATUS_LED_RED:					.BYTE	1
	PWM_STATUS_LED_GREEN:				.BYTE	1
	PWM_STATUS_LED_BLUE:				.BYTE	1
	
	STATUS_LED_RED:						.BYTE	1
	STATUS_LED_GREEN:					.BYTE	1
	STATUS_LED_BLUE:					.BYTE	1
	
	STATUS_LED_CONFIG:					.BYTE	1	
	LED_CONFIGURATION:					.BYTE	NUMBER_OF_LEDS
	
	; mode settings for all led groups
	LED_COLOR_MODE:						.BYTE	NUMBER_OF_LEDS

	; values for hsi mode
	LED_INTENSITY:						.BYTE	NUMBER_OF_LEDS	
	LED_HUE:							.BYTE	NUMBER_OF_LEDS
	
	; values for rgb mode
	LED_RED:							.BYTE	NUMBER_OF_LEDS
	LED_GREEN:							.BYTE	NUMBER_OF_LEDS
	LED_BLUE:							.BYTE	NUMBER_OF_LEDS
	
	; dimming boundaries
	LED_RED_FADE_MAX:					.BYTE	NUMBER_OF_LEDS
	LED_RED_FADE_MIN:					.BYTE	NUMBER_OF_LEDS
	LED_GREEN_FADE_MAX:					.BYTE	NUMBER_OF_LEDS
	LED_GREEN_FADE_MIN:					.BYTE	NUMBER_OF_LEDS
	LED_BLUE_FADE_MAX:					.BYTE	NUMBER_OF_LEDS
	LED_BLUE_FADE_MIN:					.BYTE	NUMBER_OF_LEDS
	LED_HUE_FADE_MAX:					.BYTE	NUMBER_OF_LEDS
	LED_HUE_FADE_MIN:					.BYTE	NUMBER_OF_LEDS
	LED_INTENSITY_FADE_MAX:				.BYTE	NUMBER_OF_LEDS
	LED_INTENSITY_FADE_MIN:				.BYTE	NUMBER_OF_LEDS
	
	; pushbutton stuff
	PUSHBUTTON_CONFIGURATION:			.BYTE	NUMBER_OF_PUSHBUTTONS
	PUSHBUTTON_PRESSED_CALLBACK_TABLE:	.BYTE	NUMBER_OF_PUSHBUTTONS*2 ; stores 16-bit addresses for each pushbutton
	PUSHBUTTON_RELEASED_CALLBACK_TABLE:	.BYTE	NUMBER_OF_PUSHBUTTONS*2
	
	UART_STATE:							.BYTE	1	
	UART_MESSAGE_BUFFER:				.BYTE 	UART_MESSAGE_LENGTH
	
; ////////////////////////////////////////////////////////////////////////////
; // Interrupt vector definition
; ////////////////////////////////////////////////////////////////////////////

.CSEG

.ORG 0x00	
		rjmp 	init

.ORG OVF1addr	
		rjmp clock_isr
		
.ORG OVF0addr
      	rjmp pwm_isr

.ORG URXCaddr
		rjmp uart_rx_isr


; ////////////////////////////////////////////////////////////////////////////
; // Lookup table definition
; ////////////////////////////////////////////////////////////////////////////

	HSI2RGB_LOOKUPTABLE:	.DB		0, 4, 8, 12, 16, 21, 25, 29, 33, 38, 42,\
									46, 50, 55, 59, 63, 67, 71, 76, 80, 84, \
									88, 93, 97, 101, 105, 110, 114, 118, 122
	
; ////////////////////////////////////////////////////////////////////////////
; // Subroutine inclusion
; ////////////////////////////////////////////////////////////////////////////

.INCLUDE "strings.asm"
.INCLUDE "colorspaces.asm"
.INCLUDE "colorcontrol.asm"
.INCLUDE "dimmer.asm"
.INCLUDE "pwm.asm"
.INCLUDE "eeprom.asm"
.INCLUDE "ioport.asm"
.INCLUDE "uart.asm"
.INCLUDE "statusled.asm"

; ////////////////////////////////////////////////////////////////////////////
; //! Interrupt service routine definitions
; ////////////////////////////////////////////////////////////////////////////

	uart_rx_isr:
		push	akku
		in		akku, SREG
		push	akku
		push	counter
		push	param
		
		push	ZL
		push	ZH
		push	YL
		push	YH
		
		rcall 	uart_check_receiver
		
		pop		YH
		pop		YL
		pop 	ZH
		pop		ZL

		pop		param
		pop		counter
		pop		akku
		out		SREG, akku
		pop		akku
		reti	
		
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Timer for determining larger time quantities (1...x seconds)
	; ////////////////////////////////////////////////////////////////////////
	
	clock_isr:
	/*	push 	akku
		in		akku, SREG
		push	akku
		push	counter*/
/*
		ldi		counter, NUMBER_OF_LEDS
		
	clock_isr_evaluate_led_configuration:
		ldi		akku, NUMBER_OF_LEDS
		sub		akku, counter
		mov		param, akku
		clr		akku
		
		ldi		ZL, LOW(LED_CONFIGURATION)
		ldi		ZH, HIGH(LED_CONFIGURATION)
		add		ZL, param
		adc		ZH, akku
		ld		akku, Z
	
	clock_isr_evaluate_led_configuration_first_config:
		cpi		akku, LED_CONFIGURATION_CYCLE_HUE_SMOOTH
		brne	clock_isr_evaluate_led_configuration_second_config
		rcall	colorcontrol_increment_hue		
		rcall	colorspaces_hsi2rgb
		rjmp	clock_isr_evaluate_led_configuration_finished
	
	clock_isr_evaluate_led_configuration_second_config:
		cpi		akku, LED_CONFIGURATION_CYCLE_HUE_HARD
		brne	clock_isr_evaluate_led_configuration_third_config
		rcall	colorcontrol_hard_color_cycle
		rcall	colorspaces_hsi2rgb
		rjmp	clock_isr_evaluate_led_configuration_finished
	
	clock_isr_evaluate_led_configuration_third_config:
		cpi		akku, LED_CONFIGURATION_CYCLE_INTENSITY_SMOOTH
		brne	clock_isr_evaluate_led_configuration_finished
		rcall	colorcontrol_increment_intensity
		rcall	colorspaces_hsi2rgb
		rjmp	clock_isr_evaluate_led_configuration_finished	
		
	clock_isr_evaluate_led_configuration_finished:
		dec		counter
		tst		counter
		brne	clock_isr_evaluate_led_configuration
	*/
	
	/*
		ldi		akku, 0x01
		add 	clock_seconds_counter_low,	akku
		ldi		akku, 0x00
		adc 	clock_seconds_counter_high,	akku
		
		cpi		clock_seconds_counter_high, HIGH(1)
		brlt 	clock_isr_return
		
		cpi		clock_seconds_counter_low, LOW(1)
		brlt	clock_isr_return
		
		inc		clock_seconds_register
	*/
		//rcall	cycle_color
		
/*	clock_isr_return:
		//rcall	dimmer_led1_fadeout_intensity
		
	*/	
	
	/*	pop		ZH
		pop		ZL
		pop		counter
		pop		akku
		out		SREG, akku
		pop		akku*/
		reti
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Software PWM for regulating the brightness of each color channel
	; //! with 7-bit duty cycle.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_isr:
		push 	akku
		in		akku, SREG
		push	akku
		
		lds		akku, PWM_Counter
		inc		akku
		cpi		akku, 128
		brne	pwm_isr_work
		clr		akku
		
	pwm_isr_work:
		sts		PWM_Counter, akku
		
		pop		akku
		out		SREG, akku
		pop		akku
		
		reti
		
		
; ////////////////////////////////////////////////////////////////////////////
; //! Basic initialization and configuration of integrated microcontroller 
; //! components.
; //////////////////////////////////////////////////////////////////////////// 
		
init:
		ldi		akku, 	HIGH(RAMEND)
		out		SPH,	akku
		ldi		akku,	LOW(RAMEND)
		out		SPL, 	akku
		
		rcall	colorcontrol_initialize
		rcall 	ioport_initialization
		rcall	uart_init		
		rcall	status_led_initialization
		rcall 	eeprom_load_configuration
		
		rcall	uart_transmit_greeting
		rcall	uart_transmit_newline
		
		ldi		akku, 0
		sts		PWM_Counter, akku
		
		; -- 
		
		ldi 	akku,	(1 << CS00)
		out 	TCCR0,	akku
		
		ldi 	akku, (1 << CS12)
		out		TCCR1B,	akku
		
		ldi		akku,	(1 << TOIE0) | (1 << TOIE1)
		out		TIMSK,	akku
		
		sei
		
		rjmp 	main_loop
		
			
; ////////////////////////////////////////////////////////////////////////////
; //! Main loop
; ////////////////////////////////////////////////////////////////////////////

main_loop:
		
		// check if the user has done some input
		rcall	ioport_check_pushbuttons	

		// apply the HSI2RGB conversion for the configured LED modules
		cbi		UCSRB, RXCIE
		rcall	colorcontrol_apply_led_color_modes
		sbi		UCSRB, RXCIE
		
		// apply the RGB values on the PWM channels
		rcall 	pwm_led1_set_rgb_tripel
		rcall	pwm_led2_set_rgb_tripel
		rcall	pwm_led3_set_rgb_tripel
		rcall	pwm_led4_set_rgb_tripel
		
		rcall	status_led_follow_led1
		rcall 	pwm_status_led_set_rgb_tripel
		
		lds		akku, PWM_Counter
		
		rcall	pwm_led0_red
		rcall	pwm_led0_green
		rcall	pwm_led0_blue		
		
		rcall	pwm_led1_red
		rcall	pwm_led1_green
		rcall	pwm_led1_blue
		
		rcall 	pwm_led2_red
		rcall	pwm_led2_green
		rcall	pwm_led2_blue
		
		rcall	pwm_led3_red
		rcall	pwm_led3_green
		rcall	pwm_led3_blue
		
		rcall	pwm_led4_red
		rcall	pwm_led4_green
		rcall	pwm_led4_blue
				
		rjmp 	main_loop
		
		
; ////////////////////////////////////////////////////////////////////////////
