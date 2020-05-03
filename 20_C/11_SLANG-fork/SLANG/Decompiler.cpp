#include "libraries.hpp"
#include "Parser.hpp"
#include "Decompiler.hpp"
#include "Operators.hpp"

#define N node
#define L node->left
#define R node->right

#define STOP( err_code )                    \
    {                                       \
        error = err_code;                   \
        return;                             \
    }                                   

bool Decompiler::GetCode (BinaryTree<Token> * code, const char * output_path)
{
    FILE * f = fopen (output_path, "w");

    fprintf (f, "%% ##      THIS FILE IS GENERATED AUTOMATICALLY      ##\n");
    fprintf (f, "%% ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##\n");

    deep = 0;
    prog = code;

    Proceed (f);
    
    fclose (f);

    return (true);
}

void Decompiler::Proceed (FILE * f)
{
    out = f;

    Calculate (prog->root);
}

void Decompiler::Calculate (Node<Token> * node)
{
    if (!node)
    {
        return;
    }

    switch (node->data.type)
    {
        case NUM_TYPE:
            fprintf (out, "%d", node->data.data);
        break;

        case ID_TYPE:
            fprintf (out, "%s", node->data.name);
        break;

        default:
            GetOperator (node);
        break;
    }
}

void Decompiler::GetOperator (Node<Token> * node)
{
    if (node->data.type == MATH_TYPE && 
        operators[node->data.data].priority < 
        operators[node->parent->data.data].priority)
    {
        fprintf (out, "(");
    }

    switch (node->data.data)
    {
        case NOT:
            fprintf (out, "!");
            Calculate (node->right);
        break;

        case DEF_FUNC:
            fprintf (out, "def ");
            Calculate (node->right);
            fprintf (out, " (");
            Calculate (node->left);
            fprintf (out, ")\n");
            Calculate (node->right->right);
            fprintf (out, "\n");
        break;

        case DEF_VAR:
            fprintf (out, "var ");
            Calculate (node->left);
            if (node->right)
            {
                fprintf (out, " = ");
                Calculate (node->right);
            }
        break;

        case CALL:
            Calculate (node->right);
            fprintf (out, " (");
            Calculate (node->left);
            fprintf (out, ")");
        break;

        case RET:
            fprintf (out, "return (");
            Calculate (node->left);
            fprintf (out, ")");
        break;

        case IF:
            fprintf (out, "if (");
            Calculate (node->left);
            fprintf (out, ")\n");
            Calculate (node->right);
        break;

        case WHILE:
            fprintf (out, "while (");
            Calculate (node->left);
            fprintf (out, ")\n");
            Calculate (node->right);
        break;

        case DEF:
            Calculate (node->left);
            fprintf (out, "\n");
            Calculate (node->right);
        break;

        case LINK:
            Calculate (node->right);
            if (node->left)
            {
                fprintf (out, "\n");
                SetTabs ();
                fprintf (out, "else\n");
                Calculate (node->left);
            }
        break;

        case OPERATOR:
            Calculate (node->left);
            SetTabs ();
            Calculate (node->right);
            fprintf (out, "\n");
        break;

        case BLOCK:
            SetTabs ();
            fprintf (out, "{\n");
            deep++;
            Calculate (node->right);
            deep--;
            SetTabs ();
            fprintf (out, "}");
        break;

        case COMMA:
            Calculate (node->right);
            if (node->left)
            {
                fprintf (out, ", ");
                Calculate (node->left);
            }
        break;

        default:
            Calculate (node->left);
            fprintf (out, " %s ", operators[node->data.data].name);
            Calculate (node->right);
        break;
    }

    if (node->data.type == MATH_TYPE && 
        operators[node->data.data].priority < 
        operators[node->parent->data.data].priority)
    {
        fprintf (out, ")");
    }
}

void Decompiler::SetTabs ()
{
    for (size_t i = 0; i < deep; i++)
    {
        fprintf (out, "\t");
    }
}
