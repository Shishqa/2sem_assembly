
.model tiny
.code

org 100h
Start:		xor ax, ax
		mov es, ax		; es = 0

		cli
;		mov bx, 9 * 4		; es:bx -> int 09h in ISR table
		mov bx, 8 * 4

		mov ax, word ptr es:[bx]
		mov Old09, ax
		mov ax, word ptr es:[bx+2]
		mov Old09+2, ax

		mov word ptr es:[bx],   offset New09
		mov ax, cs
		mov word ptr es:[bx+2], cs 
		sti

		mov ax, 3100h
		mov dx, offset EndLabel
		shr dx, 4
		inc dx
		int 21h

;==========================================================================
New09		proc

		push ax di es

		mov ax, 0b800h
		mov es, ax
		;in al, 60h
		mov al, dl			;;;
		mov di, (10 * 80 + 40) * 2
		mov ah, 4eh
		stosw

	;	in al, 61h		; контроллер клавиатуры
	;	mov ah, al
	;	or al, 80h		; моргаем старшим битом
	;	out 61h, al
	;	mov al, ah
	;	out 61h, al

	;	mov al, 20h		; контроллер прерываний
	;	out 20h, al

		pop es di ax

		db 0eah
Old09		dw 0
		dw 0

		iret			; interrupt return 
		endp
;==========================================================================

EndLabel:
end 		Start