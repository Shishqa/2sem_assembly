;;=========================================================================
;; printf.asm                                           Shishqa, MIPT 2020
;;=========================================================================

            global  printf

            extern  _put_s
            extern  _put_c
            extern  _put_u
            extern  _put_b
            extern  _put_o
            extern  _put_x
            extern  _put_d

            section .text

;;=========================================================================
;; vanilla printf
;; Entry: rdi <- format str address (null terminated)
;;        stack <- args
;; Destr: rax rbx rdx
;;=========================================================================

printf:
            push    rbp
            mov     rbp, rsp

            mov     qword [rbp-5*8], rsi
            mov     qword [rbp-4*8], rdx
            mov     qword [rbp-3*8], rcx
            mov     qword [rbp-2*8], r8
            mov     qword [rbp-1*8], r9

            xor     r8, r8

            sub     rbp, 5*8
            sub     rsp, 5*8

.printf_loop:
            mov     rax, rdi
            call    next_flag

            xor     bl, bl
            xchg    byte [rdi], bl

            mov     rdi, rax
            call    _put_s

            xchg    byte [rdi], bl

            cmp     byte [rdi], 0
            je      .exit

            inc     rdi
            call    parse_flag

            cmp     byte [rdi], SHIELD_FLAG
            jne     .next_arg

            inc     rdi
            jmp     .printf_loop

.next_arg:
            inc     rdi
            add     rbp, 8
            inc     r8
            cmp     r8, 5
            jne     .printf_loop

            add     rbp, 16
            jmp     .printf_loop

.exit:
            add     rsp, 5*8
            pop     rbp

            mov     rax, r8

            ret


;;=========================================================================
;; Flag finder
;; Entry: rdi <- current cursor position
;; Exit:  rdi <- next format pos after format flag
;;=========================================================================

next_flag:
            cmp     byte [rdi], 0
            je      .exit
            cmp     byte [rdi], '%'
            je      .exit
            inc     rdi
            jmp     next_flag
.exit:
            ret

;;=========================================================================
;; Flag parser
;; Entry: rdi <- position where is flag
;;        rbp <- argument position
;; Destr: 
;;=========================================================================

SHIELD_FLAG equ '%'

CHAR_FLAG   equ 'c'
CSTR_FLAG   equ 's'
BINT_FLAG   equ 'b'
OINT_FLAG   equ 'o'
XINT_FLAG   equ 'x'
UINT_FLAG   equ 'u'
DINT_FLAG   equ 'd'

parse_flag:
            push    rdi
            cmp     byte [rdi], SHIELD_FLAG
            je      .is_shield
            jmp     .is_flag

.is_shield:
            mov     rdi, procent
            call    _put_s
            jmp     .exit

.is_flag:
            xor     rcx, rcx
            mov     al, byte [rdi]
            mov     rdi, qword [rbp]

.find_suitable_func:
            cmp     byte [int_flag_table+rcx], 0
            je      .flag_error

            cmp     al, byte [int_flag_table+rcx]
            je      .call_suitable_func

            inc     rcx
            jmp     .find_suitable_func

.call_suitable_func:
            call    qword [put_int_table+8*rcx]
            jmp     .exit

.flag_error:
            mov     rax, 1
            mov     rdi, 1
            mov     rsi, ErrMsg
            mov     rdx, ErrMsgLen
            syscall

            mov     rax, 60
            xor     rdi, rdi
            syscall

.exit:
            pop     rdi

            ret

;;-------------------------------------------------------------------------

            section .data

procent:    db '%', 0

int_flag_table:
            db UINT_FLAG, DINT_FLAG, BINT_FLAG, OINT_FLAG, XINT_FLAG
            db CHAR_FLAG, CSTR_FLAG,                                    0
put_int_table:
            dq _put_u,    _put_d,    _put_b,    _put_o,    _put_x
            dq _put_c,    _put_s,                                       0


ErrMsg:     db "Unknown flag! Terminate...", 10
ErrMsgLen   equ $ - ErrMsg

;;=========================================================================









