	JMP label_0 
label_0:
	IN 
	POP dl 
	IN 
	POP dx 
	PUSH dl 
	PUSH 1 
	JE label_1 
	JMP label_2 
label_2:
	MOV dx ax 
	CALL label_3 
	PUSH ah 
	OUT 
	OUTC 10 
	JMP label_4 
label_1:
	MOV 0 ax 
	JMP label_5 
label_5:
	PUSH ax 
	PUSH dx 
	JA label_6 
	MOV 0 ah 
	CALL label_3 
	PUSH ah 
	OUT 
	OUTC 32 
	PUSH ax 
	PUSH 1 
	ADD 
	POP ax 
	JMP label_5 
label_6:
	OUTC 10 
	JMP label_4 
label_4:
	END 
label_3:
	PUSH ax 
	PUSH ax 
	PUSH 0 
	JE label_7 
	PUSH ax 
	PUSH 1 
	JE label_8 
	PUSH ax 
	PUSH 1 
	SUB 
	PUSH ax 
	PUSH 2 
	SUB 
	POP ax 
	CALL label_3 
	POP ax 
	PUSH ah 
	CALL label_3 
	PUSH ah 
	ADD 
	JMP label_9 
label_7:
	PUSH 0 
	JMP label_9 
label_8:
	PUSH 1 
	JMP label_9 
label_9:
	POP ah 
	POP ax 
	RET 
ax 
	RET 
