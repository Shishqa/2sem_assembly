#ifndef NODE_METHODS_H_
#define NODE_METHODS_H_

#include <stdio.h>
#include <stdlib.h>

#include "tree_class.h"

template <typename elem_t>
bool Node<elem_t>::init ()
{
    this->left  = nullptr;
    this->right = nullptr;

    this->parent = nullptr;

    return (true);
}

template <typename elem_t>
bool Node<elem_t>::init (const elem_t data)
{
    this->init ();

    this->data = data;

    return (true);
}

template <typename elem_t>
bool Node<elem_t>::clear ()
{
    this->data = 0;

    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;

    return (true);
}

template <typename elem_t>
void Node<elem_t>::print (FILE * log, const char mode, void (* printer) (FILE *, const void *),
                          const char left_brac, const char right_brac, const char sep)
{
    switch (mode)
    {
        case 'p':
        {
            fprintf (log, "%c", left_brac);

            printer (log, &this->data);

            if (this->left)
            {
                this->left->print (log, mode, printer, left_brac, right_brac, sep);
            }
            else if (this->right)
            {
                fprintf (log, "%c", sep);
            }

            if (this->right)
            {
                this->right->print (log, mode, printer, left_brac, right_brac, sep);
            }

            fprintf (log, "%c", right_brac);
        }
        break;

        case 'i':
        {
            fprintf (log, "%c", left_brac);

            if (this->left)
            {
                this->left->print (log, mode, printer, left_brac, right_brac, sep);
            }

            printer (log, &this->data);

            if (this->right)
            {
                this->right->print (log, mode, printer, left_brac, right_brac, sep);
            }

            fprintf (log, "%c", right_brac);
        }
        break;

        case 's':
        {
            fprintf (log, "%c", left_brac);

            if (this->left)
            {
                this->left->print (log, mode, printer, left_brac, right_brac, sep);
            }
            else 
            {
                fprintf (log, "%c", sep);
            }

            if (this->right)
            {
                this->right->print (log, mode, printer, left_brac, right_brac, sep);
            }
            else 
            {
                fprintf (log, "%c", sep);
            }

            printer (log, &this->data);

            fprintf (log, "%c", right_brac);
        }
        break;
    
    default:
        break;
    }
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::setChild ()
{
    Node<elem_t> * child = (Node<elem_t> *) calloc (1, sizeof (Node<elem_t>));

    child->init ();

    child->parent = this;

    return (child);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::addLeft (const elem_t data)
{
    this->setLeft ();

    this->left->data = data;

    return (this->left);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::setLeft ()
{
    this->left = this->setChild ();

    return (this->left);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::addRight (const elem_t data)
{
    this->setRight ();

    this->right->data = data;

    return (this->right);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::setRight ()
{
    this->right = this->setChild ();

    return (this->right);
}

template <typename elem_t>
bool Node<elem_t>::printDot (FILE * log, void (* printer) (FILE *, const void *))
{
    fprintf (log, "\tnode%p [shape = \"record\", label = \"{<f1>data: ", 
            this);

    printer (log, &this->data);

    fprintf (log, "|{<f2>left: %p|<f3>right: %p}|<f4>parent: %p|<f5>adress: %p}\"];\n", 
            this->left, this->right, this->parent, this);

    if (this->left)
    {
        this->left->printDot (log, printer);

        fprintf (log, "\tnode%p:f2 -> node%p\n", this, this->left);
    }

    if (this->right)
    {
        this->right->printDot (log, printer);

        fprintf (log, "\tnode%p:f3 -> node%p\n", this, this->right);
    }
    
    return (true);
}

#endif // !NODE_METHODS_H_


