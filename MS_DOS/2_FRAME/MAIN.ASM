locals @@

.186

.model tiny
.code

extrn DrawWindow: proc

org 100h
Start:		call Clear

		mov cx, 15d
		mov dx, 3d
		mov bx, 3d

@@next:		xchg bx, cx

		push cx
		call Delay
		pop cx

		push cx dx bx
		mov di, (80 * 3 + 15) * 2
		call DrawWindow
		pop bx dx cx
		add cx, 3
		inc dx

		xchg bx, cx
		loop @@next

		mov di, (80 * 10 + 27) * 2
		mov si, offset Message
		mov cx, 20d

		rep movsw
	
		mov ah, 00h
		int 16h

		call Clear

		mov ax, 4c00h
		int 21h

;==========================================================================
; Waits some time
; Destr: CX
;==========================================================================

Delay		proc

		mov CX, 0ffffh
@@next:		loop @@next
		
		ret
		endp

;==========================================================================
; Clears the screen
; Destr: AX CX DI ES
;==========================================================================

Clear		proc
		
		push 0b800h
		pop es
		mov di, 0d

		xor ax, ax

		mov cx, 80 * 25 * 2

		rep stosw
		
		ret
		endp

;==========================================================================

.data

Message		db 'A.s.s.e.m.b.l.e.r. .i.s. .a.m.a.z.i.n.g.', 0d

end		Start
