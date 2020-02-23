locals @@

.186

.model tiny
.code

public DrawWindow

extrn SetVideo: proc
extrn DrawHorLine: proc
extrn DrawVerLine: proc

;==========================================================================
; Draws a window on the screen
; Entry: DI - start addr
;	 CX - width
;	 DX - height
; Destr: AX BX CX DX
;==========================================================================

DrawWindow		proc

CORNER_COLOR_ATTR equ 3bh
INSIDE_COLOR_ATTR equ 70h
SHADOW_COLOR_ATTR equ 14h

LEFT_UP_CORNER    equ 0c9h
RIGHT_UP_CORNER   equ 0bbh
LEFT_DOWN_CORNER  equ 0c8h
RIGHT_DOWN_CORNER equ 0bch
HOR_BORDER	  equ 0cdh
VER_BORDER	  equ 0bah
INSIDE		  equ ' '
SHADOW		  equ 0b1h

			call SetVideo

UPPER_BORDER:		mov ax, (CORNER_COLOR_ATTR shl 8) or LEFT_UP_CORNER
			stosw

			push cx
			mov al, HOR_BORDER
			sub cx, 2
			call DrawHorLine
			pop cx

			mov al, RIGHT_UP_CORNER
			stosw

IN_WINDOW:		xchg cx, dx
			sub dx, 2d
			sub cx, 2d			

			add di, 156d
			sub di, dx
			sub di, dx

@@Next:			mov bx, cx

			mov ax, (CORNER_COLOR_ATTR shl 8) or VER_BORDER
			stosw

			mov ax, (INSIDE_COLOR_ATTR shl 8) or INSIDE
			mov cx, dx
			call DrawHorLine

			mov ax, (CORNER_COLOR_ATTR shl 8) or VER_BORDER
			stosw

			mov ax, (SHADOW_COLOR_ATTR shl 8) or SHADOW
			stosw

			add di, 154d
			sub di, dx
			sub di, dx

			mov cx, bx
			loop @@Next

LOWER_BORDER:		mov ax, (CORNER_COLOR_ATTR shl 8) or LEFT_DOWN_CORNER
			stosw

			mov al, HOR_BORDER
			mov cx, dx
			call DrawHorLine

			mov al, RIGHT_DOWN_CORNER
			stosw

			mov ax, (SHADOW_COLOR_ATTR shl 8) or SHADOW
			stosw

			add di, 156d
			sub di, dx
			sub di, dx

			add dx, 2d
			mov cx, dx
			call DrawHorLine

			ret
			endp
;==========================================================================

end			