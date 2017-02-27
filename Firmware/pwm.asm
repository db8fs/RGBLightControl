; ////////////////////////////////////////////////////////////////////////////
; //! @file 		pwm.asm
; //! @description	Subroutines that realize Software-PWM for the color 
; //!				channels of all LED modules.
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////




; ----------------------------------------------------------------------------
; Status LED
; ----------------------------------------------------------------------------
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for setting the software-pwm duty cycle of LED0 to the
	; //! appropriate RGB-tripel
	; ////////////////////////////////////////////////////////////////////////	

	pwm_status_led_set_rgb_tripel:
		lds		akku, STATUS_LED_RED
		sts		PWM_STATUS_LED_RED, akku
		
		lds		akku, STATUS_LED_GREEN
		sts		PWM_STATUS_LED_GREEN, akku
		
		lds		akku, STATUS_LED_BLUE
		sts		PWM_STATUS_LED_BLUE, akku
		
		ret
		
		
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'red' of LED0
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led0_red:
		lds		r17, STATUS_LED_CONFIG
		andi	r17, (1 << STATUS_LED_ENABLED)
		tst		r17
		breq	pwm_led0_red_on
		
		lds		r17, PWM_STATUS_LED_RED
		
		; check if signed 7-bit duty cycle value is negative - if so, use 
		; one-complement of this value
		
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led0_red_work
		com		r17
		
		pwm_led0_red_work:
			cp		akku, r17
			brlo	pwm_led0_red_off
			rjmp	pwm_led0_red_on
		
		pwm_led0_red_on:
			sbi		LED0_RED_PORT, LED0_RED_PIN
			rjmp	pwm_led0_red_return
		
		pwm_led0_red_off:
			cbi		LED0_RED_PORT, LED0_RED_PIN
			rjmp 	pwm_led0_red_return
		
	pwm_led0_red_return:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'green' of LED0
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led0_green:
		lds		r17, STATUS_LED_CONFIG
		andi	r17, (1 << STATUS_LED_ENABLED)
		tst		r17
		breq	pwm_led0_green_on
		
		lds 	r17, PWM_STATUS_LED_GREEN	
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led0_green_work
		com		r17
		
		pwm_led0_green_work:
			cp		akku, r17
			brlo	pwm_led0_green_off
			rjmp	pwm_led0_green_on
		
		pwm_led0_green_on:
			sbi		LED0_GREEN_PORT, LED0_GREEN_PIN
			rjmp	pwm_led0_green_return
		
		pwm_led0_green_off:
			cbi		LED0_GREEN_PORT, LED0_GREEN_PIN
			rjmp 	pwm_led0_green_return
		
	pwm_led0_green_return:
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'blue' of LED0
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led0_blue:
		lds		r17, STATUS_LED_CONFIG
		andi	r17, (1 << STATUS_LED_ENABLED)
		tst		r17
		breq	pwm_led0_blue_on
		
		lds 	r17, PWM_STATUS_LED_BLUE		
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led0_blue_work
		com		r17
		
		pwm_led0_blue_work:
			cp		akku, r17
			brlo	pwm_led0_blue_off
			rjmp	pwm_led0_blue_on
		
		pwm_led0_blue_on:
			sbi		LED0_BLUE_PORT, LED0_BLUE_PIN
			rjmp	pwm_led0_blue_return
		
		pwm_led0_blue_off:
			cbi		LED0_BLUE_PORT, LED0_BLUE_PIN
			rjmp 	pwm_led0_blue_return
		
	pwm_led0_blue_return:
		ret



; ----------------------------------------------------------------------------
; LED Module 1
; ----------------------------------------------------------------------------

	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for setting the software-pwm duty cycle of LED1 to the
	; //! appropriate RGB-tripel
	; ////////////////////////////////////////////////////////////////////////	

	pwm_led1_set_rgb_tripel:
		lds		akku, LED_RED
		sts		PWM_RED, akku
		
		lds		akku, LED_GREEN
		sts		PWM_GREEN, akku

		lds		akku, LED_BLUE
		sts		PWM_BLUE, akku
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'red' of LED1
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led1_red:
		lds		r17, PWM_RED
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led1_red_work
		com		r17
		
		pwm_led1_red_work:
			cp		akku, r17
			brlo	pwm_led1_red_on
			rjmp	pwm_led1_red_off
		
		pwm_led1_red_on:
			sbi		LED1_RED_PORT, LED1_RED_PIN
			rjmp	pwm_led1_red_return
		
		pwm_led1_red_off:
			cbi		LED1_RED_PORT, LED1_RED_PIN
			rjmp 	pwm_led1_red_return
		
	pwm_led1_red_return:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'green' of LED1
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led1_green:		
		lds		r17, PWM_GREEN
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led1_green_work
		com		r17
		
		pwm_led1_green_work:
			cp		akku, r17
			brlo	pwm_led1_green_on
			rjmp	pwm_led1_green_off
		
		pwm_led1_green_on:
			sbi		LED1_GREEN_PORT, LED1_GREEN_PIN
			rjmp	pwm_led1_green_return
		
		pwm_led1_green_off:
			cbi		LED1_GREEN_PORT, LED1_GREEN_PIN
			rjmp 	pwm_led1_green_return
		
	pwm_led1_green_return:
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'blue' of LED1
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led1_blue:
		lds		r17, PWM_BLUE
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led1_blue_work
		com		r17
		
		pwm_led1_blue_work:
			cp		akku, r17
			brlo	pwm_led1_blue_on
			rjmp	pwm_led1_blue_off
		
		pwm_led1_blue_on:
			sbi		LED1_BLUE_PORT, LED1_BLUE_PIN
			rjmp	pwm_led1_blue_return
		
		pwm_led1_blue_off:
			cbi		LED1_BLUE_PORT, LED1_BLUE_PIN
			rjmp 	pwm_led1_blue_return
		
	pwm_led1_blue_return:
		ret


; ----------------------------------------------------------------------------
; LED Module 2
; ----------------------------------------------------------------------------	

	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for setting the software-pwm duty cycle of LED2 to the
	; //! appropriate RGB-tripel
	; ////////////////////////////////////////////////////////////////////////	

	pwm_led2_set_rgb_tripel:
		lds		akku, LED_RED+1
		sts		PWM_RED+1, akku
		
		lds		akku, LED_GREEN+1
		sts		PWM_GREEN+1, akku

		lds		akku, LED_BLUE+1
		sts		PWM_BLUE+1, akku
		ret

		
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'red' of LED2
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led2_red:
		lds		r17, PWM_RED+1
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led2_red_work
		com		r17
		
		pwm_led2_red_work:
			cp		akku, r17
			brlo	pwm_led2_red_on
			rjmp	pwm_led2_red_off
		
		pwm_led2_red_on:
			sbi		LED2_RED_PORT, LED2_RED_PIN
			rjmp	pwm_led2_red_return
		
		pwm_led2_red_off:
			cbi		LED2_RED_PORT, LED2_RED_PIN
			rjmp 	pwm_led2_red_return
		
	pwm_led2_red_return:
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'green' of LED2
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led2_green:
		lds		r17, PWM_GREEN+1
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led2_green_work
		com		r17
		
		pwm_led2_green_work:
			cp		akku, r17
			brlo	pwm_led2_green_on
			rjmp	pwm_led2_green_off
		
		pwm_led2_green_on:
			sbi		LED2_GREEN_PORT, LED2_GREEN_PIN
			rjmp	pwm_led2_green_return
		
		pwm_led2_green_off:
			cbi		LED2_GREEN_PORT, LED2_GREEN_PIN
			rjmp 	pwm_led2_green_return
		
	pwm_led2_green_return:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'blue' of LED2
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led2_blue:
		lds 	r17, PWM_BLUE+1
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led2_blue_work
		com		r17
		
		pwm_led2_blue_work:
			cp		akku, r17
			brlo	pwm_led2_blue_on
			rjmp	pwm_led2_blue_off
		
		pwm_led2_blue_on:
			sbi		LED2_BLUE_PORT, LED2_BLUE_PIN
			rjmp	pwm_led2_blue_return
		
		pwm_led2_blue_off:
			cbi		LED2_BLUE_PORT, LED2_BLUE_PIN
			rjmp 	pwm_led2_blue_return
		
	pwm_led2_blue_return:
		ret
	
	
	
; ----------------------------------------------------------------------------
; LED Module 3
; ----------------------------------------------------------------------------
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for setting the software-pwm duty cycle of LED3 to the
	; //! appropriate RGB-tripel
	; ////////////////////////////////////////////////////////////////////////	

	pwm_led3_set_rgb_tripel:
		lds		akku, LED_RED+2
		sts		PWM_RED+2, akku
		
		lds		akku, LED_GREEN+2
		sts		PWM_GREEN+2, akku

		lds		akku, LED_BLUE+2
		sts		PWM_BLUE+2, akku
		ret

		
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'red' of LED3
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led3_red:
		lds		r17, PWM_RED+2
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led3_red_work
		com		r17
		
		pwm_led3_red_work:
			cp		akku, r17
			brlo	pwm_led3_red_on
			rjmp	pwm_led3_red_off
		
		pwm_led3_red_on:
			sbi		LED3_RED_PORT, LED3_RED_PIN
			rjmp	pwm_led3_red_return
		
		pwm_led3_red_off:
			cbi		LED3_RED_PORT, LED3_RED_PIN
			rjmp 	pwm_led3_red_return
		
	pwm_led3_red_return:
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'green' of LED3
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led3_green:
		lds		r17, PWM_GREEN+2
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led3_green_work
		com		r17
		
		pwm_led3_green_work:
			cp		akku, r17
			brlo	pwm_led3_green_on
			rjmp	pwm_led3_green_off
		
		pwm_led3_green_on:
			sbi		LED3_GREEN_PORT, LED3_GREEN_PIN
			rjmp	pwm_led3_green_return
		
		pwm_led3_green_off:
			cbi		LED3_GREEN_PORT, LED3_GREEN_PIN
			rjmp 	pwm_led3_green_return
		
	pwm_led3_green_return:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'blue' of LED3
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led3_blue:
		lds 	r17, PWM_BLUE+2
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led3_blue_work
		com		r17
		
		pwm_led3_blue_work:
			cp		akku, r17
			brlo	pwm_led3_blue_on
			rjmp	pwm_led3_blue_off
		
		pwm_led3_blue_on:
			sbi		LED3_BLUE_PORT, LED3_BLUE_PIN
			rjmp	pwm_led3_blue_return
		
		pwm_led3_blue_off:
			cbi		LED3_BLUE_PORT, LED3_BLUE_PIN
			rjmp 	pwm_led3_blue_return
		
	pwm_led3_blue_return:
		ret


; ----------------------------------------------------------------------------
; LED Module 4
; ----------------------------------------------------------------------------

	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for setting the software-pwm duty cycle of LED4 to the
	; //! appropriate RGB-tripel
	; ////////////////////////////////////////////////////////////////////////	

	pwm_led4_set_rgb_tripel:
		lds		akku, LED_RED+3
		sts		PWM_RED+3, akku
		
		lds		akku, LED_GREEN+3
		sts		PWM_GREEN+3, akku

		lds		akku, LED_BLUE+3
		sts		PWM_BLUE+3, akku
		ret

		
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'red' of LED4
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led4_red:
		lds 	r17, PWM_RED+3
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led4_red_work
		com		r17
		
		pwm_led4_red_work:
			cp		akku, r17
			brlo	pwm_led4_red_on
			rjmp	pwm_led4_red_off
		
		pwm_led4_red_on:
			sbi		LED4_RED_PORT, LED4_RED_PIN
			rjmp	pwm_led4_red_return
		
		pwm_led4_red_off:
			cbi		LED4_RED_PORT, LED4_RED_PIN
			rjmp 	pwm_led4_red_return
		
	pwm_led4_red_return:
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'green' of LED4
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led4_green:
		lds 	r17, PWM_GREEN+3
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led4_green_work
		com		r17
		
		pwm_led4_green_work:
			cp		akku, r17
			brlo	pwm_led4_green_on
			rjmp	pwm_led4_green_off
		
		pwm_led4_green_on:
			sbi		LED4_GREEN_PORT, LED4_GREEN_PIN
			rjmp	pwm_led4_green_return
		
		pwm_led4_green_off:
			cbi		LED4_GREEN_PORT, LED4_GREEN_PIN
			rjmp 	pwm_led4_green_return
		
	pwm_led4_green_return:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! Subroutine for enabling or disabling the channel 'blue' of LED4
	; //! dependent on the configured duty cycle in SRAM and the actual 
	; //! value of the PWM counter.
	; ////////////////////////////////////////////////////////////////////////
	
	pwm_led4_blue:
		lds 	r17, PWM_BLUE+3
		mov		r18, r17
		andi	r18, 0b10000000
		cpi		r18, 0x00
		breq	pwm_led4_blue_work
		com		r17
		
		pwm_led4_blue_work:
			cp		akku, r17
			brlo	pwm_led4_blue_on
			rjmp	pwm_led4_blue_off
		
		pwm_led4_blue_on:
			sbi		LED4_BLUE_PORT, LED4_BLUE_PIN
			rjmp	pwm_led4_blue_return
		
		pwm_led4_blue_off:
			cbi		LED4_BLUE_PORT, LED4_BLUE_PIN
			rjmp 	pwm_led4_blue_return
		
	pwm_led4_blue_return:
		ret

