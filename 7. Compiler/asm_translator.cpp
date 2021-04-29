#include "language_parsing.h"

void lang_asm (FILE *formula)
{
    assert(formula);

    bin_tree tree = {};
    construct_tree(&tree, "tree");

    variables var = {};
    elements elem = {};

    elem.elements_ = (bin_tree_elem **) calloc(MAX_PROJECT_TREE_ELEM, sizeof(bin_tree_elem *));

    fill_tree(&tree, formula, &var, &elem);

    free(elem.elements_);

    if (tree.root == nullptr)
    {
        destruct_tree(&tree);

        return;
    }

    optimize_tree(&tree);

    FILE *assmbl = fopen("code.txt", "wb");
    assert(assmbl);

    asm_trans(&tree, assmbl, &var);

    destruct_tree(&tree);
}

void analyse_expr (bin_tree_elem *element, FILE *assmbl, variables *var)
{
    if (element->type == NUM)
        fprintf(assmbl, "push %lg // push the number\n\n", element->value);
    else if (element->type == GLOB_VAR)
    {
        fprintf(assmbl, "// push the global variable:\n"
                        "push rdx\n"
                        "push %d\n"
                        "add\n"
                        "pop rbx\n"
                        "push [rbx]\n\n", (int) element->value);
    }
    else if (element->type == VAR)
    {
        fprintf(assmbl, "// push the variable:\n"
                        "push rex\n"
                        "push %d\n"
                        "mul\n"
                        "push %d\n"
                        "add\n"
                        "push rax\n"
                        "add\n"
                        "pop rbx\n"
                        "push [rbx]\n\n", MAX_VAR_IN_FUNC_NUM, (int) element->value);
    }
    else if (element->type == USER_FUNC)
    {
        for (bin_tree_elem *vertex = element->left; vertex != nullptr; vertex = vertex->left)
            analyse_expr(vertex->right, assmbl, var);

        fprintf(assmbl, "push 1 // shift the local-variables address to start the next function\n"
                        "push rex\n"
                        "add\n"
                        "pop rex\n"
                        "call :%s // call the function\n"
                        "push rex // shift backwards the local-variables address to end the previous function\n"
                        "push 1\n"
                        "sub\n"
                        "pop rex\n", var->var[(int) element->value]);
    }
    else if (element->type == OPER && (int) element->value == ADD)
    {
        analyse_expr(element->left, assmbl, var);
        analyse_expr(element->right, assmbl, var);

        fprintf(assmbl, "add\n\n");
    }
    else if (element->type == OPER && (int) element->value == SUB)
    {
        analyse_expr(element->left, assmbl, var);
        analyse_expr(element->right, assmbl, var);

        fprintf(assmbl, "sub\n\n");
    }
    else if (element->type == OPER && (int) element->value == MUL)
    {
        analyse_expr(element->left, assmbl, var);
        analyse_expr(element->right, assmbl, var);

        fprintf(assmbl, "mul\n\n");
    }
    else if (element->type == OPER && (int) element->value == DIV)
    {
        analyse_expr(element->left, assmbl, var);
        analyse_expr(element->right, assmbl, var);

        fprintf(assmbl, "div\n\n");
    }
    else if (element->type == OPER && (int) element->value == POW)
    {
        analyse_expr(element->left, assmbl, var);
        analyse_expr(element->right, assmbl, var);

        fprintf(assmbl, "pow\n\n");
    }
    else if (element->type == FUNC && (int) element->value == SIN)
    {
        analyse_expr(element->left, assmbl, var);

        fprintf(assmbl, "sin\n\n");
    }
    else if (element->type == FUNC && (int) element->value == COS)
    {
        analyse_expr(element->left, assmbl, var);

        fprintf(assmbl, "cos\n\n");
    }
    else if (element->type == FUNC && (int) element->value == TG)
    {
        analyse_expr(element->left, assmbl, var);

        fprintf(assmbl, "tg\n\n");
    }
    else if (element->type == FUNC && (int) element->value == ARCSIN)
    {
        analyse_expr(element->left, assmbl, var);

        fprintf(assmbl, "arcsin\n\n");
    }
    else if (element->type == FUNC && (int) element->value == ARCCOS)
    {
        analyse_expr(element->left, assmbl, var);

        fprintf(assmbl, "arccos\n\n");
    }
    else if (element->type == FUNC && (int) element->value == ARCTG)
    {
        analyse_expr(element->left, assmbl, var);

        fprintf(assmbl, "arctg\n\n");
    }
    else if (element->type == FUNC && (int) element->value == POWER)
    {
        analyse_expr(element->left, assmbl, var);
        analyse_expr(element->right, assmbl, var);

        fprintf(assmbl, "pow\n\n");
    }
}

void command_asm_tree (bin_tree_elem *element, FILE *assmbl, variables *var)
{
    if (element->type == COMMAND && (int) element->value == ASSIGN)
    {
        analyse_expr(element->right, assmbl, var);

        if (element->left->type == VAR)
        {
            fprintf(assmbl, "\n"
                            "push rex // put the out-number of stack into the variable\n"
                            "push %d\n"
                            "mul\n"
                            "push %d\n"
                            "add\n"
                            "push rax\n"
                            "add\n"
                            "pop rbx\n"
                            "pop [rbx]\n\n", MAX_VAR_IN_FUNC_NUM, (int) element->left->value);
        }
        else if (element->left->type == GLOB_VAR)
        {
            fprintf(assmbl, "\n"
                            "push rdx // put the out-number of stack the global variable\n"
                            "push %d\n"
                            "add\n"
                            "pop rbx\n"
                            "pop [rbx]\n\n", (int) element->left->value);
        }
    }
    else if (element->type == COMMAND && (int) element->value == IF)
    {
        analyse_expr(element->left->left,  assmbl, var);
        analyse_expr(element->left->right, assmbl, var);

        switch ((int) element->left->value)
        {
            case JE:
            {
                fprintf(assmbl, "jne :end_if_%p // if two expression are equal, do the next:\n\n", element->left);
                break;
            }
            case JA:
            {
                fprintf(assmbl, "jbe :end_if_%p // if the first expr is greater than the second, do the next:\n\n", element->left);
                break;
            }
            case JAE:
            {
                fprintf(assmbl, "jb :end_if_%p // if the first expr is greater than the second or equal, do the next:\n\n", element->left);
                break;
            }
            case JB:
            {
                fprintf(assmbl, "jae :end_if_%p // if the first expr is less than the second, do the next:\n\n", element->left);
                break;
            }
            case JBE:
            {
                fprintf(assmbl, "ja :end_if_%p // if the first expr is less than the second or equal, do the next:\n\n", element->left);
                break;
            }
            case JNE:
            {
                fprintf(assmbl, "je :end_if_%p // if two expression are not equal, do the next:\n\n", element->left);
                break;
            }
            default:
                break;
        }

        body_asm(element->right, assmbl, var);

        fprintf(assmbl, "end_if_%p:\n", element->left);
    }
    else if (element->type == COMMAND && (int) element->value == WHILE)
    {
        fprintf(assmbl, "while_%p:\n", element);

        analyse_expr(element->left->left,  assmbl, var);
        analyse_expr(element->left->right, assmbl, var);

        switch ((int) element->left->value)
        {
            case JE:
            {
                fprintf(assmbl, "je :end_while_%p // if two expression are equal, do the next->\n\n", element->left);
                break;
            }
            case JA:
            {
                fprintf(assmbl, "jbe :end_while_%p // if the first expr is greater than the second, do the next->\n\n", element->left);
                break;
            }
            case JAE:
            {
                fprintf(assmbl, "jb :end_while_%p // if the first expr is greater than the second or equal, do the next->\n\n", element->left);
                break;
            }
            case JB:
            {
                fprintf(assmbl, "jae :end_while_%p // if the first expr is less than the second, do the next->\n\n", element->left);
                break;
            }
            case JBE:
            {
                fprintf(assmbl, "ja :end_while_%p // if the first expr is less than the second or equal, do the next->\n\n", element->left);
                break;
            }
            case JNE:
            {
                fprintf(assmbl, "je :end_while_%p // if two expression are not equal, do the next->\n\n", element->left);
                break;
            }
            default:
                break;
        }

        body_asm(element->right, assmbl, var);
        fprintf(assmbl, "jump :while_%p // check the while-condition again\n", element);

        fprintf(assmbl, "end_while_%p:\n", element->left);
    }
    else if (element->type == FUNC && ((int) element->value == SCAN || (int) element->value == PRINT))
    {
        if ((int) element->value == SCAN)
        {
            fprintf(assmbl, "in // scan the variable\n"
                            "push rex\n"
                            "push %d\n"
                            "mul\n"
                            "push %d\n"
                            "add\n"
                            "push rax\n"
                            "add\n"
                            "pop rbx\n"
                            "pop [rbx]\n"
                            "\n", MAX_VAR_IN_FUNC_NUM, (int) element->left->value);
        }
        else if ((int) element->value == PRINT)
        {

            analyse_expr(element->left, assmbl, var);

            fprintf(assmbl, "out\n"
                            "pop\n");
        }
    }
    else if (element->type == USER_FUNC)
    {
        for (bin_tree_elem *vertex = element->left; vertex != nullptr; vertex = vertex->left)
            analyse_expr(vertex->right, assmbl, var);

        fprintf(assmbl, "push 1 // shift the local-variables address to start the next function\n"
                        "push rex\n"
                        "add\n"
                        "pop rex\n"
                        "call :%s // call the function\n"
                        "push rex // shift backwards the local-variables address to end the previous function\n"
                        "push 1\n"
                        "sub\n"
                        "pop rex\n", var->var[(int) element->value]);
    }
    else if (element->type == RETURN)
    {
        if (element->left != nullptr)
        {
            fprintf(assmbl, "// put the expression in stack and return:\n\n");
            analyse_expr(element->left, assmbl, var);
        }

        fprintf(assmbl, "ret // return from the func\n\n");
    }
}

void body_asm (bin_tree_elem *vertex, FILE *assmbl, variables *var)
{
    while (vertex != nullptr)
    {
        command_asm_tree(vertex->left, assmbl, var);
        vertex = vertex->right;
    }
}

void find_func_var (bin_tree_elem *element, int *param, int *n_param, int first_loc_var)
{
    if (element->left != nullptr)
        find_func_var(element->left, param, n_param, first_loc_var);

    if (element->type == VAR)
    {
        int find_state = 0;

        for (int i = first_loc_var; i < *n_param; i++)
        {
            if (param[i] == (int) element->value)
            {
                find_state = 1;
                element->value = i;
                break;
            }
        }

        if (find_state == 0 && element->value > first_loc_var)
        {
            param[*n_param] = (int) element->value;
            element->value = *n_param;
            (*n_param)++;
        }
    }

    if (element->right != nullptr)
        find_func_var(element->right, param, n_param, first_loc_var);
}

void param_pass_func (bin_tree_elem *element, FILE *assmbl)
{
    if (element->left != nullptr)
        param_pass_func(element->left, assmbl);

    fprintf(assmbl, "pop [rbx + %d] // gets all parametres from parent-function\n", (int) element->value);
}

void func_asm (bin_tree_elem *vertex, FILE *assmbl, variables *var)
{
    int *param = (int *) calloc(MAX_FUNC_PARAM, sizeof(int));
    int n_param       = 0;
    int first_loc_var = 0;

    for (bin_tree_elem *elem = vertex->left; elem != nullptr; elem = elem->left)
        param[n_param++] = elem->value;

    first_loc_var = n_param;

    user_func_optimize(vertex, param, n_param);

    find_func_var (vertex->right, param, &n_param, first_loc_var);

    fprintf(assmbl, "%s:\n"
                    "\n"
                    "push rex\n"
                    "push %d\n"
                    "mul\n"
                    "push 0\n"
                    "add\n"
                    "push rax\n"
                    "add\n"
                    "pop rbx\n"
                    "\n", var->var[(int) vertex->value], MAX_VAR_IN_FUNC_NUM);

    param_pass_func(vertex->left, assmbl);

    fprintf(assmbl, "\n");

    body_asm(vertex->right, assmbl, var);

    free(param);
}

void main_var_optimize (bin_tree_elem *element, int n_glob_vars)
{
    if (element->left != nullptr)
        main_var_optimize(element->left, n_glob_vars);

    if (element->type == VAR)
        element->value -= n_glob_vars;

    if (element->right != nullptr)
        main_var_optimize(element->right, n_glob_vars);
}

void find_glob_vars (bin_tree_elem *element, int n_glob_vars)
{
    if (element->left != nullptr)
        find_glob_vars(element->left, n_glob_vars);

    if (element->type == VAR && element->value < n_glob_vars)
        element->type = GLOB_VAR;

    if (element->right != nullptr)
        find_glob_vars(element->right, n_glob_vars);
}

void asm_trans (bin_tree *tree, FILE *assmbl, variables *var)
{
    fprintf(assmbl, "push 10000\npop rax\n"
                    "push 20000\npop rbx\n"
                    "push 30000\npop rcx\n"
                    "push 40000\npop rdx\n"
                    "push 0\n"
                    "pop rex\n\n");

    bin_tree_elem *vertex = tree->root;

    int n_glob_vars = 0;

    while (vertex->left->type == COMMAND && (int) vertex->left->value == ASSIGN)
    {
        analyse_expr(vertex->left->right, assmbl, var);
        fprintf(assmbl, "\npop [rdx+%d]\n", (int) vertex->left->left->value);
        vertex = vertex->right;

        n_glob_vars++;
    }

    find_glob_vars(vertex, n_glob_vars);

    main_var_optimize(vertex, n_glob_vars);

    body_asm(vertex->left->right, assmbl, var);

    vertex = vertex->right;

    fprintf(assmbl, "\nhlt\n\n");

    while (vertex != nullptr)
    {
        func_asm(vertex->left, assmbl, var);
        vertex = vertex->right;
    }
}

void user_func_optimize (bin_tree_elem *element, int *param, int n_param)
{
    if (element->left != nullptr)
        user_func_optimize(element->left, param, n_param);

    if (element->type == VAR)
    {
        int find_state = 0;

        for (int i = 0; i < n_param; i++)
        {
            if (param[i] == (int) element->value)
            {
                find_state     = 1;
                element->value = i;
            }
        }

        if (find_state == 0 && element->value < n_param)
            element->value += 100;
    }

    if (element->right != nullptr)
        user_func_optimize(element->right, param, n_param);
}
