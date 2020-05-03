; ##      THIS FILE IS GENERATED AUTOMATICALLY      ##
; ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##
; ORIGIN :: programs/get_chars/main.plan

	CALL	func_main
	MOV		100	ex
	END

;#####################################################################
;	function "main"
func_main:
;	"main" body:

; pushing argument before call putc
		PUSH	32
	OUTC
; pushing argument before call return
		PUSH	0
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET
;#####################################################################


