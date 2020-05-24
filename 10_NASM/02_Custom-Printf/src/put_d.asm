;;=========================================================================
;; put_d.asm                                            Shishqa, MIPT 2020
;;=========================================================================

            global  _put_d

            extern  _put_u_base
            extern  _put_c

            section .text

;;=========================================================================
;; Writes signed int in decimal representation to the console
;;
;; ENTRY: ESI <- int number to print
;; DESTR: RAX RBX RDX RSI RDI ; CC
;;=========================================================================

_put_d:
            mov     edx, 0x80000000         ; EDX = highest bit mask

            and     edx, esi
            cmp     edx, 0                  ; if the highest bit is 0,
            je      .positive               ; the number is positive

.negative:                                  ; else, the number is negative

            push    rsi                     ; save RSI

            mov     sil, '-'
            call    _put_c                  ; print '-'

            pop     rsi                     ; restore RSI
            neg     esi                     ; ESI = -ESI

.positive:

            mov     dx, (0 << 8) | 10       ; DL = 10, DH = 0
            call    _put_u_base

            ret

;;=========================================================================










