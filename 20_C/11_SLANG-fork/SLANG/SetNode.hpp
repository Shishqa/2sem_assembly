#ifndef CREATE_NODE_HPP_
#define CREATE_NODE_HPP_

#include "libraries.hpp"

#define LINK_P( p, n )      \
    if (n)                  \
    {                       \
        (n)->parent = p;    \
    }

#define LINK_L( p, n )          \
        LINK_DIR (p, n, left)   

#define LINK_R( p, n )          \
        LINK_DIR (p, n, right)  

#define LINK_DIR( p, n, dir )       \
        {                           \
            Node<Token> * tmp = n;  \
            p->dir = tmp;           \
            LINK_P (p, tmp);        \
        }

Node<Token> * SetNode (char type, int data, const char * name = nullptr,
                       Node<Token> * left = nullptr, Node<Token> * right = nullptr);

#endif // !CREATE_NODE_HPP_