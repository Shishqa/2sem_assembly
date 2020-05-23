	JMP 	start_
	
start_:

    PUSH    45
	POP	    ax

	CALL 	fibonacci	; ax == n, bx == F_n
	
    PUSH    68
    OUTC

	END
	
;==========================================================

fibonacci:
	PUSH	ax
	PUSH 	ax
	PUSH 	0
	JE 		F_0
	PUSH 	ax
	PUSH 	1
	JE 		F_1
	
	PUSH 	ax
	PUSH 	1
	SUB
	
	PUSH 	ax
	PUSH 	2
	SUB
	
	POP		ax
	CALL 	fibonacci
	POP		ax
	PUSH	bx
	CALL	fibonacci
	PUSH	bx
	ADD
	JMP		fib_return
	
F_0:
	PUSH 	0
	JMP 	fib_return
F_1:
	PUSH 	1
	JMP 	fib_return
	
fib_return:
	POP 	bx
	POP		ax
	RET

;==========================================================

