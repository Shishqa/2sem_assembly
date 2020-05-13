
        global  _start

        section .text

_start:

        push    123
        pop     rcx

        mov     rdi, rsp
        sub     rsp, 11

        mov     ebx, 10
        std
        mov     byte [rdi], 0
        dec     rdi

.convert_loop:

        xor     edx, edx
        mov     eax, ecx
        div     ebx

        mov     ecx, eax
        mov     al, dl
        add     al, '0'

        stosb

        cmp     ecx, 0
        jnz     .convert_loop 

        mov     rax, 1
        mov     rsi, rdi
        mov     rdi, 1
        mov     rdx, 10
        syscall

        add     rsp, 11

        mov     rax, 0x3C
        xor     rdi, rdi
        syscall
