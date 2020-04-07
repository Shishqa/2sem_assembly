
            global  _start
            extern  Sum

            section .text

_start:
            mov     rdi, 1
            mov     rsi, 2

            call    Sum

            mov     rax, 60
            xor     rdi, rdi
            syscall



