;;=========================================================================
;; test_put_u.asm                                       Shishqa, MIPT 2020
;;=========================================================================

            default rel

            global  _start
            extern  _put_u
            extern  _flush_buffer

            section .text

;;=========================================================================

_start:
            mov     esi, 8123
            call    _put_u

            mov     esi, 0
            call    _put_u

            mov     esi, 99999
            call    _put_u

            call    _flush_buffer

            mov     rax, 60
            xor     rdi, rdi
            syscall

;;=========================================================================

