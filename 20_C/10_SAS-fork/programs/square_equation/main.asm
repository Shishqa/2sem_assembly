	JMP input
	
input:		
	PUSH    1000
	POP		fx

    PUSH    58
    PUSH    67
    PUSH    67
    PUSH    65
    OUTC
    OUTC
    OUTC
    OUTC

    PUSH    fx
    OUTF    3

    PUSH    10
    OUTC

    PUSH    65
    OUTC
    PUSH    58
    OUTC
    PUSH    32
    OUTC
	IN
	POP 	ax

    PUSH    66
    OUTC
    PUSH    58
    OUTC
    PUSH    32
    OUTC
	IN 
	PUSH	fx
	MUL
	POP 	bx

    PUSH    67
    OUTC
    PUSH    58
    OUTC
    PUSH    32
    OUTC
	IN
	PUSH	fx
	MUL
	POP 	cx

	CALL 	solve_square_equation
exit:
	END

;=============================================
	
solve_square_equation:

	PUSH 	ax
	PUSH 	0
	JE 		solve_linear_equation
	
	PUSH 	bx
	PUSH 	bx
    MUL
	
	PUSH 	4
	PUSH 	ax
	PUSH 	cx
	MUL
    MUL
    PUSH    fx
    MUL
	SUB
	
	POP 	dx
	
	PUSH 	dx
	PUSH 	0
	JB 		zero_roots
	
	PUSH 	dx
	PUSH 	0
	JE 		get_one_root
	
	JMP 	get_two_roots
	
get_one_root:

	PUSH 	bx
	PUSH 	-1
	MUL
	
	PUSH 	2
	PUSH 	ax
    MUL

    DIV

	JMP  	one_root
	
get_two_roots:

    PUSH    dx
    SQRT    0
    POP     ex

	PUSH 	bx
	PUSH 	-1
	MUL
    PUSH    ex
    SUB

	PUSH 	2
	PUSH 	ax
    MUL

    DIV

	PUSH 	bx
	PUSH 	-1
	MUL
	PUSH	ex
	ADD

	PUSH 	2
	PUSH 	ax
    MUL

    DIV

	JMP 	two_roots
	
solve_linear_equation:

	PUSH 	bx
	PUSH 	0 
	JE 		solve_equality

	PUSH 	cx
	PUSH 	-1
	MUL
    PUSH    fx
    MUL

	PUSH 	bx

	DIV

	JMP 	one_root
	
solve_equality:

	PUSH 	cx
	PUSH 	0
	JE 		infinite_roots
	JMP 	zero_roots
	
infinite_roots:

    PUSH    73
	OUTC 	
    PUSH    10
	OUTC 	
	RET
	
zero_roots:
	PUSH 	48
	OUTC 
    PUSH    82
    OUTC
    PUSH    10
	OUTC 	
	RET

one_root:
	PUSH 	49
	OUTC
    PUSH    82
    OUTC
    PUSH    58
    OUTC
    PUSH    32
	OUTC 	
	OUTF    3
    PUSH    10
	OUTC
	RET

two_roots:
	PUSH    50
	OUTC
    PUSH    82
    OUTC
    PUSH    58
    OUTC
    PUSH    32
	OUTC 	
	OUTF    3
    PUSH    32
	OUTC 	
	OUTF    3
    PUSH    10
	OUTC 	
	RET
