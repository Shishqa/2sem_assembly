locals @@

.model tiny
.code

extrn	memchr: proc
extrn	memcmp: proc
extrn	memset: proc
extrn   memcpy: proc

public  strlen
public  strchr
public  strcpy
public  strcmp

;==========================================================================
; Gets length of null-terminated string
; Entry: ES:DI - absolute address of string
; Exit:  CX <- length of string
; Destr: AL DI
;==========================================================================

strlen		proc

		mov al, 0d
		mov cx, 0ffffh
		call memchr

		mov di, cx
		mov cx, 0ffffh
		sub cx, di
		dec cx

		ret
		endp

;==========================================================================
; Looks within bytes of null-terminated string for the first occurence
; of value
; Entry: AL - value
;	 ES:DI - absolute address of string
; Exit:  ES:DI <- addr next to the first occurence of value
;	 ZF <- 1 if value is found 
;	       0 if value is not found
; Destr: CX 
;==========================================================================

strchr		proc
		
		push di
		call strlen
		pop di

		call memchr

		ret
		endp

;==========================================================================
; Compares two null-terminated strings
; Entry: ES:DI - absolute addr of the str1
;	 DS:SI - absolute addr of the str2
; Exit:  ZF <- 0 if str1 != str2
;	       1 if str1  = str2
;        CF <- 0 if str1  > str2
;	       1 if str1  < str2
; Destr: DI SI
;==========================================================================		

strcmp		proc

		push di
		call strlen
		pop di

		inc cx
		call memcmp		
		
		endp

;==========================================================================
; Copies null-terminated string to the destination
; Entry: ES:DI - destination absolute address
;	 DS:SI - source absolute address
; Destr: CX DI SI 
;==========================================================================

strcpy		proc

		push di
		call strlen
		pop di

		call memcpy		

		ret
		endp

;==========================================================================

end		