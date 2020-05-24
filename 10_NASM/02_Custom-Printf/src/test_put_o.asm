;;=========================================================================
;; test_put_o.asm                                       Shishqa, MIPT 2020
;;=========================================================================

            default rel

            global  _start
            extern  _put_o
            extern  _flush_buffer

            section .text

;;=========================================================================

_start:
            mov     esi, 1333
            call    _put_o

            call    _flush_buffer

            mov     rax, 1
            mov     rdi, 1
            mov     rsi, check
            mov     rdx, check_len
            syscall

            mov     rax, 60
            xor     rdi, rdi
            syscall

;;=========================================================================

            section .data
check:      db 10, "check: 2465", 10
check_len   equ $ - check

;;=========================================================================
