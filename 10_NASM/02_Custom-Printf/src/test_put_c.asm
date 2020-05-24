;;=========================================================================
;; test_put_c.asm                                       Shishqa, MIPT 2020
;;=========================================================================

            default rel

            global  _start
            extern  _put_c
            extern  _flush_buffer

            section .text

;;=========================================================================

_start:
            mov     sil, 'A'
            call    _put_c

            mov     sil, 'B'
            call    _put_c

            mov     sil, '%'
            call    _put_c

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
check:      db 10, "check: AB%", 10
check_len   equ $ - check

;;=========================================================================

