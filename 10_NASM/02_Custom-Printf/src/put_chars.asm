;;=========================================================================
;;                                                         (c)Shishqa,2020
;;=========================================================================

%macro      store_char 0

            cld
            lea     rdi, [Buffer]
            add     rdi, qword [BufEnd]         ; RDI points to buffer end
            stosb                               ; write new char to buffer
            inc     byte [BufEnd]               ; BufEnd++

%endmacro


%macro      check_buf_overflow 0

            mov     al, byte [BufEnd]
            cmp     al, byte [BufBegin]         ; if BufBegin == BufEnd,
            jne     %%do_not_flush              ; the buffer is full
                                                ; so, we need to flush it
            call    _flush_buffer               ; to the stdout

    %%do_not_flush:

%endmacro

            section .text

            global  _put_s
            global  _put_c
            global  _flush_buffer
            global  _set_buffer

;;=========================================================================
;; Writes string to the console
;;
;; The output is buffered, so string
;; will not appear immediately !
;;
;; ENTRY: RSI <- string address
;; DESTR: RAX RDI ; CC
;;=========================================================================

_put_s:

.print_loop:

            mov     al, byte [rsi]

            cmp     al, 0                       ; if AL == 0, the null str
            je      .exit                       ; terminal is reached -> exit

            store_char                          ; [Buffer + BufEnd++] = AL

            check_buf_overflow                  ; flush buff if need

            inc     rsi                         ; loop to the next symbol
            jmp     .print_loop

.exit:

            ret

;;=========================================================================
;; Writes char symbol to the console
;;
;; The output is buffered, so symbol
;; will not appear immediately !
;;
;; Entry: SIL <- char value
;; Destr: RAX RDI ; CC 
;;=========================================================================

_put_c:
            mov     al, sil

            store_char                          ; [Buffer + BufEnd++] = AL

            check_buf_overflow                  ; flush buff if need

            ret

;;=========================================================================

_set_buffer:

            mov     qword [BufBegin], 0
            mov     qword [BufEnd], 0

            ret


;;=========================================================================
;; Frees output buffer
;;
;; DESTR: RAX RDI
;;=========================================================================

_flush_buffer:

            push    rsi
            push    rdx                         ; save RSI RDX

            mov     rdi, qword [BufBegin]       ; |def_____abc| - 2 blocks
            cmp     rdi, qword [BufEnd]         ; |__abc______| - 1 block
            ja      .two_blocks

.single_block:

            mov     rax, 1                      ; print single block
            mov     rdi, 1
            lea     rsi, [Buffer]
            add     rsi, qword [BufBegin]
            xor     rdx, rdx
            mov     dl, byte [BufEnd]
            sub     dl, byte [BufBegin]       ; len = BufEnd - BufBegin
            syscall

            jmp     .exit

.two_blocks:

            mov     rax, 1                      ; print first block
            mov     rdi, 1
            lea     rsi, [Buffer]
            add     rsi, qword [BufBegin]
            mov     rdx, BufSize
            sub     rdx, qword [BufBegin]       ; len = BufSize - BufBegin
            syscall

            lea     rsi, [Buffer]
            mov     rdx, qword [BufEnd]         ; len = BufEnd - 0
            syscall

.exit:
            mov     al, byte [BufEnd]
            mov     byte [BufBegin], al         ; BufBegin = BufEnd

            pop     rdx
            pop     rsi                         ; restore RDX RSI

            ret

;;-------------------------------------------------------------------------

            section .data

BufSize     equ 512

BufBegin:   dq 0
BufEnd:     dq 0

Buffer:     times BufSize db 0

;;=========================================================================


