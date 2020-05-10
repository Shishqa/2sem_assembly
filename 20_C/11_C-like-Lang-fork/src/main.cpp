
#include "libraries.hpp"

#include "Parser.hpp"
#include "Operators.hpp"

#include "Decompiler.hpp"

#include "Translator.hpp"

int main (int argc, char ** argv) 
{
    if (argc <= 1)
    {
        printf ("not enough arguments, type:\n");
        printf ("./plan <filename> <flags>\n");
        return (0);
    }

    char * path = argv[1];

    if (argc > 2 && !strncmp (argv[2], "-do", 3))
    {
        BinaryTree<Token> my_tree = {};
        my_tree.init (path, TokenReader, '{', '}', '@');

        my_tree.dotDump (DotTokenPrinter, 444);

        Decompiler d = {};

        d.GetCode (&my_tree, "DECOMPILED.plan");
        return (0);
    }
    else if (argc > 2 && !strncmp (argv[2], "-d", 2))
    {
        BinaryTree<Token> my_tree = {};
        my_tree.init (path, TokenReader, '{', '}', '@');

        my_tree.dotDump (DotTokenPrinter, 444);

        Decompiler d = {};

        d.GetCode (&my_tree, "DECOMPILED.plan");
        
        Translator t = {};

        t.BuildAndRun (&my_tree, "main.bin");
        return (0);
    }
    else
    {
        Parser p = {};

        BinaryTree<Token> * my_tree = p.Parse (path); 

        assert (my_tree);
        printf ("parsed\n");

        FILE * log = fopen ("saved_tree.txt", "w");
        my_tree->print (log, 'p', TokenPrinter, '{', '}', '@');
        fclose (log);  

        printf ("printed\n");

        if (argc > 2 && !strncmp (argv[2], "-f", 2))
        {
            return (0);
        }

        Decompiler d = {};

        d.GetCode (my_tree, "DECOMPILED.plan");

        Translator t = {};

        t.BuildAndRun (my_tree, "main.bin");
    }

    return (0); 
}
