
locals @@

.186
.model tiny

.code

public HookInt
public OldIntAddr

;==========================================================================
; System interrupt hooker
; Entry: BX - number of interrupt
;	 CS:SI - address of new interrupt handler
; Destr: AX BX DI
;==========================================================================

HookInt		proc

		shl	bx, 1
		mov	di, bx
		shl 	bx, 1			; bx *= 2

		xor 	ax, ax
		mov 	es, ax			; es = 0

		cli				; lock interrupts

		mov 	ax, word ptr es:[bx]	; saving old interrupt 
		mov	cs: OldIntAddr + di, ax ; handler address
		mov	ax, word ptr es:[bx+2]
		mov	cs: OldIntAddr + di + 2, ax

		mov 	word ptr es:[bx],   si	; setting new interrupt
		mov 	word ptr es:[bx+2], cs	; handler address

		sti				; unlock interrupts

		ret
		endp

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OldIntAddr	dw 128 dup(0, 0)		; int_vector

;==========================================================================		

end
