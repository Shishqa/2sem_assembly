;;=========================================================================
;; Custom Printf
;;=========================================================================

            global  _printf

            extern  _put_s
            extern  _put_c
            extern  _put_u
            extern  _put_b
            extern  _put_o
            extern  _put_x
            extern  _put_d

            section .text

;;=========================================================================
;; Printf
;; Entry: rdi <- format address (null terminated)
;;        next: stdcall args
;; Destr: 
;;=========================================================================

printf:
            push    rbp
            mov     rbp, rsp
            dec     rbp, 8

            mov     rax, rdi
            call    parse_next

            cmp     rsi, 0
            je      .error

            xor     bl, bl
            dec     rdi
            xchg    byte [rdi], bl

            mov     rdi, rax
            call    _put_s

            xchg    byte [rdi], bl

            mov     rbx, rdi
            mov




;;=========================================================================
;; Parser
;; Entry: rdi <- current cursor position
;; Exit:  rdi <- next format pos after format flag
;;        rsi <- address of printer
;;=========================================================================

parse_next:
            cmp     byte [rdi], 0
            je      .exit
            cmp     byte [rdi], '%'
            je      .get_format
            inc     rdi
            jmp     parse_next

.get_format:
            inc     rdi
            mov     al, byte [rdi]
            xor     rcx, rcx
.loop:
            cmp     byte [FormatTable+rcx], 0
            je      .error

            cmp     al, byte [FormatTable+rcx]
            je      .checkout
            add     rcx, 5
            jmp     .loop

.checkout:
            inc     rcx
            mov     rsi, qword [FormatTable+rcx]
            jmp     .exit

.error:
            mov     rsi, 0
.exit:
            ret

;;-------------------------------------------------------------------------

            section .data

FormatTable:
            db 'c', dq _put_c
            db 's', dq _put_s
            db 'u', dq _put_u
            db 'b', dq _put_b
            db 'o', dq _put_o
            db 'x', dq _put_x
            db 'd', dq _put_d
            db '%', dq 0
            db 0

;;=========================================================================


            

