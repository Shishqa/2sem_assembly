;;=========================================================================
;; printf.asm                                           Shishqa, MIPT 2020
;;=========================================================================

%macro      multipop 1-*

            %rep %0

                pop %1

            %rotate 1
            %endrep

%endmacro

            global  printf

            extern  _put_s
            extern  _put_c
            extern  _put_u
            extern  _put_b
            extern  _put_o
            extern  _put_x
            extern  _put_d

            extern  _flush_buffer
            extern  _set_buffer

            section .text

;;=========================================================================
;; printf
;;
;; supports: %d %u %b %o %x %c %s %%
;;
;; ENTRY: RDI <- format string address (null terminated)
;;
;;        {RSI RDX RCX R8 R9} <- {1-5th args from left to right}
;;
;;        STACK <- other args from right to left (6th on the top)
;;
;; DESTR: RAX RCX RDX RSI RDI R8 R11 ; CC
;;=========================================================================

printf:
            pop     rax                             ; ret addr -> RAX

            push    r9
            push    r8
            push    rcx                             ; push args passed
            push    rdx                             ; by registers to
            push    rsi                             ; stack

            push    rax                             ; push ret addr(RAX)

            push    rbp                             ; save old RBP
            mov     rbp, rsp                        ; 
            add     rbp, 16                         ; RBP -> 1st arg 

            call    _set_buffer                     ; align buf ptrs

            push    rbx                             ; save RBX

            xor     r8, r8

.printf_loop:

            mov     rax, rdi
            call    next_flag                       ; find format flag

            xor     bl, bl
            xchg    byte [rdi], bl                  ; '%' <-> '0'
                                                    ; (to make _put_s work)
            mov     rsi, rax                        ; RSI = str block begin
            call    _put_s                          ; print this block
            mov     rdi, rsi                        ; RDI = RDI before puts

            xchg    byte [rdi], bl                  ; '0' <-> '%'

            cmp     byte [rdi], 0                   ; if met '0', exit
            je      .exit

            inc     rdi
            call    parse_flag                      ; parse current flag
            inc     rdi

            cmp     byte [rdi-1], SHIELD_FLAG       ; if printed one of the args,
            je      .printf_loop                    ; prepare next arg

            add     rbp, 8                          ; RBP -> next arg
            inc     r8
            jmp     .printf_loop

.exit:
            pop     rbx                             ; restore RBX
            pop     rbp                             ; restore RBP

            call    _flush_buffer

            pop     rax
            add     rsp, 8 * 5
            push    rax

            mov     rax, r8

            ret

;;=========================================================================
;; Find next format flag
;;
;; ENTRY: RDI <- current cursor position
;; EXIT:  RDI <- next format flag pos (marker is '%')
;;               or string end
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
;; Parse flag and print current argument according to flag
;;
;; ENTRY: RDI <- position where is flag
;;        RBP <- address of current argument
;; Destr: RAX RBX RCX RDX RSI ; CC
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
            push    rdi                             ; save current position

            xor     rax, rax
            mov     al, byte [rdi]

            cmp     al, SHIELD_FLAG                 ; if shielded symbol
            je      .is_shield                      ; (just print it)
            jmp     .is_flag

.is_shield:
            mov     sil, SHIELD_FLAG
            call    _put_c                          ; print shielded symbol
            jmp     .exit

.is_flag:
            mov     rsi, qword [rbp]

            sub     al, BINT_FLAG
            cmp     al, SIZEOF_TABLE
            ja      .exit

            call    qword [module_table + 8*rax]

.exit:
            pop     rdi                             ; restore current position

            ret

nope:       ret

;;-------------------------------------------------------------------------

            section .data

module_table: dq _put_b ; 0
              times CHAR_FLAG-BINT_FLAG-1 dq nope
              dq _put_c ; 1
              times DINT_FLAG-CHAR_FLAG-1 dq nope 
              dq _put_d ; 2    
              times OINT_FLAG-DINT_FLAG-1 dq nope
              dq _put_o ; 13
              times CSTR_FLAG-OINT_FLAG-1 dq nope 
              dq _put_s ; 17
              times UINT_FLAG-CSTR_FLAG-1 dq nope
              dq _put_u ; 19
              times XINT_FLAG-UINT_FLAG-1 dq nope
              dq _put_x ; 22

SIZEOF_TABLE  equ 22

;;=========================================================================









