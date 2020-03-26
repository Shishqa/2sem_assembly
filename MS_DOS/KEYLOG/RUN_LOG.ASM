
locals @@

.186
.model tiny

.code

extrn HookInt: proc
extrn Logger:  proc
extrn Printer: proc

extrn CodeEnd

org 100h
Start:		mov 	bx, 09h			
		mov 	si, offset Logger

		call 	HookInt			; hooking interrupt 09h

		mov 	bx, 28h
		mov	si, offset Printer

		call    HookInt

		mov 	ax, 3100h
		mov 	dx, offset CodeEnd	; DX <- size of code
		shr 	dx, 4			; which will stay resident
		inc 	dx
		int 	21h			; staying resident

end		Start 




		
