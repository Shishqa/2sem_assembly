;;=========================================================================
;; Test put_s
;;=========================================================================

            default rel

            global  _start
            extern  _put_s

            section .text

;;=========================================================================

_start:
            mov     rdi, Msg
            call    _put_s

            mov     rax, 60
            xor     rdi, rdi
            syscall

;;-------------------------------------------------------------------------

            section .data

Msg:        db      "Hello, world!", 10, 0

;;=========================================================================


