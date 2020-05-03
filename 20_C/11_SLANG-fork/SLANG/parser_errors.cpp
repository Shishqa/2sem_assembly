#include "libraries.hpp"

#include "Parser.hpp"

void Parser::PrintError ()
{
    SETCOLOR (RED);
    printf ("Error: ");
    SETCOLOR (RESET);

    switch (error)
    {
        case NUM_EXPECT:
        printf ("expected number\n");
        break;

        case PRIMARY_EXPECT:
        printf ("expected primary expression\n");
        break;

        case ID_EXPECT:
        printf ("expected name\n");
        break;

        case UNCLOSED_BRACES:
        printf ("unclosed braces\n");
        break;

        case NO_BLOCK:
        printf ("expected definition of function\n");
        break;

        case COND_EXPECT:
        printf ("expected condition\n");
        break;

        case ARG_EXPECT:
        printf ("expected arguments\n");
        break;
    
        default:
        printf ("everything is OK, but developer of this code is bad\n");
        break;
    }

    SETCOLOR (RED);
    printf ("Token[%lu] : %d %d %s", cur_token,
                                     tokens[cur_token]->data.type,
                                     tokens[cur_token]->data.data,
                                     tokens[cur_token]->data.name);
    SETCOLOR (RESET);

    printf ("\n\n");
}
