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

            extern  _flush_buffer

            section .text

;;=========================================================================
;; printf
;;
;; supports: %d %u %b %o %x %c %s %%
;;
;; ENTRY: RDI <- format string address (null terminated)
;;        RSI <- 1st arg
;;        ...
;;   <stdcall seq>
;;        ...
;;         R9 <- 5th arg
;;      STACK <- other args from right to left
;; DESTR: RAX RBX RCX RDX RSI RDI R8 ; DF
;;=========================================================================

printf:
            push    rbp
            mov     rbp, rsp
            sub     rsp, 5*8                        ; init stack frame

            mov     qword [rbp-5*8], rsi
            mov     qword [rbp-4*8], rdx
            mov     qword [rbp-3*8], rcx
            mov     qword [rbp-2*8], r8
            mov     qword [rbp-1*8], r9             ; load args to memory

            xor     r8, r8                          ; R8 - arg counter

            sub     rbp, 5*8                        ; RBP -> first arg

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

            cmp     byte [rdi], SHIELD_FLAG         ; if printed one of the args,
            jne     .next_arg                       ; prepare next arg

            inc     rdi
            jmp     .printf_loop                    ; loop to the next flag

.next_arg:
            inc     rdi
            add     rbp, 8
            inc     r8
            cmp     r8, 5
            jne     .printf_loop                    ; if printed 5 args, skip
                                                    ; old RBP and ret address
            add     rbp, 16                         ; sof (old_RBP + @ret) = 8+8 = 16
            jmp     .printf_loop

.exit:
            call _flush_buffer                      ; flush symbols left in buffer

            add     rsp, 5*8
            pop     rbp                             ; remove stack frame

            mov     rax, r8                         ; return num of printed args

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
;; Destr: RAX RBX RCX RDX RSI ; DF
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

            cmp     byte [rdi], SHIELD_FLAG         ; if shielded symbol
            je      .is_shield                      ; (just print it)
            jmp     .is_flag

.is_shield:
            mov     sil, SHIELD_FLAG
            call    _put_c                          ; print shielded symbol
            jmp     .exit

.is_flag:
            xor     rcx, rcx                        ; RCX - flag iterator
            mov     al, byte [rdi]                  ; current flag
            mov     rsi, qword [rbp]                ; current argument

.find_suitable_flag:

            cmp     byte [flag_table+rcx], 0        ; found no flag with no such
            je      .exit                           ; char value

            cmp     al, byte [flag_table+rcx]       ; if found, print
            je      .call_suitable_func

            inc     rcx                             ; loop to the next flag
            jmp     .find_suitable_flag

.call_suitable_func:

            call    qword [call_table+8*rcx]        ; print current arg

.exit:
            pop     rdi                             ; restore current position

            ret

;;-------------------------------------------------------------------------

            section .data

flag_table:
            db UINT_FLAG, DINT_FLAG, BINT_FLAG, OINT_FLAG, XINT_FLAG
            db CHAR_FLAG, CSTR_FLAG,                                    0

call_table:
            dq _put_u,    _put_d,    _put_b,    _put_o,    _put_x
            dq _put_c,    _put_s,                                       0

;;=========================================================================









