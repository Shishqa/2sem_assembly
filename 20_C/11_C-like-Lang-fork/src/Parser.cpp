#include "libraries.hpp"

#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "SetNode.hpp"
#include "Operators.hpp"

#define STOP( err_code, node )                          \
        {                                               \
            PRINT("ERROR\n");                           \
                                                        \
            if (node)                                   \
            {                                           \
                program_parsed->deleteSubtree (node);   \
            }                                           \
                                                        \
            error = (err_code);                         \
            return (nullptr);                           \
        }                                       

#define CHECK( node, to_del )             \
        if (!node)                        \
        {                                 \
            STOP( error, to_del );        \
        }                       

#define DATA( id )              \
        tokens[id]->data.data  

#define TYPE( id )              \
        tokens[id]->data.type

#define IS_OP( type, opcode )           \
        (TYPE (cur_token) == type &&    \
         DATA (cur_token) == opcode)    \


BinaryTree<Token> * Parser::Parse (const char * path)
{
    FILE * f = fopen (path, "r");

    if (!f)
    {
        printf ("Can't open file: %s\n", path);
        return (nullptr);
    }

    char * str = nullptr;

    Read (&str, f);

    fclose (f);

    BinaryTree<Token> * res = ParseStr (str);

    free (str);

    return (res);
}

BinaryTree<Token> * Parser::ParseStr (const char * str)
{
    Tokenizer t = {};
    tokens = t.tokenize (str);

    if (tokens == nullptr)
    {
        SETCOLOR (RED);
        printf ("Compilation failed\n");
        SETCOLOR (RESET);
        return (nullptr);
    }

    cur_token = 0;

    error = PARSER_OK;

    program_parsed = (BinaryTree<Token> *) calloc (1, sizeof (*program_parsed));
    program_parsed->init();

    program_parsed->root = ParseGrammar ();

    if (!program_parsed->root || error != PARSER_OK)
    {
        PrintError ();
        SETCOLOR (RED);
        printf ("Compilation failed\n");
        SETCOLOR (RESET);
        return (nullptr);
    }

    program_parsed->dotDump (DotTokenPrinter, 1);

    return (program_parsed);
}

/*
    Grammar :: GlobalBlock* '\0'
*/
Node<Token> * Parser::ParseGrammar ()
{
    PRINT ("grammar\n");

    Node<Token> * res = SetNode (OP_TYPE, DEF, nullptr, nullptr, ParseDefinition ());
    CHECK (res->right, res);

    while (tokens[cur_token])
    {
        res = SetNode (OP_TYPE, DEF, nullptr, res);

        LINK_R (res, ParseDefinition ());
        CHECK (res->right, res);
    }

    return (res);
}

/*
    GlobalBlock :: Func | Assignment
*/
Node<Token> * Parser::ParseDefinition ()
{
    PRINT ("definition ");

    if (IS_OP (OP_TYPE, DEF_FUNC))
    {
        PRINT ("-> def_func\n");
        return (ParseDef ());
    }

    PRINT ("-> var\n");
    return (ParseVar ());
}

/*
    Func :: "def" Id Args Block
*/
Node<Token> * Parser::ParseDef ()
{
    PRINT ("def_func\n");

    Node<Token> * res = nullptr;

    if (IS_OP (OP_TYPE, DEF_FUNC))
    {
        res = tokens[cur_token++];

        LINK_R (res, ParseId ());
        CHECK (res->right, res);

        if (!IS_OP (MATH_TYPE, LEFT_B))
        {
            STOP (ID_EXPECT, res);
        }
        cur_token++;

        if (!IS_OP (MATH_TYPE, RIGHT_B))
        {
            LINK_L (res, ParseVarList ());
            CHECK (res->left, res);

            if (!IS_OP (MATH_TYPE, RIGHT_B))
            {
                STOP (UNCLOSED_BRACES, res);
            }
        }
        cur_token++;

        LINK_R (res->right, ParseBlock ());
        CHECK (res->right->right, res);

        return (res);
    }

    STOP (DEF_EXPECT, res);
}

Node<Token> * Parser::ParseVarList ()
{
    PRINT ("varlist\n");

    Node<Token> * res = SetNode (MATH_TYPE, COMMA, nullptr, nullptr, ParseVar ());
    CHECK (res->right, res);

    if (IS_OP (MATH_TYPE, COMMA))
    {
        cur_token++;
        LINK_L (res, ParseVarList ());
        CHECK  (res->left, res);
    }

    return (res);
}

/*
    Var :: "var" Id
*/
Node<Token> * Parser::ParseVar ()
{
    PRINT ("var\n");

    Node<Token> * res = nullptr;

    if (IS_OP (OP_TYPE, DEF_VAR))
    {
        res = tokens[cur_token++];

        LINK_L (res, ParseId ());
        CHECK  (res->left, res);

        if (IS_OP (MATH_TYPE, ASSIGN))
        {
            cur_token++;
            LINK_R (res, ParseExpression ());
            CHECK  (res->right, res);
        }

        return (res);
    }

    STOP (ID_EXPECT, res);
}

/*
    Block :: '{' Op* '}'
*/
Node<Token> * Parser::ParseBlock ()
{
    PRINT ("block\n");

    Node<Token> * res = nullptr;

    if (!IS_OP (OP_TYPE, LEFT_BL))
    {
        STOP (NO_BLOCK, res);
    }
    cur_token++;

    if (IS_OP (OP_TYPE, RIGHT_BL))
    {
        cur_token++;
        return (SetNode (OP_TYPE, BLOCK));
    }

    while (!IS_OP (OP_TYPE, RIGHT_BL))
    {
        res = SetNode (OP_TYPE, OPERATOR, nullptr, res);

        LINK_R (res, ParseOp ());
        CHECK  (res->right, res);
    }
    cur_token++;

    PRINT ("block <- }\n");

    //return (res);
    return (SetNode (OP_TYPE, BLOCK, nullptr, nullptr, res));
}

/*
    Assignment :: Expression = Expression';' || Expression';'
*/
Node<Token> * Parser::ParseAssignment ()
{
    PRINT ("assign\n");

    Node<Token> * res   = ParseId ();

    CHECK (res, res);

    if (IS_OP (MATH_TYPE, ASSIGN))
    {
        PRINT ("=\n");

        LINK_L (tokens[cur_token], res);
        res = tokens[cur_token++];

        LINK_R (res, ParseExpression ());
        CHECK (res->right, res);
    }

    return (res);
}

/*
    Op :: Block | CondOp | Assignment
*/
Node<Token> * Parser::ParseOp ()
{
    PRINT ("op\n");

    Node<Token> * res = nullptr;

    if (IS_OP (OP_TYPE, IF))
    {
        res = ParseIf ();
    }
    else if (IS_OP (OP_TYPE, WHILE))
    {
        res = ParseWhile ();
    }
    else if (IS_OP (OP_TYPE, DEF_VAR))
    {
        res = ParseVar ();
    }
    else if (TYPE (cur_token) == ID_TYPE && TYPE (cur_token + 1) == MATH_TYPE && 
                                            DATA (cur_token + 1) == LEFT_B)
    {
        res = ParseCall ();
    }
    else if (IS_OP (OP_TYPE, RET) || IS_OP (OP_TYPE, PRNT))
    {
        res = ParseCall ();
    }
    else
    {
        res = ParseAssignment ();
    }

    CHECK (res, res);

    return (res);
}

/*
    CondOp :: "while" '(' Expression ')' Block
*/
Node<Token> * Parser::ParseWhile ()
{
    PRINT ("while\n");

    Node<Token> * res  = nullptr;

    if (IS_OP (OP_TYPE, WHILE))
    {
        res = tokens[cur_token++];

        if (!IS_OP (MATH_TYPE, LEFT_B))
        {
            STOP (COND_EXPECT, res);
        }
        cur_token++;

        LINK_L (res, ParseExpression ());
        CHECK (res->left, res);

        if (!IS_OP (MATH_TYPE, RIGHT_B))
        {
            STOP (UNCLOSED_BRACES, res);
        }
        cur_token++;

        LINK_R (res, ParseBlock ());
        CHECK (res->right, res);

        return (res);
    }

    STOP (error, res);
}

Node<Token> * Parser::ParseIf ()
{
    PRINT ("if\n");

    Node<Token> * res = nullptr;

    if (IS_OP (OP_TYPE, IF))
    {
        res = tokens[cur_token++];

        if (!IS_OP (MATH_TYPE, LEFT_B))
        {
            STOP (COND_EXPECT, res);
        }
        cur_token++;

        LINK_L (res, ParseExpression ());
        CHECK (res->left, res);

        if (!IS_OP (MATH_TYPE, RIGHT_B))
        {
            STOP (UNCLOSED_BRACES, res);
        }
        cur_token++;

        LINK_R (res, SetNode (OP_TYPE, LINK));

        LINK_R (res->right, ParseBlock ());
        CHECK (res->right->right, res);

        if (IS_OP (OP_TYPE, ELSE))
        {
            cur_token++;
            LINK_L(res->right, ParseBlock ());
            CHECK (res->right->left, res);
        }

        return (res);
    }

    STOP (error, res);
}

/*
    Expression :: Or ('&' Or)*
*/
Node<Token> * Parser::ParseExpression ()
{
    PRINT ("(expression) and\n");

    Node<Token> * res = ParseOr ();
    CHECK (res, res);

    while (IS_OP (MATH_TYPE, AND))
    {
        LINK_L (tokens[cur_token], res);
        res = tokens[cur_token++];

        LINK_R (res, ParseOr ());
        CHECK (res->right, res);
    }

    return (res);
}

/*
    Or :: PrimaryBool '|' PrimaryBool+
*/
Node<Token> * Parser::ParseOr ()
{
    PRINT ("or\n");

    Node<Token> * res = ParseBool ();
    CHECK (res, res);

    while (IS_OP (MATH_TYPE, OR))
    {
        LINK_L (tokens[cur_token], res);
        res = tokens[cur_token++];

        LINK_R (res, ParseBool ());
        CHECK (res->right, res);
    }

    return (res);
}

#define CHECK_BOOL( opcode )                            \
        if (IS_OP (MATH_TYPE, opcode))                  \
        {                                               \
            PRINT ("bool -> ");                         \
            PRINT (operators[opcode].name);             \
            PRINT ("\n");                               \
            LINK_L (tokens[cur_token], res);            \
            res = tokens[cur_token++];                  \
                                                        \
            LINK_R (res, ParseAddSub ());               \
            CHECK (res->right, res);                    \
                                                        \
            return (res);                               \
        }

/*
    Bool :: AddSub {<,>,==,<=,>=,!=} AddSub | AddSub
*/
Node<Token> * Parser::ParseBool ()
{
    PRINT ("bool\n");

    Node<Token> * res = ParseAddSub ();
    CHECK (res, res);

    CHECK_BOOL (GEQ);
    CHECK_BOOL (LEQ);
    CHECK_BOOL (EQ);
    CHECK_BOOL (NEQ);
    CHECK_BOOL (GRTR);
    CHECK_BOOL (LESS);

    return (res);
}

/*
    AddSub :: MulDiv {+,-} MulDiv
*/
Node<Token> * Parser::ParseAddSub ()
{
    PRINT ("add/sub\n");

    Node<Token> * res = ParseMulDiv ();
    CHECK (res, res);

    while (IS_OP (MATH_TYPE, ADD) || 
           IS_OP (MATH_TYPE, SUB))
    {
        LINK_L (tokens[cur_token], res);
        res = tokens[cur_token++];

        LINK_R (res, ParseMulDiv ());
        CHECK (res->right, res);
    }

    return (res);
}

/*
    MulDiv :: Primary {*,/} Primary
*/
Node<Token> * Parser::ParseMulDiv ()
{
    PRINT ("muldiv\n");

    Node<Token> * res = ParsePrimary ();
    CHECK (res, res);

    while (IS_OP (MATH_TYPE, MUL) || 
           IS_OP (MATH_TYPE, DIV) ||
           IS_OP (MATH_TYPE, MOD))
    {
        LINK_L (tokens[cur_token], res);
        res = tokens[cur_token++];

        LINK_R (res, ParsePrimary ());
        CHECK (res->right, res);
    }

    return (res);
}

/*
    Primary :: Id Args | Var 
*/
Node<Token> * Parser::ParsePrimary ()
{
    PRINT ("primary ");

    Node<Token> * res  = nullptr;

    if (TYPE (cur_token) == NUM_TYPE)
    {
        PRINT ("-> num\n");
        res = tokens[cur_token++];
    }
    else if (IS_OP (MATH_TYPE, LEFT_B))
    {
        PRINT ("-> (\n");
        cur_token++;

        res = ParseExpression ();

        if (!IS_OP (MATH_TYPE, RIGHT_B))
        {
            STOP (UNCLOSED_BRACES, res);
        }
        cur_token++;

        PRINT ("primary <- )\n");
    }
    else if (IS_OP (MATH_TYPE, NOT))
    {
        PRINT ("-> NOT\n");
        res = tokens[cur_token++];

        LINK_R (res, ParseExpression ());
        CHECK (res->right, res);
    }
    else if (TYPE (cur_token) == ID_TYPE)
    {
        if (TYPE (cur_token + 1) == MATH_TYPE && DATA (cur_token + 1) == LEFT_B)
        {
            PRINT ("-> CALL\n");
            res = ParseCall ();
        }
        else
        {
            PRINT ("-> ID\n");
            res = ParseId ();
        }
    }
    else
    {
        STOP (PRIMARY_EXPECT, nullptr);
    }

    CHECK (res, res);
    return (res);
}

Node<Token> * Parser::ParseCall ()
{
    PRINT ("call");

    Node<Token> * res = nullptr;

    if (IS_OP (OP_TYPE, RET) || IS_OP (OP_TYPE, PRNT))
    {
        PRINT ("==RET\n");
        res = tokens[cur_token++];
    }
    else
    {
        PRINT ("\n");
        res = SetNode (OP_TYPE, CALL);
        LINK_R (res, ParseId ());
    }

    if (!IS_OP (MATH_TYPE, LEFT_B))
    {
        STOP (ARG_EXPECT, res);
    }
    cur_token++;

    if (!IS_OP (MATH_TYPE, RIGHT_B))
    {
        LINK_L (res, ParseSequence ());
        CHECK (res->left, res);   
    }

    if (!IS_OP (MATH_TYPE, RIGHT_B))
    {
        STOP (UNCLOSED_BRACES, res);
    }
    cur_token++;

    return (res);
}

/*
    Sequence :: Expression ',' Expression+
*/
Node<Token> * Parser::ParseSequence ()
{
    PRINT ("seq\n");

    Node<Token> * res = SetNode (MATH_TYPE, COMMA, nullptr, nullptr, ParseExpression ());
    CHECK (res->right, res);

    if (IS_OP (MATH_TYPE, COMMA))
    {
        cur_token++;
        LINK_L (res, ParseSequence ());
        CHECK  (res->left, res);
    }

    PRINT ("seq <-\n");

    return (res);
}

Node<Token> * Parser::ParseId ()
{
    PRINT (tokens[cur_token]->data.name);
    PRINT ("\n");

    if (TYPE (cur_token) != ID_TYPE)
    {
        STOP (ID_EXPECT, nullptr);
    }

    return (tokens[cur_token++]);
}