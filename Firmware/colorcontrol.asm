//////////////////////////////////////////////////////////////////////////////
; //! @file 		colorcontrol.asm
; //! @description	Subroutines for controlling the color of the LEDs
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
//////////////////////////////////////////////////////////////////////////////
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine does some funny initialization stuff for used SRAM
	; //! addresses.
	; ////////////////////////////////////////////////////////////////////////
	
	colorcontrol_initialize:
		ldi 	akku,	LED_CONFIGURATION_CYCLE_HUE_HARD
		sts		LED_CONFIGURATION, 		akku
		sts		LED_CONFIGURATION+1, 	akku
		sts		LED_CONFIGURATION+2, 	akku
		sts		LED_CONFIGURATION+3, 	akku
		ldi		akku, 0x00
		sts		LED_RED,				akku
		sts		LED_RED+1,				akku
		sts		LED_RED+2,				akku
		sts		LED_RED+3,				akku
		sts		LED_GREEN,				akku
		sts		LED_GREEN+1,			akku
		sts		LED_GREEN+2,			akku
		sts		LED_GREEN+3,			akku
		sts		LED_BLUE,				akku
		sts		LED_BLUE+1,				akku
		sts		LED_BLUE+2,				akku
		sts		LED_BLUE+3,				akku
		sts		LED_HUE,				akku
		sts		LED_HUE+1,				akku
		sts		LED_HUE+2,				akku
		sts		LED_HUE+3,				akku
		ldi		akku, 127
		sts		LED_INTENSITY,			akku
		sts		LED_INTENSITY+1,		akku
		sts		LED_INTENSITY+2,		akku
		sts		LED_INTENSITY+3,		akku
		ldi		akku, LED_COLOR_MODE_HSI
		sts		LED_COLOR_MODE,			akku
		sts		LED_COLOR_MODE+1,		akku
		sts		LED_COLOR_MODE+2,		akku
		sts		LED_COLOR_MODE+3,		akku
		ret
	

	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine checks which LED module runs in HSI mode and 
	; //! executes the HSI2RGB conversion for this module.
	; ////////////////////////////////////////////////////////////////////////
		
	colorcontrol_apply_led_color_modes:
		ldi		counter, NUMBER_OF_LEDS
		
	colorcontrol_apply_led_color_modes_loop:
		ldi		akku, NUMBER_OF_LEDS
		sub		akku, counter
		mov		param, akku
		clr		akku
		
		ldi		ZL, LOW(LED_COLOR_MODE)
		ldi		ZH, HIGH(LED_COLOR_MODE)
		add		ZL, param
		adc		ZH, akku
		ld		akku, Z
		
		cpi		akku, LED_COLOR_MODE_HSI
		brne	colorcontrol_apply_led_color_modes_next
		rcall	colorspaces_hsi2rgb
		
	colorcontrol_apply_led_color_modes_next:
		dec		counter
		tst		counter
		brne	colorcontrol_apply_led_color_modes_loop
		
		ret
	

	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine switches through the internal display modes, e.g.
	; //! when a button is pressed.
	; ////////////////////////////////////////////////////////////////////////
	
	colorcontrol_next_function:
		lds 	akku, LED_CONFIGURATION
		cpi		akku, LED_CONFIGURATION_CYCLE_HUE_HARD
		breq	colorcontrol_next_function_cycle_hue_smooth
		
		cpi		akku, LED_CONFIGURATION_CYCLE_HUE_SMOOTH
		breq	colorcontrol_next_function_cycle_intensity_smooth
		
		cpi		akku, LED_CONFIGURATION_CYCLE_INTENSITY_SMOOTH	
		breq 	colorcontrol_next_function_constant
		
		cpi		akku, LED_CONFIGURATION_CONSTANT_COLOR
		breq	colorcontrol_next_function_cycle_hard_color
		
	colorcontrol_next_function_cycle_intensity_smooth:
		ldi 	akku, (1 << CS10)
		out		TCCR1B,	akku
		ldi		akku, LED_CONFIGURATION_CYCLE_INTENSITY_SMOOTH	
		rjmp	colorcontrol_next_function_finished
		
	colorcontrol_next_function_constant:
		ldi 	akku, (1 << CS11) | (1 << CS10)
		out		TCCR1B,	akku
		ldi		akku, LED_CONFIGURATION_CONSTANT_COLOR
		rjmp 	colorcontrol_next_function_finished
		
	colorcontrol_next_function_cycle_hard_color:
		ldi 	akku, (1 << CS12)
		out		TCCR1B,	akku
		ldi		akku, LED_CONFIGURATION_CYCLE_HUE_HARD
		rjmp 	colorcontrol_next_function_finished
		
	colorcontrol_next_function_cycle_hue_smooth:
		ldi 	akku, (1 << CS11) | (1 << CS10)
		out		TCCR1B,	akku
		ldi 	akku,	LED_CONFIGURATION_CYCLE_HUE_SMOOTH
		rjmp 	colorcontrol_next_function_finished
		
	colorcontrol_next_function_finished:
		sts		LED_CONFIGURATION, 		akku
		sts		LED_CONFIGURATION+1, 	akku
		sts		LED_CONFIGURATION+2, 	akku
		sts		LED_CONFIGURATION+3, 	akku
		ret
	
	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine cycles through the colors using larger intervalls.
	; ////////////////////////////////////////////////////////////////////////
	
	colorcontrol_hard_color_cycle:
		push 	akku
		in		akku, SREG
		push 	akku
		push	temp
		
		clr		akku
		
		ldi		ZL, LOW(LED_HUE)
		ldi		ZH, HIGH(LED_HUE)
		add		ZL, param
		adc		ZH, akku
		
		ld		temp, Z
		ldi		akku, LED_HARD_CYCLE_INTERVALL
		add		akku, temp
		st		Z, akku
		
		pop		temp
		pop 	akku
		out 	SREG, akku
		pop		akku
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine increments the hue for led 'param' modulo 180.
	; ////////////////////////////////////////////////////////////////////////
	
	colorcontrol_increment_hue:
		push 	akku
		in		akku, SREG
		push 	akku
		
		clr		akku
		
		ldi		ZL, LOW(LED_HUE)
		ldi		ZH, HIGH(LED_HUE)
		add		ZL, param
		adc		ZH, akku
		
		ld		akku, Z
		inc		akku
		st		Z, akku
		
		pop 	akku
		out 	SREG, akku
		pop		akku
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine decrements the hue for led 'param' modulo 180.
	; ////////////////////////////////////////////////////////////////////////
	
	colorcontrol_decrement_hue:
		push 	akku
		in		akku, SREG
		push 	akku

		clr		akku

		ldi		ZL, LOW(LED_HUE)
		ldi		ZH, HIGH(LED_HUE)
		add		ZL, param
		adc 	ZH, akku		
		
		ld		akku, Z
		dec		akku
		cpi		akku, 180
		brlo	colorcontrol_decrement_hue_return
		ldi		akku, 179
		
	colorcontrol_decrement_hue_return:
		st		Z, akku
		
		pop 	akku
		out 	SREG, akku
		pop		akku
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine increments the intensity for led 'param'.
	; ////////////////////////////////////////////////////////////////////////
	
	colorcontrol_increment_intensity:
		push 	akku
		in		akku, SREG
		push 	akku
		
		clr		akku
		
		ldi		ZL, LOW(LED_INTENSITY)
		ldi		ZH, HIGH(LED_INTENSITY)
		add		ZL, param
		adc		ZH, akku
		
		ld		akku, Z
		inc		akku
		st		Z, akku
		
		pop 	akku
		out 	SREG, akku
		pop		akku
		ret


