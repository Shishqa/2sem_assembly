;;=========================================================================
;; put_u_base.asm                                       Shishqa, MIPT 2020
;;=========================================================================

            global  _put_u_base

            extern  _itoa
            extern  _put_s

            section .text

;;=========================================================================
;; Writes unsigned int number in base representation to console
;;
;; ENTRY: ESI <- int number to print
;;         DH <- binary degree if base is binary / 0 if not
;;         DL <- base (<= 16)
;; DESTR: RAX RBX RDX RSI RDI ; DF
;;=========================================================================

_put_u_base:

            lea     rdi, [IntBuffer + (IntBufSize - 2)] ; RDI -> where number
            call    _itoa                               ; will end.
                                                        ; convert num to str

            mov     rsi, rdi
            call    _put_s                              ; print converted num

            ret

;;-------------------------------------------------------------------------

            section .data

IntBufSize  equ 33

IntBuffer:  times IntBufSize db 0

;;=========================================================================











