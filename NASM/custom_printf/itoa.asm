;;=========================================================================
;; itoa.asm                                             Shishqa, MIPT 2020
;;=========================================================================

            default rel

            section .text

            global  _itoa

;;=========================================================================
;; Convert unsigned int number to string
;; Entry: edi <- unsigned int number
;;        rsi <- addr of string buffer
;;         dh <- binary degree of base if binary, 0 if not
;;         dl <- base (<=16)
;; Exit:  rsi -> addr of the last written symbol
;; Destr: rax rdx rdi r9
;;=========================================================================

_itoa:
            mov     r9, rsi

            mov     byte [BASE], dl             ; BASE = DL
            cmp     dh, 0                       ; !binary => ordinary_division
            je      .ordinary_division          ; binary  => shift_division

            dec     dword [BASE]                ; BASE = 11..11 (mask)
            mov     byte [SHIFT], dh            ; SHIFT = DH

.binary_division:
            xor     rdx, rdx
            mov     edx, edi
            and     edx, dword [BASE]           ; EDX = EDI % BASE

            add     rdx, DIGITS                 ; RDX -> correct remainder digit
            mov     al, byte [rdx]
            mov     byte [rsi], al              ; correct digit -> output buffer
            inc     rsi                         ; ++buf_ptr

            xor     rdx, rdx
            mov     dl, byte [SHIFT]
.divide:    shr     edi, 1
            dec     rdx
            jnz     .divide

            cmp     edi, 0
            jne     .binary_division
            jmp     .exit

.ordinary_division:
            xor     rdx, rdx
            mov     eax, edi
            div     dword [BASE]

            mov     edi, eax

            add     rdx, DIGITS
            mov     al, byte [rdx]
            mov     byte [rsi], al
            inc     rsi

            cmp     edi, 0
            jne     .ordinary_division
            jmp     .exit

.exit:
            mov     rdi, r9
            dec     rsi

            call    reverse

            ret

;;-------------------------------------------------------------------------

            section .data
SHIFT:      db 0
BASE:       dd 0
DIGITS:     db "0123456789ABCDEF"

;;=========================================================================
;; Reverse string
;; Entry: rdi <- string begin
;;        rsi <- string end   (pointer to the last symbol)
;; Destr: rax rdx rdi
;;=========================================================================

reverse:
            mov     rdx, rsi
.loop:
            cmp     rdi, rsi
            jae     .exit

            mov     al, byte [rdi]
            xchg    al, byte [rsi]
            mov     byte [rdi], al

            inc     rdi
            dec     rsi

            jmp     .loop

.exit:
            mov     rsi, rdx
            ret

;;=========================================================================
















    
