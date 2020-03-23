;;=========================================================================
;; test_put_d.asm                                       Shishqa, MIPT 2020
;;=========================================================================

            default rel

            global  _start
            extern  _put_d
            extern  _flush_buffer

            section .text

;;=========================================================================

_start:
            mov     esi, -1234
            call    _put_d

            mov     esi, 8
            call    _put_d

            call    _flush_buffer

            mov     rax, 60
            xor     rdi, rdi
            syscall

;;=========================================================================

