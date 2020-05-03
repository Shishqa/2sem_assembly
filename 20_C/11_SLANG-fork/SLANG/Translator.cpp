#include "libraries.hpp"
#include "Parser.hpp"
#include "Translator.hpp"

#include "Operators.hpp"

#define N node
#define L node->left
#define R node->right

#define STOP( err_code )                    \
    {                                       \
        error = err_code;                   \
        return;                             \
    }      

#define TYPE( node )      \
    (node)->data.type       

#define DATA( node )      \
    (node)->data.data       

#define IS_OP( node, t, opcode )                                \
    ((node)->data.type == t && (node)->data.data == opcode)

bool Translator::BuildAndRun (BinaryTree<Token> * tree, const char * output_path)
{
    if (!Build (tree, output_path))
    {
        return (false);
    }

    char call_cpu[200] = "";

    sprintf (call_cpu, "./bin/cpu %s", output_path);

    system (call_cpu);
}

bool Translator::Build (BinaryTree <Token> * tree, const char * output_path)
{
    const char * tmp_output = "programs/tmp.asm";

    if (!GetAsmCode (tree, tmp_output))
    {
        return (false);
    }

    char call_asm[200] = "";

    sprintf (call_asm, "./bin/asm %s %s", tmp_output, output_path);

    system (call_asm);

    return (true);
}

bool Translator::GetAsmCode (BinaryTree <Token> * tree, const char * output_path)
{
    prog = tree;

    if (!prog)
    {
        return (false);
    }

    prog->dotDump (PrintToken, 2);

    FILE * f = fopen (output_path, "w");

    Init (f);

    fprintf (out, "; ##      THIS FILE IS GENERATED AUTOMATICALLY      ##\n");
    fprintf (out, "; ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##\n");

    Proceed ();
    
    fclose (f);

    if (error != TRANSLATOR_OK)
    {
        PrintError ();
        return (false);
    }

    return (true);
}

void Translator::Init (FILE * f)
{
    out = f;

    global_var = (Variable *) calloc (VAR_MAX, sizeof (*global_var));
    var_cnt = 1;

    functions = (Function *) calloc (FUNC_MAX, sizeof (*functions));
    func_cnt = 1;

    curr_func = nullptr;

    case_cnt  = 0;

    if_cnt  = 0;
    curr_if = 0;

    cycle_cnt  = 0;
    curr_cycle = 0;
}

void Translator::Proceed ()
{
    fprintf (out, "\tCALL\tfunc_main\n");
    fprintf (out, "\tMOV\t\t%d\tex\n", VAR_MAX);
    fprintf (out, "\tEND\n\n");
    GetBlocks (prog->root);
}

void Translator::PrintError ()
{
    SETCOLOR (RED);
    printf ("Error: ");
    SETCOLOR (RESET);

    switch (error)
    {
        case FUNC_REDEF:
            printf ("redefinition of function %s\n", problem_node->data.name);
        break;

        case VAR_REDEF:
            printf ("redefinition of variable %s\n", problem_node->data.name);
        break;

        case INCORRECT_NUM_OF_ARGS:
            printf ("incorrect number of arguments in function %s call\n", problem_node->data.name);
        break;

        case EMPTY_RETURN:
            printf ("returning nothing\n");
        break;

        case UNDEF_FUNC:
            printf ("function %s is undefined\n", problem_node->data.name);
        break;

        case UNDEF_VAR:
            printf ("variable %s is undefined\n", problem_node->data.name);
        break;
    
        default:
            printf ("everything is bad\n");
        break;
    }
}

void Translator::Warn (const size_t warn_code)
{
    SETCOLOR (RED);
    printf ("Warning: ");
    SETCOLOR (RESET);

    switch (warn_code)
    {
        case MULTIPLE_RETURN:
            printf ("returning more than one variables! Only the first will be returned!\n");
        break;
    
        default:
            printf ("WTF\n");
        break;
    }
}

void Translator::GetBlocks (Node<Token> * node)
{
    if (!N)
    {
        return;
    }

    GetBlocks (L);

    if (IS_OP (R, OP_TYPE, DEF_FUNC))
    {
        DefFunc (R);
    }
    else if (IS_OP (R, OP_TYPE, DEF_VAR))
    {
        DefVar (R);
    }
}

void Translator::DefFunc (Node<Token> * node)
{
    PRINT ("func\n");

    fprintf (out, ";#####################################################################\n", var_cnt);

    int check = FindFunc (R->data.name, R->data.data);

    if (check != 0)
    {
        error = FUNC_REDEF;
        problem_node = R;
        return;
    }

    curr_func = functions + (func_cnt++);

    curr_func->name      = R->data.name;
    curr_func->name_len  = R->data.data;
    curr_func->VAR_MAX   = 50;
    curr_func->local_var = (Variable *) calloc (curr_func->VAR_MAX, sizeof (*curr_func->local_var));
    curr_func->var_cnt   = 1;
    
    fprintf (out, ";\tfunction \"%s\"\n", curr_func->name);
    fprintf (out, "func_%s:\n", curr_func->name);

    Node<Token> * curr_arg = L;

    while (curr_arg)
    {
        curr_func->local_var[curr_func->var_cnt].id = curr_arg->right->left->data.name;
        curr_func->local_var[curr_func->var_cnt].len = curr_arg->right->left->data.data;

        fprintf (out, ";\t%s_var_%s (arg_%lu)\n", curr_func->name, curr_func->local_var[curr_func->var_cnt].id, curr_func->var_cnt);
        fprintf (out, "\tPOP [ex+%lu]\n", curr_func->var_cnt);

        curr_func->var_cnt++;

        curr_arg = curr_arg->left;
    }

    curr_func->var_cnt--;

    fprintf (out, ";\t\"%s\" body:\n\n", curr_func->name);
    GetOperators (R->right);

    fprintf (out, ";#####################################################################\n\n\n", var_cnt);

    curr_func = nullptr;
}

void Translator::GetOperators (Node<Token> * node)
{
    PRINT ("operator\n");

    if (!N)
    {
        return;
    }

    if (IS_OP (N, OP_TYPE, BLOCK))
    {
        GetOperators (R);
        return;
    }

    GetOperators (L);

    if (IS_OP(R, OP_TYPE, IF))
    {
        GetIf (R);
    }
    else if (IS_OP(R, OP_TYPE, WHILE))
    {
        GetWhile (R);
    }
    else if (IS_OP(R, OP_TYPE, DEF_VAR))
    {
        DefVar (R);
    }
    else if (IS_OP(R, MATH_TYPE, ASSIGN))
    {
        GetAssign (R);
    }
    else if (IS_OP(R, OP_TYPE, CALL) || IS_OP(R, OP_TYPE, RET))
    {
        GetCall (R);
    }
}

void Translator::GetAssign (Node<Token> * node)
{
    PRINT ("assn\n");

    if (TYPE(L) != ID_TYPE)
    {
        return;
    }

    Calculate (R);

    int id = FindVar (L->data.name, L->data.data);

    if (id != 0)
    {
        if (id > 0)
        {
            fprintf (out, "\tPOP\t[ex+%d]\n", id);
        }
        else
        {
            fprintf (out, "\tPOP\t[%d]\n", -id);
        }
    }
    else
    {
        fprintf (out, "ZHOPA\n");
        error = UNDEF_VAR;
        problem_node = L;
        return;
    }
}

void Translator::DefVar (Node<Token> * node)
{
    PRINT ("var\n");

    int check = FindVar (L->data.name, L->data.data);

    Variable * curr_var = nullptr;

    if ((check < 0 && !curr_func) || (check > 0 && curr_func))
    {
        error = VAR_REDEF;
        problem_node = L;
        return;
    }

    if (curr_func)
    {
        curr_var = curr_func->local_var + (++curr_func->var_cnt);
    }
    else
    {
        curr_var = global_var + (var_cnt++);
    }

    curr_var->id  = L->data.name;
    curr_var->len = L->data.data;

    if (R)
    {
        GetAssign (N);
    }
}

void Translator::GetCall (Node<Token> * node)
{
    PRINT ("call\n");

    if (IS_OP (N, OP_TYPE, RET))
    {
        if (!L)
        {
            error = EMPTY_RETURN;
            problem_node = N;
            return;
        }

        if (L->left)
        {
            Warn (MULTIPLE_RETURN);
        }
        Calculate (L->right);

        fprintf (out, "\tPUSH\tex\n");
        fprintf (out, "\tPUSH\t%d\n", curr_func->VAR_MAX);
        fprintf (out, "\tSUB\n");
        fprintf (out, "\tPOP\tex\n");
        fprintf (out, "\tRET\n");
        return;
    }

    size_t arg_cnt = 0;
    Node<Token> * curr_arg = nullptr;

    curr_arg = L;

    if (curr_arg)
    {
        while (curr_arg)
        {
            arg_cnt++;
            if (!curr_arg->left)
            {
                break;
            }
            curr_arg = curr_arg->left;
        }

        while (curr_arg != N)
        {
            Calculate (curr_arg->right);
            curr_arg = curr_arg->parent;
        }
    }

    if (R->data.data == 5 &&!strncmp (R->data.name, "print", R->data.data))
    {
        for (size_t i = 0; i < arg_cnt; i++)
        {
            fprintf (out, "\tOUT\n");
        }
        return;
    }

    if (R->data.data == 5 &&!strncmp (R->data.name, "input", R->data.data))
    {
        fprintf (out, "\tIN\n");
        return;
    }

    if (R->data.data == 4 &&!strncmp (R->data.name, "putc", R->data.data))
    {
        for (size_t i = 0; i < arg_cnt; i++)
        {
            fprintf (out, "\tOUTC\n");
        }
        return;
    }

    int call_func = FindFunc (R->data.name, R->data.data);

    if (call_func != 0)
    {
        if (functions[call_func].var_cnt != arg_cnt)
        {
            error = INCORRECT_NUM_OF_ARGS;
            problem_node = R;
            return;
        }

        fprintf (out, "\tMOV\tex+%lu\tex\n", functions[call_func].VAR_MAX);
        fprintf (out, "\tCALL\tfunc_%s\n", functions[call_func].name);
        return;
    }

    error = UNDEF_FUNC;
    problem_node = R;
    return;
}

void Translator::GetIf (Node<Token> * node)
{
    PRINT ("if\n");

    size_t remember_curr_if = curr_if;
    curr_if = (if_cnt++);

    fprintf (out, "\tif_%lu:\n", curr_if);
    fprintf (out, ";\tif_%lu_condition\n", curr_if);

    Calculate (N->left);

    fprintf (out, "\t\tPUSH\t0\n");
    fprintf (out, "\t\tJNE\tif_%lu_positive\n", curr_if);
    fprintf (out, "\t\tJMP\tif_%lu_negative\n", curr_if);

    Node<Token> * tmp = R;

    fprintf (out, "\tif_%lu_positive:\n\n", curr_if);

    GetOperators (R->right);

    fprintf (out, "\n\t\tJMP\tif_%lu_end\n", curr_if);
    fprintf (out, "\tif_%lu_negative:\n\n", curr_if);

    GetOperators (R->left);

    fprintf (out, "\n\tif_%lu_end:\n", curr_if);

    curr_if = remember_curr_if;
}

void Translator::GetWhile (Node<Token> * node)
{
    PRINT ("while\n");

    size_t remember_cycle = curr_cycle;

    curr_cycle = (cycle_cnt++);

    fprintf (out, "cycle_%lu:\n", curr_cycle);

    Calculate (L);

    fprintf (out, "\tPUSH\t0\n");
    fprintf (out, "\tJE cycle_%lu_stop\n", curr_cycle);
    
    GetOperators (R);

    fprintf (out, "\tJMP cycle_%lu\n", curr_cycle);
    fprintf (out, "cycle_%lu_stop:\n", curr_cycle);

    curr_cycle = remember_cycle;
}

#define GET_BOOL( asm_jmp )                                                                                   \
    {                                                                                                         \
        fprintf (out, "\t%s case_%lu_positive\n", asm_jmp, case_cnt);                                         \
        fprintf (out, "\tJMP case_%lu_negative\n", case_cnt);                                                 \
        fprintf (out, "\tcase_%lu_positive:\n\t\tPUSH\t1\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_negative:\n\t\tPUSH\t0\n", case_cnt);                                       \
        fprintf (out, "\tcase_%lu_continue:\n", case_cnt);                                                    \
        case_cnt++;                                                                                           \
    }

#define GET_AND()                                                                                             \
    {                                                                                                         \
        fprintf (out, ";AND\n\tcase_%lu_check1:\n", case_cnt);                                                \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_negative\n", case_cnt);                                               \
        fprintf (out, "\t\tJMP\tcase_%lu_check2\n", case_cnt);                                                \
        fprintf (out, "\tcase_%lu_check2:\n", case_cnt);                                                      \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_negative\n", case_cnt);                                               \
        fprintf (out, "\t\tJMP\tcase_%lu_positive\n", case_cnt);                                              \
        fprintf (out, "\tcase_%lu_positive:\n\t\tPUSH\t1\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_negative:\n\t\tPUSH\t0\n", case_cnt);                                       \
        fprintf (out, "\tcase_%lu_continue:\n", case_cnt);                                                    \
        case_cnt++;                                                                                           \
    }

#define GET_OR()                                                                                              \
    {                                                                                                         \
        fprintf (out, ";OR\n\tcase_%lu_check1:\n", case_cnt);                                                 \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_check2\n", case_cnt);                                                 \
        fprintf (out, "\t\tJMP\tcase_%lu_positive\n", case_cnt);                                              \
        fprintf (out, "\tcase_%lu_check2:\n", case_cnt);                                                      \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_negative\n", case_cnt);                                               \
        fprintf (out, "\t\tJMP\tcase_%lu_positive\n", case_cnt);                                              \
        fprintf (out, "\tcase_%lu_positive:\n\t\tPUSH\t1\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_negative:\n\t\tPUSH\t0\n", case_cnt);                                       \
        fprintf (out, "\tcase_%lu_continue:\n", case_cnt);                                                    \
        case_cnt++;                                                                                           \
    }

#define GET_NOT()                                                                                             \
    {                                                                                                         \
        fprintf (out, ";NOT\n\tcase_%lu_check:\n", case_cnt);                                                 \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_set1\n", case_cnt);                                                   \
        fprintf (out, "\t\tJMP\tcase_%lu_set0\n", case_cnt);                                                  \
        fprintf (out, "\tcase_%lu_set1:\n", case_cnt);                                                        \
        fprintf (out, "\t\tPUSH\t1\n");                                                                       \
        fprintf (out, "\t\tJMP\tcase_%lu_continue\n", case_cnt);                                              \
        fprintf (out, "\tcase_%lu_set0:\n", case_cnt);                                                        \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\tcase_%lu_continue:\n", case_cnt);                                                    \
        case_cnt++;                                                                                           \
    }

void Translator::Calculate (Node<Token> * node)
{
    PRINT ("calculate\n");

    if (L && !IS_OP(N, OP_TYPE, CALL))
    {
        Calculate (L);
    }

    if (R && !IS_OP(N, OP_TYPE, CALL))
    {
        Calculate (R);
    }

    int tmp = 0;

    switch (TYPE(N))
    {
        case NUM_TYPE:
            fprintf (out, "\t\tPUSH\t%d \t; NUM\n", DATA(N));
        break;

        case ID_TYPE:
            tmp = FindVar (N->data.name, N->data.data);
            if (tmp > 0)
            {
                fprintf (out, "\t\tPUSH\t[ex+%d] \t; LOCAL VAR\n", tmp);
            }
            else
            {
                fprintf (out, "\t\tPUSH\t[%d] \t; GLOBAL VAR\n", -tmp);
            }
        break;
    
        default:
            switch (DATA(N))
            {
                case ADD:
                    fprintf (out, "\t\tADD\n");
                break;

                case SUB:
                    fprintf (out, "\t\tSUB\n");
                break;

                case MUL:
                    fprintf (out, "\t\tMUL\n");
                break;

                case DIV:
                    fprintf (out, "\t\tDIV\n");
                break;

                case MOD:
                    fprintf (out, "\t\tMOD\n");
                break;
                
                case NOT:
                    GET_NOT ();
                break;

                case AND:
                    GET_AND ();
                break;

                case OR:
                    GET_OR ();
                break;

                case LESS:
                    GET_BOOL ("JB");
                break;

                case GRTR:
                    GET_BOOL ("JA");
                break;

                case LEQ:
                    GET_BOOL ("JBE");
                break;

                case GEQ:
                    GET_BOOL ("JAE");
                break;

                case EQ:
                    GET_BOOL ("JE");
                break;

                case NEQ:
                    GET_BOOL ("JNE");
                break;

                case CALL:
                    GetCall (N);
                break;
            
                default:
                    fprintf (out, "\t\tUNDEF\n");
                break;
            }
        break;
    }
}

int Translator::FindVar (const char * var_id,  const size_t len)
{
    PRINT ("find var\n");

    if (curr_func)
    {
        for (int i = 1; i <= curr_func->var_cnt; i++)
        {
            if (len == curr_func->local_var[i].len &&
                !strncmp (var_id, curr_func->local_var[i].id, len))
            {
                return (i);
            }
        }
    }

    for (int i = 1; i < var_cnt; i++)
    {
        if (len == global_var[i].len &&
            !strncmp (var_id, global_var[i].id, len))
        {
            return (-i);
        }
    }

    return (0);
}

int Translator::FindFunc (const char * func_id, const size_t len)
{
    PRINT ("find func\n");

    for (int i = 1; i < func_cnt; i++)
    {
        if (len == functions[i].name_len &&
            !strncmp (func_id, functions[i].name, len))
        {
            PRINT ("FOUND\n");
            return (i);
        }
    }

    PRINT ("NOT FOUND\n");
    return (0);
}