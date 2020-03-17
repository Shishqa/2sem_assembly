;;=========================================================================
;; Put_d
;;=========================================================================

            global  _put_d

            extern  _put_u_base
            extern  _put_s

            section .text

;;=========================================================================
;; Writes signed int in decimal representation to the console
;; Entry: edi <- int number to print
;; Destr: 
;;=========================================================================

_put_d:
            mov     edx, 1 << 31
            and     edx, edi

            cmp     edx, 0
            je      .positive
.negative:
            push    rdi
            mov     rdi, NegMarker
            call    _put_s
            pop     rdi

            neg     edi
.positive:
            mov     dx, (0 << 8) | 10
            call    _put_u_base

            ret

;;-------------------------------------------------------------------------

            section .data

NegMarker:  db '-', 0

;;=========================================================================










