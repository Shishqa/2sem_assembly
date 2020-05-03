/*
 * run_cpu.cpp
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "ms_stack.h"
#include "cpu.h"

void PrintSyntax ();
void ProceedProcessing (const char* in);

const char* DEFAULT_PATH = "../../source.bin";


int main (int argc, char** argv)
{
    if (argc < 2)
    {
        PrintSyntax ();
        printf ("press Enter to continue with default path [or type something to exit]:\n>>>");

        int n_chars = 0;

        if (!scanf ("%*[^\n]%n", &n_chars) && n_chars == 0)
        {
            ProceedProcessing (DEFAULT_PATH);

            exit (CPU_OK);
        }

        exit (CPU_OK);
    }

    if (argc > 2)
    {
        printf ("error: too many arguments\n\n");
        PrintSyntax ();

        exit (CPU_OK);
    }

    char code_path[PATH_MAX] = {};

    if (!strcmp (argv[1], "--"))
    {
        printf ("please, write path of code to process:\n>>>");
        scanf ("%s", code_path);
    }
    else
    {
        strcpy (code_path, argv[1]);
    }

    ProceedProcessing (code_path);

    return 0;
}


void PrintSyntax ()
{
    printf ("##########################################################################################\n");
    printf ("cpu HELP:\n\n");
    printf ("./cpu {executable}\n");
    printf ("keys:\n");
    printf ("\t--\t\tenter path from keyboard\n");
    printf ("\n");
    printf ("default path:\n");
    printf ("\tin = %s\n", DEFAULT_PATH);
    printf ("##########################################################################################\n");
}


void ProceedProcessing (const char* in)
{
    FILE* in_file = fopen (in, "rb");

    if (in_file == nullptr)
    {
        printf ("error: can't open file %s\n", in);
        exit (INPUT_ERR);
    }

    printf ("%s:\n", in);

    Err_n check = Processor (in_file);

    printf ("\nprocessor returned: %d\n", check);

    fclose (in_file);
}






