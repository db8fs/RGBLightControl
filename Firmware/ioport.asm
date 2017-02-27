//////////////////////////////////////////////////////////////////////////////
; //! @file 		ioport.asm
; //! @description	Subroutines for controlling the I/O port behaviour.
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
//////////////////////////////////////////////////////////////////////////////

; ----------------------------------------------------------------------------
; Initialize I/O 
; ----------------------------------------------------------------------------

ioport_initialization:
		push	akku
		in		akku, SREG
		push	akku		
		
		
		; init status led pins as output
		
		in		akku, LED0_RED_DDR
		ori		akku, (1 << LED0_RED_PIN)
		out		LED0_RED_DDR, akku
		
		in		akku, LED0_GREEN_DDR
		ori		akku, (1 << LED0_GREEN_PIN)
		out		LED0_GREEN_DDR, akku
		
		in		akku, LED0_BLUE_DDR
		ori		akku, (1 << LED0_BLUE_PIN)
		out		LED0_BLUE_DDR, akku
		
		
		; init led module 1 pins as output

		in		akku, LED1_RED_DDR
		ori		akku, (1 << LED1_RED_PIN)
		out		LED1_RED_DDR, akku
		
		in		akku, LED1_GREEN_DDR
		ori		akku, (1 << LED1_GREEN_PIN)
		out		LED1_GREEN_DDR, akku

		in		akku, LED1_BLUE_DDR
		ori		akku, (1 << LED1_BLUE_PIN)
		out		LED1_BLUE_DDR, akku
		
		
		; init led module 2 pins as output

		in		akku, LED2_RED_DDR
		ori		akku, (1 << LED2_RED_PIN)
		out		LED2_RED_DDR, akku
		
		in		akku, LED2_GREEN_DDR
		ori		akku, (1 << LED2_GREEN_PIN)
		out		LED2_GREEN_DDR, akku

		in		akku, LED2_BLUE_DDR
		ori		akku, (1 << LED2_BLUE_PIN)
		out		LED2_BLUE_DDR, akku
		
		
		; init led module 3 pins as output
				
		in		akku, LED3_RED_DDR
		ori		akku, (1 << LED3_RED_PIN)
		out		LED3_RED_DDR, akku
		
		in		akku, LED3_GREEN_DDR
		ori		akku, (1 << LED3_GREEN_PIN)
		out		LED3_GREEN_DDR, akku

		in		akku, LED3_BLUE_DDR
		ori		akku, (1 << LED3_BLUE_PIN)
		out		LED3_BLUE_DDR, akku
		

		; init led module 4 pins as output
				
		in		akku, LED4_RED_DDR
		ori		akku, (1 << LED4_RED_PIN)
		out		LED4_RED_DDR, akku
		
		in		akku, LED4_GREEN_DDR
		ori		akku, (1 << LED4_GREEN_PIN)
		out		LED4_GREEN_DDR, akku

		in		akku, LED4_BLUE_DDR
		ori		akku, (1 << LED4_BLUE_PIN)
		out		LED4_BLUE_DDR, akku


		; init pushbuttons

		in		akku, PUSHBUTTON1_DDR
		andi	akku, ~(1 << PUSHBUTTON1_PIN)
		out		PUSHBUTTON1_DDR, akku
		
		in		akku, PUSHBUTTON2_DDR
		andi	akku, ~(1 << PUSHBUTTON2_PIN)
		out		PUSHBUTTON2_DDR, akku

		in		akku, PUSHBUTTON3_DDR
		andi	akku, ~(1 << PUSHBUTTON3_PIN)
		out		PUSHBUTTON3_DDR, akku
		
		pop		akku
		out		SREG, akku
		pop		akku
		ret


; ----------------------------------------------------------------------------
; Debounce and read in status of pushbuttons
; ----------------------------------------------------------------------------


ioport_check_pushbuttons:
		push 	akku
		in 		akku, SREG
		push	akku
		
		rcall 	ioport_check_pushbutton1
		rcall	ioport_check_pushbutton2
		rcall	ioport_check_pushbutton3
		
		pop		akku
		out 	SREG, akku
		pop		akku
		ret
		
		
; ////////////////////////////////////////////////////////////////////////////
; //! This subroutine executes the debounce operation on pushbutton 1. If the
; //! button was pressed, we will execute the selected function of the 
; //! callback table. Which function of the callback table will be selected 
; //! depends on the setting in the appropriate "pushbutton config
; //! register" in SRAM. Using this way we are able to reconfigure the
; //! behaviour for reacting on pushbutton events via UART 
; //! (we may map user input (button presses) during runtime to different 
; //! functions).
; //////////////////////////////////////////////////////////////////////////// 		

ioport_check_pushbutton1:
		mov 	pushbutton_debounce_temp, pushbutton_debounce
		in		akku,	PUSHBUTTON1_PORT
		andi	akku,	(1 << PUSHBUTTON1_PIN)
		cpi		akku, 0x00
		brne	ioport_check_pushbutton1_set_button_bit
		rjmp	ioport_check_pushbutton1_clear_button_bit
		
ioport_check_pushbutton1_set_button_bit:
		ldi		akku, (1 << 0)
		or		pushbutton_debounce, akku
		rjmp	ioport_check_pushbutton1_debounce
		
ioport_check_pushbutton1_clear_button_bit:
		ldi		akku, ~(1 << 0)
		and		pushbutton_debounce, akku
		rjmp	ioport_check_pushbutton1_debounce
		
ioport_check_pushbutton1_debounce:
		mov		akku, pushbutton_debounce_temp
		eor		akku, pushbutton_debounce
		andi	akku, (1 << 0)
		cpi		akku, 0x00
		breq 	ioport_check_pushbutton1_return
		
		; determine if key was pressed or released
		mov		akku, pushbutton_debounce_temp
		andi	akku, (1 << 0)
		cpi		akku, 0x00
		brne	ioport_check_pushbutton1_released_event
		rjmp	ioport_check_pushbutton1_pressed_event
		

ioport_check_pushbutton1_pressed_event:
		ldi		XL, LOW(PUSHBUTTON_CONFIGURATION)
		ldi		XH, HIGH(PUSHBUTTON_CONFIGURATION)
		ld		akku, X
		andi	akku, PUSHBUTTON_ACTION_ON_PRESSED_EVENT
		cpi		akku, 0x00
		brne	ioport_check_pushbutton1_execute_pressed_function
		rjmp 	ioport_check_pushbutton1_return


ioport_check_pushbutton1_released_event:
		ldi		XL, LOW(PUSHBUTTON_CONFIGURATION)
		ldi		XH, HIGH(PUSHBUTTON_CONFIGURATION)
		ld		akku, X
		andi	akku, PUSHBUTTON_ACTION_ON_RELEASED_EVENT
		cpi		akku, 0x00
		brne	ioport_check_pushbutton1_execute_released_function
		rjmp 	ioport_check_pushbutton1_return
		
		
ioport_check_pushbutton1_execute_pressed_function:
		lds		akku, STATUS_LED_CONFIG
		ori		akku, (1 << STATUS_LED_ENABLED)
		sts		STATUS_LED_CONFIG, akku
		ret
		
ioport_check_pushbutton1_execute_released_function:
		lds		akku, STATUS_LED_CONFIG
		andi	akku, ~(1 << STATUS_LED_ENABLED)
		sts		STATUS_LED_CONFIG, akku
		ret
		
ioport_check_pushbutton1_return:
		ret




; ////////////////////////////////////////////////////////////////////////////
; //! This subroutine executes the same black magic for pushbutton 2 like the
; //! routines above for button 1.
; //////////////////////////////////////////////////////////////////////////// 		

ioport_check_pushbutton2:
		mov 	pushbutton_debounce_temp, pushbutton_debounce
		in		akku,	PUSHBUTTON2_PORT
		andi	akku,	(1 << PUSHBUTTON2_PIN)
		cpi		akku, 0x00
		brne	ioport_check_pushbutton2_set_button_bit
		rjmp	ioport_check_pushbutton2_clear_button_bit
		
ioport_check_pushbutton2_set_button_bit:
		ldi		akku, (1 << 1)
		or		pushbutton_debounce, akku
		rjmp	ioport_check_pushbutton2_debounce
		
ioport_check_pushbutton2_clear_button_bit:
		ldi		akku, ~(1 << 1)
		and		pushbutton_debounce, akku
		rjmp	ioport_check_pushbutton2_debounce
		
ioport_check_pushbutton2_debounce:
		mov		akku, pushbutton_debounce_temp
		eor		akku, pushbutton_debounce
		andi	akku, (1 << 1)
		cpi		akku, 0x00
		breq 	ioport_check_pushbutton2_return
		mov		akku, pushbutton_debounce_temp
		andi	akku, (1 << 1)
		cpi		akku, 0x00
		brne	ioport_check_pushbutton2_released_event
		rjmp	ioport_check_pushbutton2_pressed_event
		

ioport_check_pushbutton2_pressed_event:
		ldi		XL, LOW(PUSHBUTTON_CONFIGURATION+1)
		ldi		XH, HIGH(PUSHBUTTON_CONFIGURATION+1)
		ld		akku, X
		andi	akku, PUSHBUTTON_ACTION_ON_PRESSED_EVENT
		cpi		akku, 0x00
		brne	ioport_check_pushbutton2_execute_pressed_function
		rjmp 	ioport_check_pushbutton2_return


ioport_check_pushbutton2_released_event:
		ldi		XL, LOW(PUSHBUTTON_CONFIGURATION+1)
		ldi		XH, HIGH(PUSHBUTTON_CONFIGURATION+1)
		ld		akku, X
		andi	akku, PUSHBUTTON_ACTION_ON_RELEASED_EVENT
		cpi		akku, 0x00
		brne	ioport_check_pushbutton2_execute_released_function
		rjmp 	ioport_check_pushbutton2_return


ioport_check_pushbutton2_execute_pressed_function:
		ret

ioport_check_pushbutton2_execute_released_function:
		rcall	colorcontrol_next_function
		ret
		
ioport_check_pushbutton2_return:
		ret



; ////////////////////////////////////////////////////////////////////////////
; //! This subroutine executes the same black magic for pushbutton 3 like the
; //! routines above for button 1 and 2.
; //////////////////////////////////////////////////////////////////////////// 		

ioport_check_pushbutton3:
		mov 	pushbutton_debounce_temp, pushbutton_debounce
		in		akku,	PUSHBUTTON3_PORT
		andi	akku,	(1 << PUSHBUTTON3_PIN)
		cpi		akku, 0x00
		brne	ioport_check_pushbutton3_set_button_bit
		rjmp	ioport_check_pushbutton3_clear_button_bit
		
ioport_check_pushbutton3_set_button_bit:
		ldi		akku, (1 << 2)
		or		pushbutton_debounce, akku
		rjmp	ioport_check_pushbutton3_debounce
		
ioport_check_pushbutton3_clear_button_bit:
		ldi		akku, ~(1 << 2)
		and		pushbutton_debounce, akku
		rjmp	ioport_check_pushbutton3_debounce
		
ioport_check_pushbutton3_debounce:
		mov		akku, pushbutton_debounce_temp
		eor		akku, pushbutton_debounce
		andi	akku, (1 << 2)
		cpi		akku, 0x00
		breq 	ioport_check_pushbutton3_return
		mov		akku, pushbutton_debounce_temp
		andi	akku, (1 << 2)
		cpi		akku, 0x00
		brne	ioport_check_pushbutton3_released_event
		rjmp	ioport_check_pushbutton3_pressed_event
		

ioport_check_pushbutton3_pressed_event:
		ldi		XL, LOW(PUSHBUTTON_CONFIGURATION+2)
		ldi		XH, HIGH(PUSHBUTTON_CONFIGURATION+2)
		ld		akku, X
		andi	akku, PUSHBUTTON_ACTION_ON_PRESSED_EVENT
		cpi		akku, 0x00
		brne	ioport_check_pushbutton3_execute_pressed_function
		rjmp 	ioport_check_pushbutton3_return
		
		
ioport_check_pushbutton3_released_event:
		ldi		XL, LOW(PUSHBUTTON_CONFIGURATION+2)
		ldi		XH, HIGH(PUSHBUTTON_CONFIGURATION+2)
		ld		akku, X
		andi	akku, PUSHBUTTON_ACTION_ON_RELEASED_EVENT
		cpi		akku, 0x00
		brne	ioport_check_pushbutton3_execute_released_function
		rjmp 	ioport_check_pushbutton3_return
		
		
ioport_check_pushbutton3_execute_pressed_function:
		ret

ioport_check_pushbutton3_execute_released_function:
		ret
		
ioport_check_pushbutton3_return:
		ret

