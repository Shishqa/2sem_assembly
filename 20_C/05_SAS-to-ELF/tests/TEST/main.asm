
        global  _start

        section .text

_start:

        imul     rcx

        mov     rax, 0x3C
        xor     rdi, rdi
        syscall
