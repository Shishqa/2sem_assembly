.model tiny
.code

org 100h

Start:		mov ax, 0b800h
		mov es, ax

		mov bx, offset MyString
		mov cx, (80d * 9d + 30d) * 2d

PrintString:	cmp byte ptr [bx], 0d
		je Printed

		mov dl, [bx]
		mov dh, 14h

		inc bx
		push bx

		mov bx, cx

		mov es:[bx], dx

		add bx, 2		
		mov cx, bx

		pop bx

		jmp PrintString

Printed:	ret

MyString:	db 'Im hacker!', 0

end		Start