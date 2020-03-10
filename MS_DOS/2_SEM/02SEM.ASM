
.model tiny
.code

org 100h

Start:		mov ax, 0b800h
		mov es, ax

		mov bx, (80d * 12d + 40d) * 2 	; b - base, только по bx можно [bx]
		mov byte ptr es:[bx], 65d	; 65 == 'A'

		inc bx

		mov byte ptr es:[bx], 4eh or 80h	; color in the nex byte
		
		ret

end		Start