;;=========================================================================
;; Test printf
;;=========================================================================

            default rel

            global  _start
            extern  printf

            section .text

;;=========================================================================

_start:
            mov     rdi, format

            mov     rsi, qword CHAR_TEST
            mov     rdx, CSTR_TEST
            mov     rcx, qword DINT_TEST
            mov     r8,  qword UINT_TEST
            mov     r9,  qword BINT_TEST
            push    qword XINT_TEST
            push    qword OINT_TEST

            call    printf

            mov     rax, 60
            xor     rdi, rdi
            syscall

;;-------------------------------------------------------------------------

            section .data

CHAR_TEST   equ 'G'

CSTR_TEST:  db '"Hello, it is a string"', 0

DINT_TEST   equ -12
UINT_TEST   equ 235123
BINT_TEST   equ 2121
OINT_TEST   equ 9898
XINT_TEST   equ 1024

format:     db "Test formats:", 10
            db "%%c: %c", 9,9,9,9,  "(should be 'G')",      10
            db "%%s: %s",                                   10
            db "%%d: %d", 9,9,9,9,  "(should be -12d)",     10
            db "%%u: %u", 9,9,9,    "(should be 235123d)",  10
            db "%%b: %b", 9,9,      "(should be 2121d)",    10
            db "%%o: %o", 9,9,9,    "(should be 9898d)",    10
            db "%%x: %x", 9,9,9,9,  "(should be 1024d)",    10
            db "Done",                                      10
            db "Undefined flag (%%y) will cause error %y",  10, 0

;;=========================================================================

