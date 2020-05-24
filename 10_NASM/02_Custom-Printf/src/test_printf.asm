;;=========================================================================
;; Test printf
;;=========================================================================

%macro      multipush 0-*

            %rep %0

                push    %1

            %rotate 1
            %endrep

%endmacro

%macro      invoke 1-*

        %if %0 > 1

            mov     rdi, %2

        %if %0 > 2

            mov     rsi, %3

        %if %0 > 3

            mov     rdx, %4

        %if %0 > 4

            mov     rcx, %5

        %if %0 > 5

            mov     r8, %6

        %if %0 > 6

            mov     r9, %7

        %if %0 > 7

            multipush %{-1:8}

        %endif
        %endif
        %endif
        %endif
        %endif
        %endif
        %endif

            call    %1

        %if %0 > 7

            add     rsp, (%0 - 8) * 8

        %endif

%endmacro


            default rel

            global  _start
            extern  printf

            section .text

;;=========================================================================

_start:
            invoke printf, format1, qword CHAR_TEST, CSTR_TEST, qword DINT_TEST, qword UINT_TEST, qword BINT_TEST, qword OINT_TEST, qword XINT_TEST

            invoke printf, format2, lovestr, qword 3802, qword 10000000, qword '!', qword 127, qword 101, meowstr

            mov     rax, 60
            xor     rdi, rdi
            syscall

;;-------------------------------------------------------------------------

            section .data

CHAR_TEST   equ 'G'

CSTR_TEST:  db '"Hello, it is a string"', 0

DINT_TEST   equ -8625
UINT_TEST   equ 235123
BINT_TEST   equ 2121
OINT_TEST   equ 9898
XINT_TEST   equ 1024

format2:    db 'I %s %x%d%%%c%b, %d %s!' , 0xA, 0
lovestr:    db 'love', 0
meowstr:    db 'meow', 0

format1:    db "Test formats:", 10
            db "%%c: %c", 9,9,9,9,  "(should be 'G')",         10
            db "%%s: %s",                                      10
            db "%%d: %d", 9,9,9,    "(should be -8625)",       10
            db "%%u: %u", 9,9,9,    "(should be 235123)",      10
            db "%%b: %b", 9,9,      "(should be 100001001001)",10
            db "%%o: %o", 9,9,9,    "(should be 23252)",       10
            db "%%x: %x", 9,9,9,9,  "(should be 400)",         10
            db "Done",                                         10, 0

;;=========================================================================

