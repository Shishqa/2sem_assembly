;===============================================================================
; > check_password
;===============================================================================

            global  check_password
            extern  get_string

            section .text

;###############################################################################
;# Checks user password
;# ENTRY: RDI <- user password addr
;#        RSI <- user password length
;# EXIT:  RAX -> 0 if password is correct
;#               1 if password is incorrect
;# DESTR: RDI RSI RDX
;###############################################################################

check_password:

            push    rbp
            mov     rbp, rsp
            sub     rsp, BUFFER_SIZE + 16

            mov     qword [rbp - 8], rsp

            mov     qword [rbp - 16], rdi

            mov     rsi, rsp
            call    get_string

            xor     qword [rbp - 8], rsp
            jne     .stack_smashed

            add     rsp, BUFFER_SIZE + 16
            pop     rbp
            ret

            ;mov     rcx, BUFFER_SIZE
            ;mov     rdi, qword [rbp - 16]

            ;repne   cmpsb

                        


.stack_smashed:

            mov     rax, 60
            mov     rdi, 1337
            syscall
            

;-------------------------------------------------------------------------------

            section .data

BUFFER_SIZE equ 10
CANARY_VALUE equ 0x123abc

;###############################################################################

