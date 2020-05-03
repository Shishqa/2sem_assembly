
#include "libraries.hpp"

#include "Parser.hpp"
#include "Operators.hpp"

#include "Decompiler.hpp"

#include "Translator.hpp"

int main () 
{
    Parser p = {};

    BinaryTree<Token> * my_tree = p.Parse ("programs/fibonacci/main.plan"); 

    assert (my_tree);

    FILE * log = fopen ("saved_tree", "w");
    my_tree->print (log, 'p', TokenPrinter);
    fclose (log);  

    Decompiler d = {};

    d.GetCode (my_tree, "programs/fibonacci/DEC.plan");

    Translator t = {};

    t.BuildAndRun (my_tree, "programs/fibonacci/main.bin");

    return (0); 
}