
        global  _start

        section .text

_start:
        call    .test_in

        mov     rsi, rdi

        call    .test_out

        mov     rax, 60
        xor     rdi, rdi
        syscall

.test_in:
        sub     rsp, 11

        xor     rax, rax
        mov     rsi, rsp
        xor     rdi, rdi
        mov     rdx, 10
        push    r11
        syscall
        pop     r11

        cld
        xor     rdi, rdi 
        xor     rax, rax

.convert:

        lodsb

        sub     al, '0'
        cmp     al, 9
        ja      .end

        imul    rdi, rdi, 10
        add     rdi, rax

        jmp     .convert

.end:
        add     rsp, 11
        ret

;==================================================

.test_out:

        sub     rsp, 11
    
        mov     rdi, rsp
        add     rdi, 10

        std

        mov     rbx, 10
        xor     rcx, rcx

.convert_loop:
        xor     edx, edx
        mov     eax, esi
        div     ebx

        mov     esi, eax
        mov     al, dl
        add     al, '0'

        stosb
        inc     rcx

        cmp     esi, 0
        jne     .convert_loop

        inc     rdi

        mov     rax, 1
        mov     rsi, rdi
        mov     rdi, 1
        mov     rdx, rcx
        push    r11
        syscall
        pop     r11

        add     rsp, 11

        ret
