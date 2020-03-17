;;=========================================================================
;; Put_b
;;=========================================================================

            global  _put_b
            global  _put_o
            global  _put_u
            global  _put_x

            extern  _put_u_base

            section .text

;;=========================================================================
;; Writes unsigned int in binary representation to the console
;; Entry: edi <- int number to print
;; Destr: 
;;=========================================================================

_put_b:
            mov     dx, (1 << 8) | 2
            call    _put_u_base

            ret

;;=========================================================================
;; Writes unsigned int in decimal representation to the console
;; Entry: edi <- int number to print
;; Destr: 
;;=========================================================================

_put_u:
            mov     dx, (0 << 8) | 10
            call    _put_u_base

            ret

;;=========================================================================
;; Writes unsigned int in octal representation to the console
;; Entry: edi <- int number to print
;; Destr: 
;;=========================================================================

_put_o:
            mov     dx, (3 << 8) | 8
            call    _put_u_base

            ret

;;=========================================================================
;; Writes unsigned int in hexadecimal representation to the console
;; Entry: edi <- int number to print
;; Destr: 
;;=========================================================================

_put_x:
            mov     dx, (4 << 8) | 16
            call    _put_u_base

            ret

;;=========================================================================









