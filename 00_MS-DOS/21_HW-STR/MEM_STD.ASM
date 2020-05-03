locals @@

.model tiny
.code

public memchr
public memcmp
public memset
public memcpy

;==========================================================================
; Looks within first n-bytes of memory block for the first occurence
; of value
; Entry: AL - value
;	 ES:DI - absolute address of block
;	 CX - num of bytes to be analyzed
; Exit:  ES:DI <- addr next to the first occurence of value
;	 ZF <- 1 if value is found 
;	       0 if value is not found 
; Destr: CX 
;=========================================================================

memchr		proc
	
		cld
		repne scasb	; while (ES:[DI++] != AL && CX) CX--;

		dec di

		ret
		endp

;==========================================================================
; Compares two blocks of memory of equal size
; Entry: CX - size of blocks
;	 ES:DI - absolute addr of the first block
;	 DS:SI - absolute addr of the second block
; Exit:  ZF <- 1 if blocks are equal
;	       0 if blocks are not equal
;	 CF <- 0 if first byte that doesn't match is greater in block1
;	       1 if first byte that doesn't match is greater in block2
; Destr: CX DI SI
;==========================================================================		

memcmp		proc

		cld
		repe cmpsb	; while (ES:[DI++] == DS:[SI++]) CX++

		ret
		endp

;===========================================================================
; Sets the first n bytes of memory block to the specific value
; Entry: AL - value
;	 ES:DI - absolute address of block
;	 CX - num of bytes
; Destr: DI CX	 
;===========================================================================

memset		proc

		cld
		rep stosb	; while (CX--) ES:[DI++] = AH

		ret
		endp

;==========================================================================
; Copies n bytes from one block of memory to another
; Entry: ES:DI - destination absolute address
;	 DS:SI - source absolute address
;	 CX - num of bytes
; Destr: CX DI SI
;==========================================================================

memcpy		proc

		cld
		rep movsb	; while (CX--) ES:[DI++] = DS:[SI++]

		ret
		endp

;==========================================================================

end		