
        global _start

        section .text

_start: 
        push    rax
        pop     rbx

        mov     rax, 0x3C
        xor     rdi, rdi
        syscall


