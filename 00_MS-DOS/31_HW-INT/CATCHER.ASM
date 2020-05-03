
locals @@

.186
.model tiny

.code

extrn	DrawWindow: proc
extrn 	CodeEnd

org 100h
Start:		mov bx, 09h
		mov si, offset Catch09int
		call CatchInt 

;==========================================================================
; Interrupt catcher
; Entry: BX - interrupt number
;	 CS:SI - address of new interrupt handler
; Destr: AX ES
;==========================================================================

CatchInt	proc

		shl bx, 2

		xor ax, ax
		mov es, ax

		cli
		;mov ax, word ptr es:[bx]
		;mov OldInt, ax
		;mov ax, word ptr es:[bx+2]
		;mov OldInt+2, ax 

		mov word ptr es:[bx],   si
		mov word ptr es:[bx+2], cs
		sti

		mov ax, 3100h			; int21H - 31 (resident)
		mov dx, offset CodeEnd		
		shr dx, 4
		inc dx				
		int 21h				; Staying resident

		endp

;==========================================================================
; Converts al 0-3 bits to 1-9A-F hex format
; Entry: AL 
; Exit:  AL <- correct ascii code
;==========================================================================

toHex		proc

		cmp al, 0ah
		jb @@digit
		
@@letter:	add al, 55d
		jmp @@exit

@@digit:	add al, 48d

@@exit:		ret
		endp

;==========================================================================
; Int09h catcher
;==========================================================================

Catch09int	proc

		push ax bx cx dx di es

		mov ax, 0b800h
		mov es, ax

		in al, 60h

		mov cx, 15d
		mov dx, 5d
		mov di, (80 * 2 + 50) * 2
		call DrawWindow

		mov bl, al
		shr al, 4
		and al, 0fh
		call toHex

		mov di, (80 * 4 + 57) * 2
		mov ah, 4eh
		stosw

		mov al, bl
		and al, 0fh
		call toHex
		stosw

		in al, 61h
		mov ah, al
		or al, 80h
		out 61h, al
		mov al, ah
		out 61h, al

		mov al, 20h
		out 20h, al

		pop es di dx cx bx ax
		
		iret
		endp

;==========================================================================

end		Start		