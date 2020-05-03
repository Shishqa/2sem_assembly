/*
 * macros.h
 *
 *  Created on: 24 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef MACROS_H_
#define MACROS_H_

#define SYS   0
#define JUMP  1
#define STD   2
#define CALC  3
#define FLOAT 4
#define GRAPH 5

#define T_REG( num ) \
	reg[USER_REG_NUM + num]

#define PUSH( val ) \
	StackPush (&cpu_stack, val);

#define PUSH_CALL( val ) \
	StackPush (&call_stack, val);

#define POP( to ) \
	StackPop (&cpu_stack, &(to));

#define POP_CALL( to ) \
	StackPop (&call_stack, &(to));

#define RET( code ) 			\
	free (buffer); 				\
    StackDestruct (&cpu_stack); \
	return (code);

#define INT_P \
	arg.int_param

#define REG_P \
	arg.reg_param

#define MEM_P \
	arg.mem_param

#define NOARG( curr ) \
	*(curr) == NOARG_PARAM

#define MCUR( val )	\
	curr += val;

#define SCUR( val )	\
	curr = code_begin + val;

#define SOF_ARG sizeof(Argument)


#define GET_ARG() \
	memcpy (&arg, curr, sizeof(Argument));


#define DUMP() \
	StackDump (&cpu_stack, "Call from cpu", __FILE__, "DUMP", curr_line);	\
    sleep (5);


#define GET_VAL( to )					\
	to = 0;								\
										\
	if (INT_P)							\
	{									\
		to += arg.val;					\
	}									\
										\
	if (REG_P)							\
	{									\
		to += reg[(int)arg.reg];		\
	}									\
										\
	if (!INT_P && !REG_P)				\
	{									\
		RET (WRONG_ARG_TYPE_ERR)		\
	}									\
										\
	if (MEM_P)							\
	{									\
		to = ram[to];					\
	}



#define DEF_JMP( opcode, name, op, len ) 				\
	DEF_CMD (opcode, name, JUMP, len, 1, 2,				\
	{													\
		POP (T_REG(2))									\
		POP (T_REG(1))									\
														\
		if (T_REG(1) op T_REG(2))						\
		{												\
			MCUR (1)									\
														\
			GET_ARG ()									\
														\
			GET_VAL (T_REG(0))							\
														\
			SCUR (T_REG(0))								\
		}												\
		else											\
		{												\
			MCUR (SOF_ARG + 1)							\
		}												\
	})


#define DEF_OP( opcode, name, op, len )								\
	DEF_CMD (opcode, name, CALC, len, 0, 2,							\
	{																\
		POP  (T_REG(1))												\
		POP  (T_REG(0))												\
																	\
		if ((opcode == CMD_DIV || opcode == CMD_MOD) && !T_REG(1))	\
		{															\
			RET (DIV_ZERO_ERR)										\
		}															\
																	\
		PUSH (T_REG(0) op T_REG(1))									\
		MCUR (1)													\
	})


#endif /* MACROS_H_ */
