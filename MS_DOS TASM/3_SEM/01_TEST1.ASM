.186
locals @@

.model tiny
.code

extrn	DrawLine: proc

org 100h

Start:		mov ax, (4eh shl 8) or '='
		mov di, 80d * 5d + 5d
		mov cx, 40d
		call DrawLine

		mov ax, (4eh shl 8) or '+'
		mov di, 80d * 7d + 5d
		mov cx, 45d
		call DrawLine

		mov ax, 4c00h
		int 21h

end		Start