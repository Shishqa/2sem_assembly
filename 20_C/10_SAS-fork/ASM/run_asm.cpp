/*
 * run_asm.cpp
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <math.h>

#include "assert_ext.h"
#include "ms_stack.h"
#include "ms_label.h"
#include "../config/config.h"
#include "../config/cmd_enum.h"
#include "asm.h"

void  PrintSyntax ();
Err_n ProceedCompilation (const char* in, const char* out);

const char* DEFAULT_IN_PATH  = "../../source.asm";
const char* DEFAULT_OUT_PATH = "../../source.bin";

int main (int argc, char** argv)
{
    if (argc < 2)
    {
        PrintSyntax ();
        printf ("press Enter to start with default paths [or type something to exit]:\n>");

        int n_chars = 0;

        if (!scanf ("%*[^\n]%n", &n_chars) && n_chars == 0)
        {
            scanf ("%*c"); // скушать \n

            ProceedCompilation (DEFAULT_IN_PATH, DEFAULT_OUT_PATH);
        }

        return (ASM_OK);
    }

    if (argc != 3)
    {
        printf ("error: invalid number of arguments\n\n");
        PrintSyntax ();

        return (WRONG_CONSOLE_ARGS);
    }

    char in_path[PATH_MAX] = {};

    if (strlen(argv[1]) == 2 && !strncmp (argv[1], "--", 2))
    {
        printf ("please, write path of file with code:\n>");
        scanf ("%s", in_path);
    }
    else
    {
        strncpy (in_path, argv[1], strlen(argv[1]));
    }

    char out_path[PATH_MAX] = {};

    if (strlen(argv[2]) == 2 && !strncmp (argv[2], "--", 2))
    {
        printf ("please, write path to write the compiled code:\n>");
        scanf ("%s", out_path);
    }
    else
    {
    	strncpy (out_path, argv[2], strlen(argv[2]));
    }

    ProceedCompilation (in_path, out_path);

    return (ASM_OK);
}

Err_n ProceedCompilation (const char* in, const char* out)
{
    FILE* file_in  = fopen (in,  "r");

    if (file_in == nullptr)
    {
        printf ("error: can't open file %s\n", in);
        return (INPUT_ERR);
    }

    FILE* file_out = fopen (out, "wb");

    if (file_out == nullptr)
    {
    	fclose (file_in);

        printf ("error: can't open/create file %s\n", out);
        return (OUTPUT_ERR);
    }

    int n_run = 1;
    Err_n check = Compiler (file_in, file_out);

    fclose (file_in);
    fclose (file_out);

    printf ("assembler returned: %d\n", check);
}

void PrintSyntax ()
{
    printf ("======================================================================================\n");
    printf ("asm HELP:\n\n");
    printf ("./asm {from} {to}\n");
    printf ("keys:\n");
    printf ("\t--\t\tenter path from keyboard\n");
    printf ("\n");
    printf ("default paths:\n");
    printf ("\tin = %s\n", DEFAULT_IN_PATH);
    printf ("\tout = %s\n", DEFAULT_OUT_PATH);
    printf ("======================================================================================\n");
}


