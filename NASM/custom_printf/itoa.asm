;;=========================================================================
;; itoa.asm                                             Shishqa, MIPT 2020
;;=========================================================================

            default rel

            section .text

            global  _itoa

;;=========================================================================
;; Convert unsigned int number to string in base representation
;;
;; Danger! Writes from end to begin of the number
;; So, RDI should point to the place where the number
;; should end.
;;
;;                  v-RDI_entry
;; buffer: _1_2_3_4_5_
;;          ^-RDI_exit
;;
;; ENTRY: ESI <- unsigned int number
;;        RDI <- addr of string buffer end
;;         DH <- binary degree of base if binary, 0 if not
;;         DL <- base (<= 16)
;; Exit:  RDI -> addr of the last written digit
;; Destr: RAX RBX RDX RSI ; DF
;;=========================================================================

_itoa:
            lea     rbx, [DIGITS]               ; RBX points to DIGITS arr
            std                                 ; set dest. flag (R to L)

            mov     byte [BASE], dl             ; BASE = DL
            cmp     dh, 0                       ; !binary => ordinary_division
            je      .ordinary_division          ; binary  => shift_division

            dec     dword [BASE]                ; BASE = 11..11 (mask)
            mov     byte [SHIFT], dh            ; SHIFT = DH

.binary_division:

            xor     rax, rax                    ; RAX = 0
            mov     eax, esi
            and     eax, dword [BASE]           ; AL = ESI % BASE

            xlatb                               ; AL = [DIGITS + AL]
            stosb                               ; [RDI++] = AL

            mov     al, byte [SHIFT]            ; AL = SHIFT
.shr_loop:                                      ; loop: ESI <<= AL
            shr     esi, 1
            dec     rax
            jnz     .shr_loop                   ; while (SHIFT--) EDI >> 1;

            cmp     esi, 0
            jne     .binary_division            ; if done, exit
            jmp     .exit

.ordinary_division:
            xor     edx, edx                    ; 0:ESI / BASE = EAX * BASE + EDX
            mov     eax, esi
            div     dword [BASE]

            mov     esi, eax                    ; ESI <- QUOTIENT
            mov     al, dl                      ; AL <- REMAINDER

            xlatb                               ; AL = [DIGITS + AL]
            stosb                               ; [RDI++] = AL

            cmp     esi, 0
            jne     .ordinary_division          ; if done, exit
            jmp     .exit

.exit:
            inc     rdi                         ; [RDI] == first digit

            ret

;;-------------------------------------------------------------------------

            section .data
SHIFT:      db 0
BASE:       dd 0
DIGITS:     db "0123456789ABCDEF"

;;=========================================================================
