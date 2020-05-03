/*
 * asm.h
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef ASM_H_
#define ASM_H_

enum
{
	ASM_OK 			   = 0,
	WRONG_CMD_ERR 	   = 1,
	EMPTY_ARG_ERR 	   = 2,
	REG_EXP_ERR		   = 3,
	INT_EXP_ERR		   = 4,
	WRONG_ARG_ERR 	   = 5,
	LABEL_ERR 		   = 6,
	JMP_NONDEF 		   = 7,
	OPEN_BRAC_ERR 	   = 8,
	INPUT_ERR 		   = 9,
	OUTPUT_ERR 		   = 10,
	WRONG_CONSOLE_ARGS = 11,
	UNDEF_REFERENCE    = 12
};


int IsReg (char* name, size_t len);
void PutSignature (char** out);
Err_n Compiler(FILE *in, FILE *out);
char* SkipComment(char *curr);
bool GetSmth (char* curr);
Err_n ParseLabel (char* name, int len, Labels* labels, int ptr);
Err_n ParseArgs  (char** in, char** out, int n_args, Labels* labels);

#endif /* ASM_H_ */
