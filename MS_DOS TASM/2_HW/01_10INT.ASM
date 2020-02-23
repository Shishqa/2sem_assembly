.model tiny
.code

org 100h

Start:		mov ah, 0eh			; 10H 0EH int
		
		mov bx, offset MyString		; bx <- str start ptr

PrintSymbol:	cmp byte ptr [bx], 0d		; [ c ] =?= 0
		je Printed			; bx^

		mov al, [bx]			; al <- [ c ]
		int 10h				;	bx^

		inc bx				; [ c ][ d ]
						; bx-----^
		jmp PrintSymbol

Printed:	ret

MyString:	db 'Hello, Buddy!', 13, 10, 0

end		Start		