;;=========================================================================
;; Itoa implementation
;;=========================================================================

            default rel

            section .text

            global  _itoa

;;=========================================================================
;; Convert unsigned int number to string
;; Entry: edi <- unsigned int number
;;        rsi <- addr to store string
;;         dx <- dh:binary base if binary, 0 if not|dl:base (<=16) 
;; Destr: 
;;=========================================================================

_itoa:
            mov     r9, rsi

            mov     byte [BASE], dl
            cmp     dh, 0
            je      .process

            dec     dword [BASE]
            mov     byte [SHIFT], dh

.process_binary:
            xor     rdx, rdx
            mov     edx, edi
            and     edx, dword [BASE]

            add     rdx, DIGITS
            mov     al, byte [rdx]
            mov     byte [rsi], al
            inc     rsi

            xor     rcx, rcx
            mov     cl, byte [SHIFT]
.divide:    shr     edi, 1
            loop    .divide

            cmp     edi, 0
            jne     .process_binary
            jmp     .exit
.process:
            xor     rdx, rdx
            mov     eax, edi
            div     dword [BASE]

            mov     edi, eax

            add     rdx, DIGITS
            mov     al, byte [rdx]
            mov     byte [rsi], al
            inc     rsi

            cmp     edi, 0
            jne     .process
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
;;        rsi <- string end
;; Destr: rsi rdi
;;=========================================================================

reverse:
            push    rdi
            push    rsi
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
            pop     rsi
            pop     rdi
            ret

;;=========================================================================
















    
