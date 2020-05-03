/*
 * asm.cpp
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "ms_stream.h"
#include "ms_stack.h"
#include "ms_label.h"
#include "assert_ext.h"
#include "../config/config.h"
#include "../config/cmd_enum.h"
#include "asm.h"


Err_n Compiler(FILE *in, FILE *out)
{
    if (in == nullptr)
    {
        return (INPUT_ERR);
    }

    if (out == nullptr)
    {
        return (OUTPUT_ERR);
    }

    char* in_buffer = nullptr;

    Read (&in_buffer, in);

    char* in_curr = in_buffer;


    char* out_buffer = (char*) calloc (MAX_OUTPUT, sizeof (*out_buffer));
    char* out_curr = out_buffer;


    PutSignature (&out_curr);


    char* code_begin = out_curr;


    // UTILITY
    char   cmd[MAX_CMD_LEN] = "";
    int    ptr 				= 0;
    size_t len        	    = 0;
    size_t s_len            = 0;
    size_t curr_line        = 1;
    Err_n  check			= 0;
    // UTILITY

    Labels labels = {};

    labels.init (LABEL_MAX_CNT, MAX_CALL);

    curr_line += SkipSpaces (&in_curr);


    while (sscanf (in_curr, "%s%n", cmd, &s_len) == 1)
    {
    	if (check != ASM_OK)
    	{
    		return (check);
    	}

		if (cmd[0] == COMMENT_MARKER)
		{
			in_curr = SkipComment (in_curr);
		    curr_line += SkipSpaces (&in_curr);
			continue;
		}

		len = strlen (cmd);
		ptr = out_curr - code_begin;
		in_curr += s_len;

		check = ParseLabel (cmd, len, &labels, ptr);

		if (check == PARSED)
		{
		    curr_line += SkipSpaces (&in_curr);
		    check = ASM_OK;
			continue;
		}
		else if (check != NOT_PARSED)
		{
			return (LABEL_ERR);
		}

		#define DEF_CMD( opcode, name, type, cmd_len, n_args, stack_args, code )       	\
			if (len == cmd_len && !strncmp (cmd, #name, len))  		              	    \
			{                                                              				\
				*out_curr = CMD_##name;                                            		\
				++out_curr;                          						        	\
																						\
				check   = ParseArgs  (&in_curr, &out_curr, n_args, &labels);			\
			    curr_line += SkipSpaces (&in_curr);										\
																						\
				continue;									                			\
			}

			//###########################################
			// CMD_DEF with commands
			//###########################################
			#include "../config/cmd_defines.h"
			//###########################################
			// CMD_DEF with commands
			//###########################################

		#undef DEF_CMD                                                                          											                                                                                      										\

		printf ("error: unexpected command \"%s\" (line %lu)\n", cmd, curr_line);

		free (in_buffer);
		free (out_buffer);

		return (WRONG_CMD_ERR);
    }

    if (!labels.fillPointers())
    {
    	labels.printUndef();
    	return (UNDEF_REFERENCE);
    }

    fwrite (out_buffer, sizeof (char), out_curr - out_buffer, out);

    free (in_buffer);
    free (out_buffer);
    labels.clear();

    return (ASM_OK);
}


void PutSignature (char** out)
{
    Signature file_sign = {};

    memcpy (*out, &file_sign, sizeof (file_sign));

    *out += sizeof (Signature);
}


char* SkipComment (char* curr)
{
    while (*curr != '\n')
    {
		++curr;

		if (*curr == '\0')
		{
			return curr;
		}
    }

    return curr;
}


bool GetSmth (char* curr)
{
    while (*curr != '\n')
    {
		++curr;

		if (isalpha(*curr) || isdigit(*curr))
		{
			return true;
		}
    }

    return false;
}


int IsReg (char* name, size_t len)
{
	for (int i = 0; i < USER_REG_NUM; i++)
	{
		if (len == REG_LEN - 1 && !strncmp (name, reg_names[i], len))
		{
			return (i);
		}
	}

	return (-1);
}


Err_n ParseLabel (char* name, int len, Labels* labels, int ptr)
{
	if (name[len - 1] == ':' && len > 1)
	{
		--len;

		if (labels->add (name, len, ptr))
		{
			return (PARSED);
		}

		return (FAILURE);
	}

	return (NOT_PARSED);
}

Err_n ParseArgs (char** in, char** out, int n_args, Labels* labels)
{
	Argument arg = {};

	char term[MAX_ARG_LEN]  = "";
	char buf[LABEL_MAX_LEN] = "";
	size_t term_len = 0;
	size_t skip_len = 0;

	for (int i = 0; i < n_args; i++)
	{
		arg = {};

		if (SkipSpaces (in) > 0 || sscanf (*in, "%s%n", term, &skip_len) == 0)
		{
			memcpy (*out, &arg, sizeof (Argument));

			*out += sizeof (Argument);

			return (ASM_OK);
		}

		term_len = strlen (term);

		if (term[0] == '[' && term[term_len - 1] == ']')
		{
			arg.mem_param = 1;

			sscanf (*in + 1, "%s", term);

			term[term_len - 1] = 0;

			term_len -= 2;
		}

		*in += skip_len;

		if (sscanf (term, "%[^+]+%d", buf, &arg.val) == 2)
		{
			if ((arg.reg = IsReg (buf, strlen (buf))) == -1)
			{
				return (REG_EXP_ERR);
			}

			arg.reg_param = 1;
			arg.int_param = 1;
		}
		else if (sscanf (term, "%d%n", &arg.val, &skip_len) == 1 && skip_len == term_len)
		{
			arg.int_param = 1;
		}
		else if ((arg.reg = IsReg (term, term_len)) != -1)
		{
			arg.reg_param = 1;
		}
		else
		{
			arg.reg = 0;
			arg.int_param = 1;

			labels->call (term, term_len, *out + 2);
		}

		memcpy (*out, &arg, sizeof (Argument));

		*out += sizeof (Argument);
	}

	return (ASM_OK);
}

