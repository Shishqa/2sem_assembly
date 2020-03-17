;;=========================================================================
;; Custom puts implementation                               (c)Shishqa,2020
;;=========================================================================

            section .text

            global  _put_s

            extern  _put_c

;;=========================================================================
;; Writes string to the console
;; Entry: rdi <- string address
;; Destr: rax, rdi, rsi, rdx
;;=========================================================================

_put_s:
            cmp     byte [rdi], 0
            je      .exit

            mov     rcx, rdi
            push    rcx
            mov     dil, byte [rcx]
            call    _put_c
            pop     rdi
            inc     rdi
            jmp     _put_s
.exit:
            ret

;;=========================================================================
