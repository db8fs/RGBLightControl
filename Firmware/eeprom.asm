; ////////////////////////////////////////////////////////////////////////////
; //! @file 		eeprom.asm
; //! @description	Subroutines for reading and writing configuration data
; //!				into EEPROM.
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////


; ----------------------------------------------------------------------------
; Reading configuration data from EEPROM.
; ----------------------------------------------------------------------------

eeprom_load_configuration:
		push	akku
		push	param
		clr		param

		cli
		rcall 	eeprom_pushbutton_configuration_apply
		sei

		pop		param
		pop		akku
		ret


; ----------------------------------------------------------------------------
; Storing configuration data into EEPROM.
; ----------------------------------------------------------------------------

eeprom_store_configuration:
		push	akku
		push	param
		ldi		akku, 0xff
		mov		param, akku
		
		cli
		rcall 	eeprom_pushbutton_configuration_apply
		sei
		
		pop		param
		pop		akku
		ret

		
		
 ; ---------------------------------------------------------------------------

; ////////////////////////////////////////////////////////////////////////////
; // This function reads or writes the pushbutton configuration into SRAM.
; ////////////////////////////////////////////////////////////////////////////

eeprom_pushbutton_configuration_apply:
		push	counter
		sbic	EECR, EEWE
		rjmp	eeprom_pushbutton_configuration_apply
		
		; configure target address in SRAM
		ldi		XL, LOW(PUSHBUTTON_CONFIGURATION)
		ldi		XH, HIGH(PUSHBUTTON_CONFIGURATION)
		ldi		ZL, LOW(EEPROM_PUSHBUTTON_CONFIGURATION)
		ldi		ZH, HIGH(EEPROM_PUSHBUTTON_CONFIGURATION)
		ldi		counter, NUMBER_OF_PUSHBUTTONS		
		
eeprom_pushbutton_configuration_mode_settings:
		rcall 	eeprom_read_or_write_byte
		
		dec		counter
		tst		counter
		brne	eeprom_pushbutton_configuration_mode_settings
		
		ldi		XL, LOW(PUSHBUTTON_PRESSED_CALLBACK_TABLE)
		ldi		XH, HIGH(PUSHBUTTON_PRESSED_CALLBACK_TABLE)
		ldi		ZL, LOW(EEPROM_PUSHBUTTON_PRESSED_CALLBACK_TABLE)
		ldi		ZH, HIGH(EEPROM_PUSHBUTTON_PRESSED_CALLBACK_TABLE)
		ldi		counter, NUMBER_OF_PUSHBUTTONS
		
eeprom_pushbutton_configuration_pressed_callbacks:
		rcall 	eeprom_read_or_write_byte
		rcall 	eeprom_read_or_write_byte
		
		dec		counter
		tst		counter
		brne	eeprom_pushbutton_configuration_pressed_callbacks
		
		ldi		XL, LOW(PUSHBUTTON_RELEASED_CALLBACK_TABLE)
		ldi		XH, HIGH(PUSHBUTTON_RELEASED_CALLBACK_TABLE)
		ldi		ZL, LOW(EEPROM_PUSHBUTTON_RELEASED_CALLBACK_TABLE)
		ldi		ZH, HIGH(EEPROM_PUSHBUTTON_RELEASED_CALLBACK_TABLE)
		ldi		counter, NUMBER_OF_PUSHBUTTONS
		
eeprom_pushbutton_configuration_released_callbacks:
		rcall 	eeprom_read_or_write_byte
		rcall 	eeprom_read_or_write_byte
		
		dec		counter
		tst		counter
		brne	eeprom_pushbutton_configuration_released_callbacks
		
		pop		counter
		ret
		
		
; ////////////////////////////////////////////////////////////////////////////
; // If the 'temp' register equals 0, we will do a read operation, else we 
; // will write the configuration into the EEPROM.
; ////////////////////////////////////////////////////////////////////////////

eeprom_read_or_write_byte:
		tst		param
		brne	eeprom_write_byte_in_eeprom_and_increment_address_pointer
		rjmp 	eeprom_read_byte_in_sram_and_increment_address_pointer
		
		
; ////////////////////////////////////////////////////////////////////////////
; // Little helping routine for improved reading from EEPROM into SRAM.
; ////////////////////////////////////////////////////////////////////////////

eeprom_read_byte_in_sram_and_increment_address_pointer:
		out		EEARH, ZH
		out		EEARL, ZL
		sbi		EECR, EERE
		in		akku, EEDR
		st		X+, akku		
		adiw	ZH:ZL, 1
		ret		
		
		
; ////////////////////////////////////////////////////////////////////////////
; // Little helping routine for improved reading from SRAM into EEPROM.
; ////////////////////////////////////////////////////////////////////////////

eeprom_write_byte_in_eeprom_and_increment_address_pointer:
		sbic	EECR, EEWE
		rjmp	eeprom_write_byte_in_eeprom_and_increment_address_pointer
		
		out		EEARH, ZH
		out		EEARL, ZL
		
		ld		akku, X+
		out		EEDR, akku
		in		akku, SREG
		push 	akku
		
		sbi		EECR, EEMWE
		sbi		EECR, EEWE
		
		pop		akku
		out		SREG, akku
		adiw	ZH:ZL, 1
		ret
