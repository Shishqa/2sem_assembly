
        global  _start

        section .text

_start:

        jmp       .label
        mov       r8, 0x3C

.label:
        mov     rax, 0x3C
        xor     rdi, rdi
        syscall
