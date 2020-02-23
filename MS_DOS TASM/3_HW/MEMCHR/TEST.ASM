
.model tiny
.code

extrn memchr: proc

org 100h
Start:			mov di, offset String
			push ds
			pop es
			mov al, '2'
			mov cx, 4d
			cld

			call memchr
			jnz symbol_not_found

symbol_found:		mov byte ptr es:[di-1], '@'

			mov ah, 09h
			mov dx, offset OK_msg
			int 21h

			mov dx, offset String
			int 21h

			jmp return

symbol_not_found:	mov ah, 09h
			mov dx, offset NOK_msg
			int 21h 

return:			mov ax, 4c00h
			int 21h

;--------------------------------------------------------------------------
.data
OK_msg		db "Symbol found and replaced to '@':", 13d, 10d, '$'
NOK_msg		db "Symbol not found", 13d, 10d, '$'

String		db "Hello * Goodbye = 42", '$'
;--------------------------------------------------------------------------

end			Start


 