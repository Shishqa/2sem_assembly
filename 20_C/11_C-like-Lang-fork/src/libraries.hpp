#ifndef LIBRARIES_HPP_
#define LIBRARIES_HPP_

//#define DEBUG MEOW

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <ctype.h>

#include "ms_stream.h"
#include "tree.h"

#include "Token.hpp"

#define RESET  "\033[0m"
#define RED    "\033[1;31m"
#define YELLOW "\033[3;33m"

#define SETCOLOR(color)             \
        printf ("%s", color)        \

#ifdef DEBUG
#define PRINT( str )                    \
        {                               \
            SETCOLOR (YELLOW);          \
            printf ( str );             \
            SETCOLOR (RESET);           \
        }

#else
#define PRINT( str )          \
        if (0) printf ( str )
#endif // DEBUG

#endif // !LIBRARIES_HPP_
