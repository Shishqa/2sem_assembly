;===============================================================================
; > test_get
;===============================================================================

            global  _start

            extern  check_password

            section .text

;###############################################################################

_start:
            mov     rax, 1
            mov     rdi, 1
            mov     rsi, MsgAsk
            mov     rdx, ASK_LEN
            syscall

            mov     rdi, USER_HASH

            call    check_password

            jnz     .denied

.granted:
            mov     rsi, MsgCorrect
            mov     rdx, CORRECT_LEN
            jmp     .print_msg

.denied:
            mov     rsi, MsgWrong
            mov     rdx, WRONG_LEN

.print_msg:
            mov     rdi, 1
            mov     rax, 1
            syscall

            mov     rax, 0x3C
            xor     rdi, rdi
            syscall

;-------------------------------------------------------------------------------

            section .data

USER_HASH   equ 0x8ADC94C64A71ACC4

MsgAsk:     db "User password: "
ASK_LEN     equ $ - MsgAsk

MsgCorrect: db "Access granted", 10
CORRECT_LEN equ $ - MsgCorrect

MsgWrong:   db "Access denied", 10
WRONG_LEN   equ $ - MsgWrong

;###############################################################################

