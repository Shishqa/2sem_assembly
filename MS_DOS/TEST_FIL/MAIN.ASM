
locals @@

.186
.model tiny

.code

org 100h
Start:		mov ax, 3d01h
		mov dx, offset FileSpec

		int 21h

		mov bx, ax
		mov ah, 40h
		mov cx, 5d
		mov dx, offset TestStr

		int 21h

		mov ah, 3eh
	
		int 21h

		ret

FileSpec	db "W:\TEST_FIL\test.txt", 0
TestStr		db "hello"

end		Start