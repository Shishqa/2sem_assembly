; ##      THIS FILE IS GENERATED AUTOMATICALLY      ##
; ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##
	CALL	func_main
	MOV		100	ex
	END

;#####################################################################
;	function "Fib"
func_Fib:
;	Fib_var_num (arg_1)
	POP [ex+1]
;	"Fib" body:

	if_0:
;	if_0_condition
		PUSH	[ex+1] 	; LOCAL VAR
		PUSH	1 	; NUM
	JBE case_0_positive
	JMP case_0_negative
	case_0_positive:
		PUSH	1
		JMP	case_0_continue
	case_0_negative:
		PUSH	0
	case_0_continue:
		PUSH	0
		JNE	if_0_positive
		JMP	if_0_negative
	if_0_positive:

		PUSH	[ex+1] 	; LOCAL VAR
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET

		JMP	if_0_end
	if_0_negative:


	if_0_end:
		PUSH	[ex+1] 	; LOCAL VAR
		PUSH	1 	; NUM
		SUB
	MOV	ex+50	ex
	CALL	func_Fib
		PUSH	[ex+1] 	; LOCAL VAR
		PUSH	2 	; NUM
		SUB
	MOV	ex+50	ex
	CALL	func_Fib
		ADD
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET
;#####################################################################


;#####################################################################
;	function "main"
func_main:
;	"main" body:

	IN
	POP	[ex+1]
	IN
	POP	[ex+2]
	if_1:
;	if_1_condition
		PUSH	[ex+1] 	; LOCAL VAR
		PUSH	1 	; NUM
	JE case_1_positive
	JMP case_1_negative
	case_1_positive:
		PUSH	1
		JMP	case_1_continue
	case_1_negative:
		PUSH	0
	case_1_continue:
		PUSH	0
		JNE	if_1_positive
		JMP	if_1_negative
	if_1_positive:

		PUSH	0 	; NUM
	POP	[ex+3]
cycle_0:
		PUSH	[ex+3] 	; LOCAL VAR
		PUSH	[ex+2] 	; LOCAL VAR
	JBE case_2_positive
	JMP case_2_negative
	case_2_positive:
		PUSH	1
		JMP	case_2_continue
	case_2_negative:
		PUSH	0
	case_2_continue:
	PUSH	0
	JE cycle_0_stop
		PUSH	[ex+3] 	; LOCAL VAR
	MOV	ex+50	ex
	CALL	func_Fib
	OUT
	if_2:
;	if_2_condition
		PUSH	[ex+3] 	; LOCAL VAR
		PUSH	[ex+2] 	; LOCAL VAR
	JNE case_3_positive
	JMP case_3_negative
	case_3_positive:
		PUSH	1
		JMP	case_3_continue
	case_3_negative:
		PUSH	0
	case_3_continue:
		PUSH	0
		JNE	if_2_positive
		JMP	if_2_negative
	if_2_positive:

		PUSH	44 	; NUM
	OUTC

		JMP	if_2_end
	if_2_negative:


	if_2_end:
		PUSH	32 	; NUM
	OUTC
		PUSH	[ex+3] 	; LOCAL VAR
		PUSH	1 	; NUM
		ADD
	POP	[ex+3]
	JMP cycle_0
cycle_0_stop:

		JMP	if_1_end
	if_1_negative:

		PUSH	[ex+2] 	; LOCAL VAR
	MOV	ex+50	ex
	CALL	func_Fib
	OUT

	if_1_end:
		PUSH	10 	; NUM
	OUTC
		PUSH	0 	; NUM
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET
;#####################################################################


