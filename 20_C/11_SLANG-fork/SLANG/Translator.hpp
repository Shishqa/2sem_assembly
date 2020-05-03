#ifndef TRANSLATOR_HPP_
#define TRANSLATOR_HPP_

#include "libraries.hpp"
#include "Parser.hpp"

enum Errors
{
    TRANSLATOR_OK,
    FUNC_REDEF,
    VAR_REDEF,
    UNDEF_VAR,
    UNDEF_FUNC,
    INCORRECT_NUM_OF_ARGS,
    EMPTY_RETURN
};

enum Warnings
{
    MULTIPLE_RETURN
};

struct Variable
{
    const char * id;
    size_t len;
};

struct Function
{
    const char * name;
    size_t name_len;

    int VAR_MAX;
    
    int var_cnt;
    Variable * local_var;
};

class Translator
{
    Parser parser;

    size_t error;
    Node<Token> * problem_node;

    BinaryTree<Token> * prog;

    const int VAR_MAX = 100;

    int var_cnt;
    Variable * global_var;

    const int FUNC_MAX = 100;

    int func_cnt;
    Function * functions;

    Function * curr_func;

    size_t case_cnt;

    size_t if_cnt;
    size_t curr_if;

    size_t curr_cycle;
    size_t cycle_cnt;

    FILE * out;

    void Init (FILE * f);
    void Proceed ();

    void Calculate      (Node<Token> * node);
    void GetAdd         (Node<Token> * node);
    void GetSub         (Node<Token> * node);
    void GetMul         (Node<Token> * node);
    void GetDiv         (Node<Token> * node);
    void DefFunc        (Node<Token> * node);
    void GetCall        (Node<Token> * node);
    void GetAssign      (Node<Token> * node);
    void DefVar         (Node<Token> * node);
    void GetIf          (Node<Token> * node);
    void GetWhile       (Node<Token> * node);
    void GetBlocks      (Node<Token> * node);
    void GetOperators   (Node<Token> * node);
    void GetExpression  (Node<Token> * node);

    int FindVar  (const char * var_id,  const size_t len);
    int FindFunc (const char * func_id, const size_t len);

    void PrintError ();
    void Warn (const size_t warn_code);

public:
    bool GetAsmCode  (BinaryTree <Token> * tree, const char * output_path);
    bool Build       (BinaryTree <Token> * tree, const char * output_path); 
    bool BuildAndRun (BinaryTree <Token> * tree, const char * output_path);
};

#endif // !TRANSLATOR_HPP_