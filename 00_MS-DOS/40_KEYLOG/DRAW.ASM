locals @@

.186

.model tiny
.code

public SetVideo
public DrawHorLine
public DrawVerLine

;==========================================================================
; Sets ES pointer to the videosegment
; Exit: ES <- VRAM segment
;==========================================================================

SetVideo		proc

			push 	0b800h
			pop 	es

			ret
			endp

;==========================================================================
; Draws a horizontal line on the screen (make sure videosegment is set)
; Entry: AL - char to draw a line (ASCII code)
;	 AH - color attribute
;	 DI - char address of start
;	 CX - line length
; Destr: CX DI DF
;==========================================================================

DrawHorLine		proc

			cld

			rep 	stosw
			
			ret
			endp

;==========================================================================
; Draws vertical line on the screen (make sure videosegment is set)
; Entry: AL - char to draw a line (ASCII code)
;	 AH - color attribute
;	 DI - char address to start
;	 CX - line length
; Destr: CX DI
;==========================================================================

DrawVerLine		proc
			
@@Next:			stosw
			add 	di, 158d
			loop 	@@Next
			
			ret
			endp

;==========================================================================

end


			












