#ifndef TOKEN_HPP_
#define TOKEN_HPP_

struct Token
{
    char    type;
    int     data;
    const char * name;
};

enum types
{
    OP_TYPE,
    MATH_TYPE,
    NUM_TYPE,
    ID_TYPE
};

void PrintToken (FILE * out, const void * data);
void TokenPrinter (FILE * out, const void * data);

#endif // !TOKEN_HPP_