;;=========================================================================
;; test_put_b.asm                                       Shishqa, MIPT 2020
;;=========================================================================

            default rel

            global  _start
            extern  _put_b
            extern  _flush_buffer

            section .text

;;=========================================================================

_start:
            mov     esi, 0x45
            call    _put_b
            call    _flush_buffer

            mov     rax, 1
            mov     rdi, 1
            mov     rsi, check
            mov     rdx, check_len
            syscall

            mov     rax, 0x3C
            xor     rdi, rdi
            syscall

;;=========================================================================

            section .data
check:      db 10, "check: 1000101", 10
check_len   equ $ - check

;;=========================================================================
