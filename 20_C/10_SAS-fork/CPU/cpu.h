/*
 * cpu.h
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef CPU_H_
#define CPU_H_

enum
{
    CPU_OK              = 0,
    NO_ARG_ERR          = 1,
    FEW_STK_ARGS_ERR    = 2,
    DIV_ZERO_ERR        = 3,
    SQRT_NEGATIVE_ERR   = 4,
	POW_NEGATIVE_ERR	= 5,
    WRONG_CMD_ERR       = 6,
    WRONG_ARG_TYPE_ERR  = 7,
    INIT_ERR            = 8,
    STACK_ERR           = 9,
    FORMAT_ERR          = 10,
    NO_END_ERR          = 11,
    INPUT_ERR	        = 12,
	OUT_ERR				= 13,
	IN_ERR				= 14,
};

Err_n Processor (FILE* stream);

int BinaryPow (int x, int a);


#endif /* CPU_H_ */
