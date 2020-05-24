;;=========================================================================
;; put_u.asm                                            Shishqa, MIPT 2020
;;=========================================================================

            global  _put_b
            global  _put_o
            global  _put_u
            global  _put_x

            extern  _put_u_base

            section .text

;;=========================================================================
;; Writes unsigned int in binary representation to the console
;;
;; ENTRY: ESI <- int number to print
;; DESTR: RAX RBX RDX RSI RDI ; CC
;;=========================================================================

_put_b:
            mov     dx, (1 << 8) | 2        ; DL = 2, DH = 1 (BASE = 2^1)
            call    _put_u_base

            ret

;;=========================================================================
;; Writes unsigned int in decimal representation to the console
;;
;; Entry: ESI <- int number to print
;; Destr: RAX RBX RDX RSI RDI ; CC
;;=========================================================================

_put_u:
            mov     dx, (0 << 8) | 10       ; DL = 10, DH = 0 (BASE = 10)
            call    _put_u_base

            ret

;;=========================================================================
;; Writes unsigned int in octal representation to the console
;;
;; ENTRY: ESI <- int number to print
;; Destr: RAX RBX RDX RSI RDI ; CC
;;=========================================================================

_put_o:
            mov     dx, (3 << 8) | 8        ; DL = 8, DH = 3 (BASE = 2^3)
            call    _put_u_base

            ret

;;=========================================================================
;; Writes unsigned int in hexadecimal representation to the console
;;
;; ENTRY: ESI <- int number to print
;; DESTR: RAX RBX RDX RSI RDI ; CC
;;=========================================================================

_put_x:
            mov     dx, (4 << 8) | 16       ; DL = 16, DH = 4 (BASE = 2^4)
            call    _put_u_base

            ret

;;=========================================================================









