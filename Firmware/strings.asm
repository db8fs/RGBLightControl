; ////////////////////////////////////////////////////////////////////////////
; //! @file 		strings.asm
; //! @description	String constants for UART communication.
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////
	
	
; ----------------------------------------------------------------------------
; String constants
; ----------------------------------------------------------------------------

; ////////////////////////////////////////////////////////////////////////////
; // Message string definition
; ////////////////////////////////////////////////////////////////////////////		

	UART_MESSAGE_GREETING:				.DB		10,13,"RGB LED Light Control, (c) 2011 by DB8FS",0,0
	UART_MESSAGE_COMMAND_RECEIVED:		.DB		"Command received!",0
	UART_MESSAGE_NEWLINE:				.DB		10,13,0,0
