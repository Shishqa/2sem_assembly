/*
 * ms_stack.h
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef MS_STACK_H_
#define MS_STACK_H_

const bool STACK_SILENT = true;

#define DEBUG meow0
#ifdef DEBUG
    #define ASSERT_OK( stk )                                                          \
    {                                                                                 \
        if (((stk)->err = StackValid ( stk )) != OK_CODE && !STACK_SILENT)            \
        {                                                                             \
            StackDump ((stk), "Assertion", __FILE__, __PRETTY_FUNCTION__, __LINE__);  \
            abort();                                                                  \
        }                                                                             \
    }
    #define ON_DEBUG( code ) code
#else
    #define ASSERT_OK( stk )  ;
    #define ON_DEBUG( code )  ;
#endif // DEBUG


//#undef HASHING
#define HASHING meow1
#ifdef HASHING
    #define IF_HASHING( code ) code
    #define $( smth ) smth
#else
    #define IF_HASHING( code ) ;
    #define $( smth ) 0
#endif // HASHING


//#undef CANARIES
#define CANARIES meow2
#ifdef CANARIES
    #define CANARY( code ) code
    #define $$( smth ) smth
#else
    #define CANARY( code ) ;
    #define $$( smth ) 0
#endif // CANARIES


#define StackConstruct( stk )                  \
{                                              \
    Stack_Construct (stk, (char*)(#stk) + 1);  \
}

#define StackArrConstruct( stk_arr, num, names_arr )         \
{                                                            \
    for (int i = 0; i < num; ++i)                            \
    {                                                        \
        Stack_Construct (stk_arr[i], names_arr[i]);          \
    }                                                        \
}


#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <stdint.h>

typedef int Elem_t;
typedef int Err_n;

//constants

const size_t START_STACK_SIZE = 4;
const size_t MAX_NAME_LENGTH = 20;

CANARY ( const long long CANARY_VAL   = 6660666; )
CANARY ( const Elem_t CANARY_ELEM_VAL = 9990999; )

IF_HASHING ( const int POLY_HASHING_MULT = 3; )

const char POISON_BYTE  = 255;
const Elem_t POISON_VAL = -666;

const int RESIZE_GISTERESIS_RATIO = 4; // ratio of memory size to the number of
                                       // elements in the stack to resize it down

const int MEM_RESIZE_RATIO        = 2; // memory increase or decrease ratio

const Err_n DESTRUCTED_ERR_CODE = -1; // Stack is destructed
const Err_n OK_CODE             = 0;  // Stack is OK
const Err_n SIZE_ERR_CODE       = 1;  // Size is greater than mem_size
const Err_n MEM_SIZE_ERR_CODE   = 2;  // Mem_size didn't decreased when it was needed
const Err_n NON_INIT_ERR_CODE   = 3;  // Data is not-initialized
const Err_n EMPTY_ERR_CODE      = 4;  // Stack is empty but user asked for value
const Err_n UNDEFINED_ERR_CODE  = 5;  // Stack is empty but user asked for value

CANARY ( const Err_n CANARY_ERR_CODE   = 6; ) // Canary died :(
IF_HASHING ( const Err_n HASH_ERR_CODE = 7; ) // The hash of the current state of the stack
                                              // is different from the recorded hash

#pragma pack (1)
struct Stack_t
{
    CANARY ( const long long canary_left  = CANARY_VAL; )

    size_t size;
    size_t mem_size;
    char name[MAX_NAME_LENGTH];
    Elem_t* data;
    Err_n err;
    bool is_empty;
    IF_HASHING ( int str_hash; )
    IF_HASHING ( int data_hash; )

    CANARY ( const long long canary_right = CANARY_VAL; )
};
#pragma pack ()

// Stack_t methods

Err_n Stack_Construct (Stack_t* this_, const char* init_name = "undef", size_t init_size = START_STACK_SIZE);
Err_n StackDestruct (Stack_t* this_);
Err_n StackRealloc (Stack_t* this_, size_t new_size);
Err_n StackPush (Stack_t* this_, Elem_t value);
Err_n StackTop  (Stack_t* this_, Elem_t* rec);
Err_n StackPop  (Stack_t* this_, Elem_t* rec = nullptr);
Err_n StackValid (Stack_t* this_);
bool IsStackDestructed (Stack_t* this_);
void StackDump (Stack_t* this_, const char* reason = "Undefined Dump", const char* active_file = "\"undefined file\"", const char* active_func = "\"undefined function\"", const int active_line = -1);

// Hashing

void StackHash (Stack_t* this_, unsigned int (*ByteHasher)(void*, size_t));
bool CheckHash (Stack_t* this_, unsigned int (*ByteHasher)(void*, size_t));
unsigned int GetPolyHash (void* buffer, size_t num);
unsigned int GetHash (void* buffer, size_t num); // обертка
uint32_t GetMurMurHash2 (const uint8_t* key, size_t len);

// Canaries

bool CheckCanaries (Stack_t* this_);


#endif /* MS_STACK_H_ */
