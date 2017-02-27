; ////////////////////////////////////////////////////////////////////////////
; //! @file 		statusled.asm
; //! @description	Subroutines which control the mode of the onboard status
; //!				LED (LED0).
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////


; ----------------------------------------------------------------------------
; Subroutines
; ----------------------------------------------------------------------------

	status_led_initialization:
		push	akku
		in		akku, SREG
		push	akku
		
		push	ZL
		push	ZH
		
		ldi		ZL, LOW(STATUS_LED_RED)
		ldi		ZH, HIGH(STATUS_LED_RED)
		ldi		akku, 16
		st		Z, akku
		
		ldi		ZL, LOW(STATUS_LED_GREEN)
		ldi		ZH, HIGH(STATUS_LED_GREEN)
		ldi		akku, 0
		st		Z, akku
		
		ldi		ZL, LOW(STATUS_LED_BLUE)
		ldi		ZH, HIGH(STATUS_LED_BLUE)
		ldi		akku, 0
		st		Z, akku				
		
		ldi		akku, 0x00
		sts		STATUS_LED_CONFIG, akku

		pop		ZH
		pop		ZL

		pop		akku
		out		SREG, akku
		pop		akku
	ret


	status_led_follow_led1:
		push	akku
		in		akku, SREG
		push	akku

		push	ZL
		push	ZH
		
		ldi		ZL, LOW(LED_RED)
		ldi		ZH, HIGH(LED_RED)
		ld		akku, Z

		ldi 	ZL, LOW(STATUS_LED_RED)
		ldi		ZH, HIGH(STATUS_LED_RED)
		st		Z, akku
		
		ldi		ZL, LOW(LED_GREEN)
		ldi		ZH, HIGH(LED_GREEN)
		ld		akku, Z

		ldi 	ZL, LOW(STATUS_LED_GREEN)
		ldi		ZH, HIGH(STATUS_LED_GREEN)
		st		Z, akku
		
		ldi		ZL, LOW(LED_BLUE)
		ldi		ZH, HIGH(LED_BLUE)
		ld		akku, Z

		ldi 	ZL, LOW(STATUS_LED_BLUE)
		ldi		ZH, HIGH(STATUS_LED_BLUE)
		st		Z, akku				
		pop		ZH
		pop		ZL
		
		pop		akku
		out		SREG, akku
		pop		akku
	ret
