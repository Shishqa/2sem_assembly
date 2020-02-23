.186
locals @@

.model tiny

.code
public DrawLine

;==========================================================================
; Draws a horizontal line on the screen 
; Entry: AL - char to draw a line (ASCII)
;	 AH - color attr
;	 DI - char place to start
;	 CX - line length
; Destr: CX DI
;==========================================================================

DrawLine	proc			; начало процедуры

		push 0b800h
		pop es

		shl di, 1
		cld			; will ++ di
		
		rep stosw		; while (cx--) es:[di++] := ax

		ret
		endp			; конец процедуры

;==========================================================================

end					