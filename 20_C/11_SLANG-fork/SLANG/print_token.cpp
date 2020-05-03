#include "libraries.hpp"
#include "Operators.hpp"

void PrintToken (FILE * out, const void * data)
{
    Token * token = (Token *) data;

    switch (token->type)
    {
        case NUM_TYPE:
            fprintf (out, "%d", token->data);
        break;

        case OP_TYPE:
            fprintf (out, "OP: %s", operators[token->data].name);
        break;

        case MATH_TYPE:
            fprintf (out, "M_OP: %s%s", ((*(operators[token->data].name) == '<' ||
                                          *(operators[token->data].name) == '>' || 
                                          *(operators[token->data].name) == '|') ? "\\" : ""), 
                                          operators[token->data].name);
        break;

        case ID_TYPE:
            fprintf (out, "ID: %s", token->name);
        break;
    
        default:
            fprintf (out, "UNKNOWN_TYPE");
        break;
    }
}

void TokenPrinter (FILE * out, const void * data)
{
    Token * token = (Token *) data;

    switch (token->type)
    {
        case NUM_TYPE:
            fprintf (out, "N:%d", token->data);
        break;

        case ID_TYPE:
            fprintf (out, "I:%s#", token->name);
        break;
    
        default:
            fprintf (out, "O:%s#", operators[token->data].name);
        break;
    }
}