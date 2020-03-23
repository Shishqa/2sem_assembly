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

            mov     rax, 60
            xor     rdi, rdi
            syscall

;;=========================================================================



