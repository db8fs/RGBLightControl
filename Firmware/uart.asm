; ////////////////////////////////////////////////////////////////////////////
; //! @file 		uart.asm
; //! @description	Subroutines for receiving and transmitting data using the
; //!				Universal Asynchronous Receiver and Transmitter (UART).
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////
	
	
; ----------------------------------------------------------------------------
; Baudrate calculations
; ----------------------------------------------------------------------------


.EQU	UBRR_VAL		= ((F_CPU + BAUD*8)/(BAUD*16)-1)
.EQU	BAUD_REAL		= (F_CPU / (16*(UBRR_VAL+1)))
.EQU	BAUD_ERROR		= ((BAUD_REAL*1000)/BAUD-1000) 
 

; ----------------------------------------------------------------------------
; Subroutines
; ----------------------------------------------------------------------------
	

	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine initializes the UART of the AVR.
	; ////////////////////////////////////////////////////////////////////////
	
	uart_init:
		push 	akku
		in	 	akku, SREG
		push	akku
		
		ldi		akku, HIGH(UBRR_VAL)
		out		UBRRH, akku
		ldi		akku, LOW(UBRR_VAL)
		out		UBRRL, akku
		
		ldi		akku, (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0)
		out		UCSRC, akku
		
		ldi		akku, (1 << TXEN) | (1 << RXEN) | (1 << RXCIE)
		out		UCSRB, akku
		
		ldi		akku, UART_STATE_READY
		sts		UART_STATE, akku		
		
		pop		akku
		out 	SREG, akku
		pop		akku
		ret
	

	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine fills the message buffer until a valid message is
	; //! received.
	; ////////////////////////////////////////////////////////////////////////	
	
	uart_check_receiver:
		sbis	UCSRA, RXC
		rjmp	uart_check_receiver_finished
		
		ldi		ZL, LOW(UART_MESSAGE_BUFFER)
		ldi		ZH, HIGH(UART_MESSAGE_BUFFER)
		lds 	akku, UART_STATE
		add		ZL, akku
		clr		akku
		adc		ZH, akku
		
		in		param, UDR
		st		Z, param		
		
		lds		akku, UART_STATE		
		inc		akku
		sts		UART_STATE, akku
		
		cpi		akku, UART_STATE_MAGIC_BYTE
		breq	uart_check_receiver_magic_byte		
		
		cpi		akku, UART_STATE_CHECKSUM
		breq	uart_check_receiver_message_complete
		
		rjmp	uart_check_receiver_finished
		
	uart_check_receiver_message_complete:
		ldi		akku, UART_STATE_READY
		sts		UART_STATE, akku
		clr		akku
		
		rcall	uart_calculate_message_checksum		
		cpse	param, akku
		rjmp	uart_check_receiver_finished
		
		rcall	uart_decode_led_selection_byte
		
		ldi		ZL, LOW(UART_MESSAGE_COMMAND_RECEIVED*2)
		ldi		ZH, HIGH(UART_MESSAGE_COMMAND_RECEIVED*2)
		rcall	uart_transmit_string
		rjmp	uart_check_receiver_finished
		
	uart_check_receiver_magic_byte:
		mov		akku, param
		cpi		akku, UART_MESSAGE_MAGIC_BYTE
		breq	uart_check_receiver_finished
		
		ldi		akku, UART_STATE_READY
		sts		UART_STATE, akku
		
		rjmp	uart_check_receiver_finished
		
	uart_check_receiver_finished:
		ret
	
	
	; /////////////////////////////////////////////////////////////////////////
	; // Calculates the checksum for the message buffer and returns it in r16.
	; /////////////////////////////////////////////////////////////////////////	
	
	uart_calculate_message_checksum:
#ifdef	_NO_CHECKSUM_CALCULATION_
		mov 	akku, param
#else
		push	param
		push	counter
		
		clr		r0
		clr 	r1
		
		; start with led select byte
		ldi		ZL, LOW(UART_MESSAGE_BUFFER+1)
		ldi		ZH, HIGH(UART_MESSAGE_BUFFER+1)		
		
		; stop before checksum byte (no magic byte, no checksum byte -> 2)
		ldi		counter, UART_MESSAGE_LENGTH-2
		
		uart_calculate_message_checksum_loop:		
		ld		akku, Z+
		add		r1, akku
		clr		akku
		adc		r0, akku
		
		dec		counter
		tst		counter
		brne	uart_calculate_message_checksum_loop
		
		; the low byte of the accumulated message bytes is the checksum
		mov 	akku, r1
		
		pop		counter
		pop		param
#endif
		ret


	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine decodes the message in the message buffer and 
	; //! executes the received command.
	; //!
	; //! The UART_LED_SELECT byte has the following format:
	; //! Bit 7654 | 3210
	; //!     ^^^^  ^^^^
	; //! COLOR[3..0] | LED[3..0]
	; //! 
	; //! The LED nibble addresses the LEDs for which the following command
	; //! will be applied. E.g. a low nibble of 0100 will lead to a mode change
	; //! for LED group 3, a low nibble of 1011 changes the mode for LED groups 
	; //! 1,2 and 4.
	; //! 
	; //! The COLOR nibble determines whether the following will influence the
	; //! RGB or the HSI values of the addressed LED groups. A high nibble of
	; //! 1101 (assumed that the low nibble addresses all led groups: 1111) will 
	; //! lead to HSI color mode for LED groups 1,3 and 4 and RGB color mode for 
	; //! LED group 2.
	; //! 
	; //! An additional example:
	; //! UART_LED_SELECT byte: 0b10100110
	; //! 
	; //! Result: LED group 2 and 3 are selected, group 2 uses HSI mode,
	; //! group 3 uses RGB mode. The other led groups are not affected.
	; ////////////////////////////////////////////////////////////////////////
	
	uart_decode_led_selection_byte:
		ldi		akku, NUMBER_OF_LEDS-1
		mov		param, akku
		ldi		counter, (1 << NUMBER_OF_LEDS-1)	; start with bit 3 (LED group 4)
		
	uart_decode_led_selection_byte_loop:
		lds		akku, UART_MESSAGE_BUFFER+1		; selected leds
		and		akku, counter
		tst		akku
		breq	uart_decode_led_selection_byte_next
		
		; ok, led group was selected, now get the colormode
		lds		akku, UART_MESSAGE_BUFFER+1
		swap	akku
		and		akku, counter
		tst		akku
		breq	uart_decode_led_selection_byte_rgb_mode
		rjmp	uart_decode_led_selection_byte_hsi_mode
		
		uart_decode_led_selection_byte_rgb_mode:
		rcall	uart_decode_command_message_rgb_mode
		rjmp	uart_decode_led_selection_byte_next
		
		uart_decode_led_selection_byte_hsi_mode:
		rcall	uart_decode_command_message_hsi_mode
		rjmp	uart_decode_led_selection_byte_next
		
	uart_decode_led_selection_byte_next:
		dec		param
		lsr 	counter
		tst		counter
		brne	uart_decode_led_selection_byte_loop
		
		ret
		
	
	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine decodes the received command byte as a RGB 
	; //! command. The received payload bytes of the message are therefore
	; //! interpretes as RGB tripels for the selected LED group. Which 
	; //! LED group is affected is addressed in the 'param' register.
	; ////////////////////////////////////////////////////////////////////////
	
	uart_decode_command_message_rgb_mode:
		ldi		ZL, LOW(LED_COLOR_MODE)
		ldi		ZH, HIGH(LED_COLOR_MODE)
		add		ZL, param
		clr		akku
		adc		ZH, akku
		ldi		akku, LED_COLOR_MODE_RGB
		st		Z, akku
		
		ldi		ZL, LOW(LED_CONFIGURATION)
		ldi		ZH, HIGH(LED_CONFIGURATION)
		add		ZL, param
		clr		akku
		adc		ZH, akku
		
		lds		akku, UART_MESSAGE_BUFFER+2		; apply command for selected led
		st		Z, akku
		
		ldi		ZL, LOW(UART_MESSAGE_BUFFER+3) 	; calculate position of RGB tripel in received message
		ldi		ZH, HIGH(UART_MESSAGE_BUFFER+3)
		ldi		akku, 3
		mul		param, akku
		add		ZL, r0
		adc 	ZH, r1
		
		; store red channel
		ldi		YL, LOW(LED_RED)
		ldi		YH, HIGH(LED_RED)
		add		YL, param
		clr 	akku
		adc		YH, akku
		
		ld		akku, Z+
		st		Y, akku
		
		; store green channel
		ldi		YL, LOW(LED_GREEN)
		ldi		YH, HIGH(LED_GREEN)
		add		YL, param
		clr		akku
		adc		YH, akku
		
		ld		akku, Z+
		st		Y, akku
		
		; store blue channel
		ldi		YL, LOW(LED_BLUE)
		ldi		YH, HIGH(LED_BLUE)
		add		YL, param
		clr 	akku
		adc		YH, akku
		
		ld		akku, Z
		st		Y, akku
		
		ret
		
		
	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine decodes the received command byte as a HSI command.
	; //! The received payload bytes of the message are therefore
	; //! interpreted as HSI tripels for the selected LED group. Which 
	; //! LED group is affected is addressed in the 'param' register.
	; ////////////////////////////////////////////////////////////////////////
	
	uart_decode_command_message_hsi_mode:
		ldi		ZL, LOW(LED_COLOR_MODE)
		ldi		ZH, HIGH(LED_COLOR_MODE)
		add		ZL, param
		clr		akku
		adc		ZH, akku
		ldi		akku, LED_COLOR_MODE_HSI
		st		Z, akku
		
		ldi		ZL, LOW(LED_CONFIGURATION)
		ldi		ZH, HIGH(LED_CONFIGURATION)
		add		ZL, param
		clr		akku
		adc		ZH, akku

		lds		akku, UART_MESSAGE_BUFFER+2		; apply command for selected led
		st		Z, akku
		
		ldi		ZL, LOW(UART_MESSAGE_BUFFER+3) 	; calculate position of HSI tripel in received message
		ldi		ZH, HIGH(UART_MESSAGE_BUFFER+3)
		ldi		akku, 3
		mul		param, akku
		add		ZL, r0
		adc 	ZH, r1
		
		; store hue channel
		ldi		YL, LOW(LED_HUE)
		ldi		YH, HIGH(LED_HUE)
		add		YL, param
		clr 	akku
		adc		YH, akku
		
		ld		akku, Z+
		st		Y, akku
		
		; skip saturation channel (is assumed to be full saturated)
		ld		akku, Z+
		
		; store intensity channel
		ldi		YL, LOW(LED_INTENSITY)
		ldi		YH, HIGH(LED_INTENSITY)
		add		YL, param
		clr 	akku
		adc		YH, akku
		
		ld		akku, Z
		st		Y, akku
		ret
		
		
	; ////////////////////////////////////////////////////////////////////////
	; //! This subroutine returns the content of the message buffer.
	; ////////////////////////////////////////////////////////////////////////
	
	uart_transmit_message_buffer:
		ldi 	counter, UART_MESSAGE_LENGTH
		ldi		ZL, LOW(UART_MESSAGE_BUFFER)
		ldi		ZH, HIGH(UART_MESSAGE_BUFFER)
		
	uart_transmit_message_buffer_loop:
		ld		param, Z+
		rcall	uart_transmit_byte
		
		dec		counter
		cpi		counter, 0x00
		brne	uart_transmit_message_buffer_loop
		
		rcall	uart_transmit_newline
		ret	
		
		
	; ////////////////////////////////////////////////////////////////////////
	; // Sends a greeting message via UART.
	; ////////////////////////////////////////////////////////////////////////	
	
	uart_transmit_greeting:
		ldi		ZL, LOW(UART_MESSAGE_GREETING*2)
		ldi		ZH, HIGH(UART_MESSAGE_GREETING*2)
		rcall 	uart_transmit_string
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; // Loads the string addressed by the Z-register in program memory.
	; ////////////////////////////////////////////////////////////////////////
	
	uart_transmit_string:
		lpm
		and		r0, r0
		breq	uart_transmit_string_finished
	
	uart_transmit_string_wait:
		sbis	UCSRA, UDRE
		rjmp	uart_transmit_string_wait
		out		UDR, r0
		adiw	ZL:ZH, 1
		rjmp	uart_transmit_string
		
	uart_transmit_string_finished:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	
	uart_transmit_newline:
		ldi		ZL, LOW(UART_MESSAGE_NEWLINE*2)
		ldi		ZH, HIGH(UART_MESSAGE_NEWLINE*2)
		rcall 	uart_transmit_string
		
		ret	
	
	; ////////////////////////////////////////////////////////////////////////
	
	uart_transmit_byte:
		sbis 	UCSRA, UDRE
		rjmp	uart_transmit_byte
		out		UDR, param
		ret
	
	
	
