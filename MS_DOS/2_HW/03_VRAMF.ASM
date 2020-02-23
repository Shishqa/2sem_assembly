.model tiny

videosegment = 0b800h

.code

org 100h

Start:		mov ax, videosegment		
		mov es, ax			; set segment to videoRAM

		mov ax, 0			
		mov dh, 32d
		mov dl, 70h			; black on white

DrawBG:		call DrawHorLine		; DrawHorLine(ax, ' ', white)
		inc ax
		cmp ax, 25d
		jb DrawBG

		mov dl, 07h			; white on black

		mov dh, 32d			; ' ' 

		mov ax, 1d			
		call DrawVerLine
		mov ax, 78d
		call DrawVerLine

		mov dh, 0cdh

		mov ax, 0d
		call DrawHorLine
		mov ax, 24d
		call DrawHorLine

		mov dh, 0bah

		mov ax, 0d
		call DrawVerLine
		mov ax, 79d
		call DrawVerLine

		mov bx, 0d
		mov byte ptr es:[bx], 0c9h	; upper-left corner
		mov bx, 158d
		mov byte ptr es:[bx], 0bbh	; upper-right corner
		mov bx, 24d * 160d
		mov byte ptr es:[bx], 0c8h	; lower-left corner
		mov bx, 24d * 160d + 158d
		mov byte ptr es:[bx], 0bch	; lower-right corner

		mov dx, (12d * 80d + 30d) * 2d
		mov bx, offset Message

PrintMessage:	cmp byte ptr [bx], 0d
		je Delay

		mov cl, [bx]			; char -> |cl|
		mov ax, bx
		mov bx, dx
		
		mov es:[bx], cl			; |cl| -> VRAM
		
		mov dx, bx
		mov bx, ax

		inc bx				; next symbol
		add dx, 2d			; next VRAM cell
		jmp PrintMessage

Delay:		mov ah, 00h
		int 16h				; wait keyboard action

		mov ax, 4c00h			; return 0
		int 21h

;-----------------------------------------------------------------		
;	Func draws horizontal line on the screen
;	|ax| <- distance from the top of the screen (0-24)
;	|dh| <- ascii code of symbol
;	|dl| <- bg color
;-----------------------------------------------------------------
DrawHorLine:	push ax

		mov bx, ax
		shl bx, 5d		; bx = 32 ax + 128 ax = 160 ax
		shl ax, 7d
		add bx, ax		

		mov ax, 00h		; now ax is the width counter

H_DrawSymbol:	mov es:[bx], dh		; symbol -> vram
		inc bx

		mov es:[bx], dl		; setting bg color
		inc bx 

		inc ax
		cmp ax, 80d
		jne H_DrawSymbol

		pop ax
		ret
;-----------------------------------------------------------------
;#################################################################
;-----------------------------------------------------------------
;	Function draws vertical line on the screen
;	|ax| <- distance from the left corner of the screen (0-79)
;	|dh| <- ascii code of symbol
;	|dl| <- bg color
;-----------------------------------------------------------------
DrawVerLine:	push ax

		mov bx, ax
		shl bx, 1d

		mov ax, 00h

V_DrawSymbol:	mov es:[bx], dh
		inc bx

		mov es:[bx], dl
		add bx, 159d

		inc ax
		cmp ax, 25d
		jne V_DrawSymbol

		pop ax
		ret
;-----------------------------------------------------------------

.data

Message: 	db 'Asm is very interesting!', 0d

end		Start
		