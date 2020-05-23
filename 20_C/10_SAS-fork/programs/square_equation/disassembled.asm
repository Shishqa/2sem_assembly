	JMP label_0 
label_0:
	OUTC 65 
	OUTC 58 
	OUTC 32 
	IN 
	POP dh 
	PUSH dh 
	PUSH 0 
	JB label_1 
	PUSH 10 
	PUSH dh 
	POW 
	POP ch 
	IN 
	PUSH ch 
	MUL 
	POP ax 
	IN 
	PUSH ch 
	MUL 
	POP bx 
	IN 
	PUSH ch 
	MUL 
	POP cx 
	CALL label_2 
label_1:
	END 
label_2:
	PUSH ax 
	PUSH 0 
	JE label_3 
	PUSH bx 
	PUSH bx 
	MUL 
	PUSH ch 
	DIV 
	PUSH 4 
	PUSH ax 
	PUSH cx 
	MUL 
	PUSH ch 
	DIV 
	MUL 
	SUB 
	POP dx 
	PUSH dx 
	PUSH 0 
	JB label_4 
	PUSH dx 
	PUSH 0 
	JE label_5 
	JMP label_6 
label_5:
	PUSH bx 
	PUSH -1 
	MUL 
	PUSH ch 
	MUL 
	PUSH 2 
	PUSH ax 
	MUL 
	DIV 
	PUSH dx 
	PUSH 0 
	JE label_7 
	JMP label_6 
label_6:
	PUSH bx 
	PUSH -1 
	MUL 
	PUSH ch 
	MUL 
	PUSH 2 
	PUSH ax 
	MUL 
	DIV 
	PUSH dx 
	SQRT dh 
	PUSH ch 
	MUL 
	PUSH 2 
	PUSH ax 
	MUL 
	DIV 
	SUB 
	PUSH bx 
	PUSH -1 
	MUL 
	PUSH ch 
	MUL 
	PUSH 2 
	PUSH ax 
	MUL 
	DIV 
	PUSH dx 
	SQRT dh 
	PUSH ch 
	MUL 
	PUSH 2 
	PUSH ax 
	MUL 
	DIV 
	ADD 
	JMP label_8 
label_3:
	PUSH bx 
	PUSH 0 
	JE label_9 
	PUSH cx 
	PUSH -1 
	PUSH ch 
	MUL 
	MUL 
	PUSH bx 
	DIV 
	JMP label_7 
label_9:
	PUSH cx 
	PUSH 0 
	JE label_10 
	JMP label_4 
label_10:
	OUTC 73 
	OUTC 10 
	RET 
label_4:
	PUSH 0 
	OUT 
	OUTC 10 
	RET 
label_7:
	PUSH 1 
	OUT 
	OUTC 32 
	OUTF dh 
	OUTC 10 
	RET 
label_8:
	PUSH 2 
	OUT 
	OUTC 32 
	OUTF dh 
	OUTC 32 
	OUTF dh 
	OUTC 10 
	RET 
10 
	RET 
