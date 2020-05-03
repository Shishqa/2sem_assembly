#ifndef TREE_CLASS_H_
#define TREE_CLASS_H_

template <typename elem_t>
struct Node
{
    elem_t data;
    Node<elem_t> * left;
    Node<elem_t> * right;

    Node<elem_t> * parent;

    bool init (const elem_t data);
    bool init ();
    bool clear ();

    void print (FILE * log, const char mode, void (* printer) (FILE *, const void *),
                const char left_brac = '{', const char right_brac = '}', const char sep = '$');
    
    bool printDot (FILE * log, void (* printer) (FILE *, const void *));

    Node<elem_t> * setChild ();

    Node<elem_t> * setLeft  ();
    Node<elem_t> * setRight ();

    Node<elem_t> * addLeft  (const elem_t data);
    Node<elem_t> * addRight (const elem_t data);

    bool search (const elem_t data);
};


template <typename elem_t>
class BinaryTree
{
private:
    size_t size;
    size_t dump_cnt;

public:
    Node<elem_t> * root;

    static Node<elem_t> * setNode (Node<elem_t> ** ptr);

    bool init ();
    bool init (const elem_t data);
    bool init (const char * path, void (* reader) (char **, const void *));
    static char * initSubtree (Node<elem_t> * node, char * cur, void (* reader) (char **, const void *));
    bool clear ();

    bool deleteList (Node<elem_t> * node);
    bool deleteSubtree (Node<elem_t> * node);

    Node<elem_t> * search (const elem_t data);
    Node<elem_t> * rec_search (Node<elem_t> * node, const elem_t data);

    bool valid (Node<elem_t> * node);

    bool print (FILE * log, const char mode, void (* printer) (FILE *, const void *),
                const char left_brac = '{', const char right_brac = '}', const char sep = '$');

    bool dump  (FILE * log, void (* printer) (FILE *, const void *));
    bool dotDump (void (* printer) (FILE *, const void *), int num = 0);
};

#endif // !TREE_CLASS_H_