#include "libraries.hpp"
#include "SetNode.hpp"

Node<Token> * SetNode (char type, int data, const char * name,
                       Node<Token> * left, Node<Token> * right)
{
    Node<Token> * node = (Node<Token> *) calloc (1, sizeof (*node));

    node->init ();

    node->data.type = type;
    node->data.data = data;
    node->data.name = name;

    if (left)
    {
        LINK_L (node, left);
    }

    if (right)
    {
        LINK_R (node, right);
    }

    return (node);
}

