.model tiny
.code
org 100h

Start:			mov ah, 09h				; puts(dx)
			mov dx, offset HelloMsg
			int 21h

			mov ax, 4C00h				; return(0)
			int 21h
			
HelloMsg:		db 'Hello, World!' , 13 , 10 , '$'	; "Hello, World!\r\n"

end			Start
			