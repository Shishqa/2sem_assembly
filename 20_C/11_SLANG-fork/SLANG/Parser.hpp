#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "libraries.hpp"

//YACC Bizon

enum Parser_errors
{
    PARSER_OK,
    DEF_EXPECT,
    COND_EXPECT,
    NUM_EXPECT,
    PRIMARY_EXPECT,
    ID_EXPECT,
    ASSIGN_EXPECT,
    ARG_EXPECT,
    UNCLOSED_BRACES,
    NO_BLOCK
};

class Parser 
{
    BinaryTree<Token> * program_parsed;

    Node<Token> ** tokens;
    size_t cur_token;

    size_t error;

    Node<Token> * ParseGrammar ();

    Node<Token> * ParseDefinition ();

    Node<Token> * ParseDef     ();
    Node<Token> * ParseVar     ();
    Node<Token> * ParseVarList ();

    Node<Token> * ParseBlock  ();
    Node<Token> * ParseOp     ();
    Node<Token> * ParseIf     ();
    Node<Token> * ParseWhile  ();

    Node<Token> * ParseSequence ();
    Node<Token> * ParseArgs     ();

    Node<Token> * ParseExpression ();
    Node<Token> * ParseAssignment ();
    
    Node<Token> * ParseAnd         ();
    Node<Token> * ParseOr          ();
    Node<Token> * ParseBool        ();

    Node<Token> * ParseAddSub      ();
    Node<Token> * ParseMulDiv      ();
    Node<Token> * ParsePrimary     ();

    Node<Token> * ParseCall   ();
    Node<Token> * ParseId     ();

    void PrintError ();

public:

    BinaryTree<Token> * ParseStr (const char * str);
    BinaryTree<Token> * Parse (const char * path);
};

#endif // !PARSER_HPP_