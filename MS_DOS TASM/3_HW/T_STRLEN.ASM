locals @@

.186
.model tiny

.code

extrn strlen: proc

org 100h
Start:		push ds
		pop es
		mov di, offset TestString

		call strlen

		mov ax, 4c00h
		int 21h

.data

TestString	db "it is 24 symbols lenght!", 0d

end		Start