;===============================================================================
; > get_string
;===============================================================================

            global  get_string

            section .text

;###############################################################################
;# Get string from stdin to buffer
;# ENTRY: RSI <- arrd of buffer
;# EXIT:  @
;# DESTR: RAX RDI RDX 
;###############################################################################

get_string:
            xor     rdi, rdi                    ; file desctiptor = STDIN = 0
            mov     rdx, 1024
            xor     rax, rax                    ; 0 syscall - read from STDIN
            syscall                             ; make syscall

            ret                                 ; return 

;###############################################################################

