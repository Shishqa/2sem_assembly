;;=======================================================================
;; Echo                                                   (c)Shishqa,2020
;;=======================================================================

;            default rel

            global  main
            extern  puts

            section .text

main:                               ; argc -> |rdi| and argv -> |rsi|
            push    rdi             ; save rdi,rsi
            push    rsi
            sub     rsp, 8          ; aligning stack because |rsi|rdi|args
                                    ;                      sp^-->--sp^
            mov     rdi, [rsi]      ; address of string = [argv]
            call    puts wrt ..plt

            add     rsp, 8          ; stack pointer restore
            pop     rsi             ; restore rsi,rdi
            pop     rdi

            add     rsi, 8          ; argv++
            dec     edi             ; argc--
            jnz     main            ; loop

            xor     eax, eax        ; return value - 0
            ret

