;;=========================================================================
;; Put_u_base
;;=========================================================================

            global  _put_u_base

            extern  _itoa
            extern  _put_s

            section .text

;;=========================================================================
;; Writes unsigned int number in base representation to console
;; Entry: edi <- int number to print
;;         dl <- base (<=16)
;;         dh <- binary degree if base is binary / 0 if not
;; Destr: 
;;=========================================================================

_put_u_base:
            mov     rsi, Buffer
            call    _itoa

            inc     rsi
            mov     byte [rsi], 0

            mov     rdi, Buffer
            call    _put_s

            ret

;;-------------------------------------------------------------------------

            section .data

MAX_BUF_LEN equ     33

Buffer:     times   MAX_BUF_LEN db 0

;;=========================================================================











