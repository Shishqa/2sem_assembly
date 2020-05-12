#ifndef DECOMPILER_HPP_
#define DECOMPILER_HPP_

#include "libraries.hpp"
#include "Parser.hpp"

// enum Errors
// {
//     TRANSLATOR_OK,
//     FUNC_REDEF,
//     VAR_REDEF,
//     FEW_ARGS,
//     MANY_ARGS,
//     NO_RETURN
// };

class Decompiler
{
    size_t error;

    BinaryTree<Token> * prog;

    FILE * out;
    size_t deep;

    void Proceed (FILE * f);

    void Calculate     (Node<Token> * node);
    void GetOperator   (Node<Token> * node);

    void SetTabs ();

public:
    bool GetCode (BinaryTree<Token> * code, const char * output_path);
};

#endif // !DECOMPILER_HPP_
