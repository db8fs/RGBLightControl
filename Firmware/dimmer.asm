; ////////////////////////////////////////////////////////////////////////////
; //! @file 		dimmer.asm
; //! @description	Subroutines for increasing or decreasing the brightness
; //! 				of each LED module.
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////
	
; ----------------------------------------------------------------------------
; LED Module 1
; ----------------------------------------------------------------------------
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for increasing the intensity of the LED 1 color starting
	; //! with the actual intensity until the maximum target intensity is 
	; //! reached.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led1_fadein_intensity:
		push 	intensity
		in		intensity, SREG
		push 	intensity

		ldi		ZL, LOW(LED_INTENSITY)
		ldi		ZH, HIGH(LED_INTENSITY)
		
		ld		intensity, Z
		inc		intensity
		
	dimmer_led1_fadein_intensity_return:
		st		Z, intensity
		
		pop 	intensity
		out 	SREG, intensity
		pop		intensity
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for decreasing the intensity of the LED 1 color starting
	; //! with the actual intensity until the minimum intensity is reached.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led1_fadeout_intensity:
		push 	intensity
		in		intensity, SREG
		push 	intensity

		ldi		ZL, LOW(LED_INTENSITY)
		ldi		ZH, HIGH(LED_INTENSITY)

		ld		intensity, Z
		dec		intensity
		
	dimmer_led1_fadeout_intensity_return:
		st		Z, intensity
		
		pop		intensity
		out 	SREG, intensity
		pop		intensity
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for holding the intensity of LED 1 until the given time
	; //! intervall is elapsed.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led1_hold_intensity:
		ret	
	

	
; ----------------------------------------------------------------------------
; LED Module 2
; ----------------------------------------------------------------------------
		
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for increasing the intensity of the LED 2 color starting
	; //! with the actual intensity until the maximum target intensity is 
	; //! reached.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led2_fadein_intensity:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for decreasing the intensity of the LED 2 color starting
	; //! with the actual intensity until the minimum intensity is reached.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led2_fadeout_intensity:
		ret
	

	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for holding the intensity of LED 2 until the given time
	; //! intervall is elapsed.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led2_hold_intensity:
		ret	
	

; ----------------------------------------------------------------------------
; LED Module 3
; ----------------------------------------------------------------------------
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for increasing the intensity of the LED 3 color starting
	; //! with the actual intensity until the maximum target intensity is 
	; //! reached.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led3_fadein_intensity:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for decreasing the intensity of the LED 3 color starting
	; //! with the actual intensity until the minimum target intensity is 
	; //! reached.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led3_fadeout_intensity:
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for holding the intensity of LED 3 until the given time
	; //! intervall is elapsed.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led3_hold_intensity:
		ret	
	
	
; ----------------------------------------------------------------------------
; LED Module 4
; ----------------------------------------------------------------------------
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for increasing the intensity of the LED 4 color starting
	; //! with the actual intensity until the maximum target intensity is 
	; //! reached.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led4_fadein_intensity:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for decreasing the intensity of the LED 4 color starting
	; //! with the actual intensity until the minimum target intensity is 
	; //! reached.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led4_fadeout_intensity:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for holding the intensity of LED 4 until the given time
	; //! intervall is elapsed.
	; ////////////////////////////////////////////////////////////////////////
	
	dimmer_led4_hold_intensity:
		ret	
	
	
	
; ////////////////////////////////////////////////////////////////////////////
