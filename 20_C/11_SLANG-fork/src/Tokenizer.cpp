#include "libraries.hpp"
#include "Tokenizer.hpp"
#include "SetNode.hpp"
#include "Operators.hpp"

Node<Token> ** Tokenizer::tokenize (const char * str)
{
    error = TOKENIZER_OK;

    cur = str;
    line = 1;

    tokens = (Node<Token> **) calloc (MAX_TOKENS, sizeof (*tokens));
    n_tokens = 0;

    Proceed ();

    if (error != TOKENIZER_OK)
    {
        PrintError ();
        free (tokens);
        return (nullptr);
    }

    return (tokens);
}

void Tokenizer::PrintError ()
{
    SETCOLOR (RED);
    printf ("Error: ");
    SETCOLOR (RESET);

    switch (error)
    {
        case UNKNOWN_OPERATOR:
            printf ("unknown operator");
        break;
    
        default:
            printf ("strange error");
        break;
    }

    printf ("\nLine %lu: ", line);
    SETCOLOR (RED);
    printf ("%c (%d)", *cur, *cur);
    SETCOLOR (RESET);
    printf ("\n");
}

void Tokenizer::Proceed ()
{
    SkipSpaces ();

    while (*cur)
    {
        if (*cur == '%')
        {
            while (*cur != '\n' && *cur != '\0')
            {
                cur++;
            }
            SkipSpaces ();
        }
        else if (isdigit (*cur))
        {
            ParseNum ();
        }
        else if (isalpha (*cur))
        {
            ParseId ();
        }
        else
        {
            ParseOp ();
        }

        if (error != TOKENIZER_OK)
        {
            return;
        }

        SkipSpaces ();
    }
}

void Tokenizer::ParseNum ()
{
    int num = 0;
    size_t num_len = 0;

    sscanf (cur, "%d%n", &num, &num_len);

    tokens[n_tokens++] = SetNode (NUM_TYPE, num);

    cur += num_len;
}

void Tokenizer::ParseId ()
{
    char * name = (char *) calloc (MAX_NAME_LEN, sizeof (*name));
    int name_len = 0;

    bool is_operator = false;

    sscanf (cur, "%[a-zA-Z0-9_]%n", name, &name_len);

    for (size_t i = 0; i < OP_CNT; i++)
    {
        if (name_len == operators[i].name_len &&
            !strncmp (name, operators[i].name, name_len))
        {
            tokens[n_tokens++] = SetNode (operators[i].type, operators[i].opcode);

            is_operator = true;
            break;
        }
    }

    if (!is_operator)
    {
        tokens[n_tokens++] = SetNode (ID_TYPE, name_len, name);
    }

    cur += name_len;
}

void Tokenizer::ParseOp ()
{
    for (size_t i = 0; i < OP_CNT; i++)
    {
        if (!strncmp (cur, operators[i].name, operators[i].name_len))
        {
            tokens[n_tokens++] = SetNode (operators[i].type, operators[i].opcode);

            assert (operators[i].name_len > 0); 

            cur += operators[i].name_len;
            return;
        }
    }

    error = UNKNOWN_OPERATOR;
}

void Tokenizer::SkipSpaces ()
{
    while (*cur == ' ' || *cur == '\n' || *cur == '\t' || *cur == '\r')
    {
        if (*cur == '\n')
        {
            line++;
        }

        cur++;
    }
}