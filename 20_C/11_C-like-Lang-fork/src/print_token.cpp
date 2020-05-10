#include "libraries.hpp"
#include "Operators.hpp"

void DotTokenPrinter (FILE * out, const void * data)
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

void TokenReader (char ** cur, const void * data)
{
    Token * token = (Token *) data;
    int skip;

    switch (**cur)
    {
        case 'N':
            *cur += 2;
            token->type = NUM_TYPE;
            sscanf (*cur, "%d%n", &(token->data), &skip);
        break;

        case 'I':
            *cur += 2;
            token->type = ID_TYPE;
            sscanf (*cur, "%m[^#]%n", &(token->name), &skip);
            token->data = skip;
            *cur += 1;
        break;
    
        default:
            *cur += 2;
            sscanf (*cur, "%m[^#]%n", &(token->name), &skip);
            for (size_t i = 0; i < OP_CNT; i++)
            {
                if (!strncmp(token->name, operators[i].name, skip))
                {
                    token->type = operators[i].type;
                    token->data = operators[i].opcode;
                }
            }
            *cur += 1;
        break;
    }

    *cur += skip;
}