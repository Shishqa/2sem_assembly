
locals @@

.186
.model tiny

.code

org 100h
Start:		xor ax, ax
		mov es, ax

		mov bx, 9 * 4

		cli
		mov Old09, word ptr es:[bx]
		mov Old09+2, word ptr es:[bx+2]

		mov word ptr es:[bx], offset New09
		