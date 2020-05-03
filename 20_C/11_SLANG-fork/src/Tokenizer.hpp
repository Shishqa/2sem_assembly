#ifndef TOKENIZER_HPP_
#define TOKENIZER_HPP_

#include "libraries.hpp"

enum Tokenizer_errors
{
    TOKENIZER_OK,
    UNKNOWN_OPERATOR
};

class Tokenizer
{
    size_t error;

    Node<Token> ** tokens;
    const size_t MAX_TOKENS = 10000;
    size_t n_tokens;

    const size_t MAX_NAME_LEN = 100;

    const char * cur;
    size_t line;

    void Proceed ();
    void SkipSpaces ();

    void ParseNum ();
    void ParseId ();
    void ParseOp ();

    void PrintError ();

    public:

    Node<Token> ** tokenize (const char * str);
};

#endif // !TOKENIZER_HPP_