;;=========================================================================
;; test_put_s.asm                                       Shishqa, MIPT 2020
;;=========================================================================

            default rel

            global  _start
            extern  _put_s
            extern  _flush_buffer

            section .text

;;=========================================================================

_start:
            mov     rsi, Msg
            call    _put_s
            call    _flush_buffer

            mov     rax, 1
            mov     rdi, 1
            mov     rsi, check
            mov     rdx, check_len
            syscall

            mov     rax, 60
            xor     rdi, rdi
            syscall

;;-------------------------------------------------------------------------

            section .data

Msg:        db "Hello, world!", 10, 0

check:      db 10, "check: Hello, world!", 10, 10
check_len   equ $ - check

;;=========================================================================


