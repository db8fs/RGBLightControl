; ////////////////////////////////////////////////////////////////////////////
; //! @file 		colorspaces.asm
; //! @description	Subroutines for realizing color space transformations.
; //! @author		Falk Schilling (DB8FS) <falk.schilling@gmx.net>
; ////////////////////////////////////////////////////////////////////////////


; ----------------------------------------------------------------------------
; RGB-HSI transformation
; ----------------------------------------------------------------------------

	; ////////////////////////////////////////////////////////////////////////
	; //! This function transforms a LED_RGB tripel (consisting of 
	; //! LED_RED, LED_GREEN, LED_BLUE) into a tripel of LED_HUE, 
	; //! LED_SATURATION and LED_INTENSITY. The led group for which this
	; //! transformation will be performed is addressed by the led_counter
	; //! register. If the led_counter register is zero, we perform the 
	; //! conversion for all 4 led modules.
	; ////////////////////////////////////////////////////////////////////////
	
	colorspaces_rgb2hsi:
		ret
	
	
; ----------------------------------------------------------------------------
; HSI-RGB transformation
; ----------------------------------------------------------------------------
	
	; ////////////////////////////////////////////////////////////////////////
	; //! This function transforms a tupel of LED_HUE and LED_INTENSITY
	; //! into a LED_RGB tripel. The saturation will be hold constantly at 1
	; //! (full saturation). We perform this transformation for the HSI 
	; //! tripels of all LED modules. The transformation itself is only
	; //! approximated to avoid calculations with floating point and 
	; //! cosine operations. We also assume that the hue values in SRAM are
	; //! in the intervall [0, 180], which will be interpreted as angles of
	; //! the color cycle through the mapping f(x) = 2*x. Therefore we can
	; //! avoid using 16-bit arithmetics.
	; //! 
	; //! Examples: 
	; //!  - Value of 180 matches 360°
	; //!  - Value of 120 matches 240°
	; //!  - Value of 90  matches 180°
	; //!  - Value of 60  matches 120°
	; //!  - ...
	; ////////////////////////////////////////////////////////////////////////
	
	colorspaces_hsi2rgb:
		push 	akku
		in	 	akku, SREG
		push 	akku
		push 	counter
		push	hue
		push	intensity
		push	red
		push	green
		push	blue
		push	r0
		push	r1
		
		rcall	colorspaces_normalize_hue

		mov		led_counter, param		

		;ldi		led_counter, NUMBER_OF_LEDS
		
		;colorspaces_hsi2rgb_loop:
			;dec		led_counter
			clr		akku

			ldi		ZL, LOW(LED_HUE)
			ldi		ZH, HIGH(LED_HUE)
			add		ZL, led_counter
			adc		ZH, akku

			ld		hue, Z
				
			ldi		ZL, LOW(LED_INTENSITY)
			ldi		ZH, HIGH(LED_INTENSITY)
			add		ZL, led_counter
			adc		ZH, akku

			ld		intensity, Z

			mov		akku, intensity
		
			andi	akku, 0b10000000
			cpi		akku, 0x00
			breq	colorspaces_hsi2rgb_start_hue_calculation
			com		intensity		

			colorspaces_hsi2rgb_start_hue_calculation:
			clr		counter
			mov 	akku, hue
		
			; calculate h_i (hue section)
		
			colorspaces_hsi2rgb_hue_section_loop:
			cpi		akku, 30
			brlo	colorspaces_hsi2rgb_evaluate_hue_section
			inc		counter
			subi	akku, 30
			rjmp 	colorspaces_hsi2rgb_hue_section_loop
		
			; calculate p,q,r, save position of lookup-table in 'hue' register
			colorspaces_hsi2rgb_evaluate_hue_section:
			mov		hue, akku
			clr		hsi2rgb_p	; trivial case, because of full saturation
			clr		akku		

			; calculate position of lookup-table
			ldi		ZL, LOW(HSI2RGB_LOOKUPTABLE << 1)
			ldi		ZH, HIGH(HSI2RGB_LOOKUPTABLE << 1)
			add		ZL, hue
			adc		ZH, akku
		
			; load value f from LUT and calculate f*I
			lpm
			mov		akku, r0
			mul		akku, intensity
			lsl		r1
			mov		hsi2rgb_t, r1
			mov		hsi2rgb_q, intensity
			sub		hsi2rgb_q, hsi2rgb_t

			/*
			Evaluate (hue modulo 30°) in counter register 
			and set the appropriate (R,G,B)-tripel

			(R,G,B) = (I,t,p), if h_i == 0
					= (q,I,p), if h_i == 1
					= (p,I,t), if h_i == 2
					= (p,q,I), if h_i == 3
					= (t,p,I), if h_i == 4
					= (I,p,q), if h_i == 5
					= (I,t,p), if h_i == 6
			*/
		
			cpi		counter, 0x00
			breq	colorspace_hsi2rgb_sector_0
			cpi		counter, 0x01
			breq	colorspace_hsi2rgb_sector_1
			cpi		counter, 0x02
			breq	colorspace_hsi2rgb_sector_2
			cpi		counter, 0x03
			breq	colorspace_hsi2rgb_sector_3
			cpi		counter, 0x04
			breq	colorspace_hsi2rgb_sector_4
			cpi		counter, 0x05
			breq	colorspace_hsi2rgb_sector_5
			rjmp 	colorspace_hsi2rgb_sector_0
		
			colorspace_hsi2rgb_sector_0:
				mov		red, 	intensity
				mov		green, 	hsi2rgb_t
				mov		blue,	hsi2rgb_p
				rjmp	colorspace_hsi2rgb_fit_red_intervall
		
			colorspace_hsi2rgb_sector_1:
				mov		red, 	hsi2rgb_q
				mov		green,	intensity
				mov		blue, 	hsi2rgb_p
				rjmp	colorspace_hsi2rgb_fit_red_intervall
		
			colorspace_hsi2rgb_sector_2:
				mov		red,	hsi2rgb_p
				mov		green, 	intensity
				mov		blue,	hsi2rgb_t
				rjmp	colorspace_hsi2rgb_fit_red_intervall
		
			colorspace_hsi2rgb_sector_3:
				mov		red, 	hsi2rgb_p
				mov		green, 	hsi2rgb_q
				mov		blue,	intensity
				rjmp	colorspace_hsi2rgb_fit_red_intervall
		
			colorspace_hsi2rgb_sector_4:
				mov		red,	hsi2rgb_t
				mov		green,	hsi2rgb_p
				mov		blue,	intensity
				rjmp	colorspace_hsi2rgb_fit_red_intervall
		
			colorspace_hsi2rgb_sector_5:
				mov		red, 	intensity
				mov		green,	hsi2rgb_p
				mov		blue, 	hsi2rgb_q
				rjmp	colorspace_hsi2rgb_fit_red_intervall
		
			colorspace_hsi2rgb_fit_red_intervall:
				cpi		red, 128
				brlo	colorspace_hsi2rgb_fit_green_intervall
				ldi		red, 127
			
			colorspace_hsi2rgb_fit_green_intervall:
				cpi		green, 128
				brlo	colorspace_hsi2rgb_fit_blue_intervall
				ldi		green, 127
			
			colorspace_hsi2rgb_fit_blue_intervall:
				cpi		blue, 128
				brlo	colorspace_hsi2rgb_store_conversion
				ldi		blue, 127
			
			colorspace_hsi2rgb_store_conversion:
				clr		akku
				ldi		ZL, LOW(LED_RED)
				ldi		ZH, HIGH(LED_RED)
				add		ZL, led_counter
				adc		ZH, akku
				st		Z, red
			
				ldi		ZL, LOW(LED_GREEN)
				ldi		ZH, HIGH(LED_GREEN)
				add		ZL, led_counter
				adc		ZH, akku
				st		Z, green
			
				ldi		ZL, LOW(LED_BLUE)
				ldi		ZH, HIGH(LED_BLUE)
				add		ZL, led_counter
				adc		ZH, akku
				st		Z, blue
			
			;cpi		led_counter, 0x00
			;breq	colorspaces_hsi2rgb_return
			;rjmp 	colorspaces_hsi2rgb_loop
			
		colorspaces_hsi2rgb_return:
			pop		r1
			pop		r0
			pop		blue
			pop		green
			pop		red
			pop		intensity
			pop		hue
			pop		counter
			pop		akku
			out		SREG, akku
			pop 	akku
		ret


; ----------------------------------------------------------------------------
; Helper functions
; ----------------------------------------------------------------------------

	; ////////////////////////////////////////////////////////////////////////
	; //! This function calculates all minimums of LED_RED[i], LED_GREEN[i]
	; //! and LED_BLUE[i]. The led_counter register addresses the led module
	; //! for which the calculation will be performed (0 < led_counter <= 4)
	; ////////////////////////////////////////////////////////////////////////
	
	colorspaces_calc_rgb_minimum:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! This function calculates all minimums of LED_RED[i], LED_GREEN[i]
	; //! and LED_BLUE[i]. The led_counter register addresses the led module
	; //! for which the calculation will be performed (0 < led_counter <= 4)
	; ////////////////////////////////////////////////////////////////////////
	
	colorspaces_calc_rgb_maximum:
		ret
	
	
	; ////////////////////////////////////////////////////////////////////////
	; //! This function realizes the following C-operation for all LEDs to 
	; //! provide a nice way of cycling through the hue angles by only 
	; //! use of incrementing or decrementing the hue values in SRAM.
	; //! 
	; //! hue >= 180 ? hue = 0 : hue = hue % 180
	; ////////////////////////////////////////////////////////////////////////
	
	colorspaces_normalize_hue:
		push 	akku
		in 		akku, SREG
		push 	akku
		push 	r17
		
		ldi		ZL, LOW(LED_HUE)
		ldi		ZH, HIGH(LED_HUE)
		ldi		r17, NUMBER_OF_LEDS
		
		colorspaces_normalize_hue_loop:
			cpi		r17, 0x00
			breq	colorspaces_normalize_hue_return
			dec		r17
			
			ld 		akku, Z
			
			cpi		akku, 180
			brlo	colorspaces_normalize_hue_loop_next
			subi	akku, 180
			
			colorspaces_normalize_hue_loop_next:
			st		Z+, akku
			rjmp	colorspaces_normalize_hue_loop		
			
		colorspaces_normalize_hue_return:
		pop		r17
		pop		akku
		out 	SREG, akku
		pop		akku
		ret
		



//////////////////////////////////////////////////////////////////////////////
//! Some HSI2RGB calculations, based on:
//! http://de.wikipedia.org/wiki/HSV-Farbraum#Umrechnung_HSV_in_RGB 
//! (09.01.2011)
//////////////////////////////////////////////////////////////////////////////
/*
Suppose: constant saturation at S = 1 and
			0 <= H <= 180,
			0 <= I <= 1

---------------------------------------

-> h_i = floor(H/30°)

f = (H/30°- h_i)

p = I*(1-S) 		= 0
q = I*(1-S*f) 		= I*(1-f)
t = I*(1-S*(1-f)) 	= I*f

---------------------------------------

(R,G,B) = (I,t,p), if h_i == 0
		= (q,I,p), if h_i == 1
		= (p,I,t), if h_i == 2
		= (p,q,I), if h_i == 3
		= (t,p,I), if h_i == 4
		= (I,p,q), if h_i == 5
		= (I,t,p), if h_i == 6

----------------------------------------

Register:

p,q,t - 8-Bit values between 0 and 127

oder:

Integer:				Fixkomma:
127 -> 01111111			0.1111111 -> 0.9921875
63  -> 00111111			0.0111111 -> 0.4921875
31  -> 00011111			0.0011111 -> 0.2421875
15  -> 00001111			0.0001111 -> 0.1171875
7   -> 00000111			0.0000111 -> 0.0546875
4 	-> 00000100			0.0000100 -> 0.0312500
3	-> 00000011			0.0000011 -> 0.0234375
1	-> 00000001			0.0000001 -> 0.0078125


Bestmöglicher Ansatz:

1. Hue in Register klatschen
2. gucken ob größer 30°
3. wenn ja, Zähler erhöhen, 30 abziehen
4. Schritt 3 wiederholen bis hue kleiner 30
5. h_i entspricht Zählerregister -> somit Fallunterscheidung möglich
6. Ermitteln von p,q,t:
	6.1.	p = 0
	6.2 	t = I*f 	-> wissen: f ist Nachkommaanteil von H/30°, dabei Werte zwischen [0/30, 29/30]
						-> zudem: 0 <= I <= 127, in Fixkommanotation (1.7) somit proportional zu Kommazahl zwischen 0 und ~1 (siehe Tabelle)
						-> somit Notwendigkeit Nachkommaanteil zwischen 0 und ~1 (bzw. 0 und 127) umzurechnen
						-> danach Multiplikation mit Intensität möglich, Nachkommaanteil ist Wert von t
						-> möglicher Ansatz: - Wertetabelle für Nachkommaanteile (f) einführen (lässt sich für q gleich mitnutzen)
	6.3		q = I*(1-f) = I - I*f
*/


