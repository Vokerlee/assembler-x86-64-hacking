#include "language_parsing.h"

#define SUBST_REC(elem1, elem2)                                           \
{                                                                         \
    free(element->elem1);                                                 \
                                                                          \
    element->type  = element->elem2->type;                                \
    element->value = element->elem2->value;                               \
                                                                          \
    bin_tree_elem *temp = element->elem2;                                 \
    element->left  = temp->left;                                          \
    element->right = temp->right;                                         \
                                                                          \
    free(temp);                                                           \
                                                                          \
    (*optimize_state)++;                                                  \
}

#define SUBST_MUL0(elem1, elem2)                                          \
{                                                                         \
    delete_tree_elem(element->elem1);                                     \
    free(element->elem2);                                                 \
                                                                          \
    element->type  = NUM;                                                 \
    element->value = 0;                                                   \
    element->left  = nullptr;                                             \
    element->right = nullptr;                                             \
                                                                          \
    (*optimize_state)++;                                                  \
}                                                                         \

#define SUBST_DIV(elem1, elem2)                                           \
{                                                                         \
    element->elem1->left->type  = element->elem2->type;                   \
    element->elem1->left->value = element->elem2->value;                  \
    element->elem1->left->left  = element->elem2->left;                   \
    element->elem1->left->right = element->elem2->right;                  \
                                                                          \
    free(element->elem2);                                                 \
                                                                          \
    element->type  = element->elem1->type;                                \
    element->value = element->elem1->value;                               \
                                                                          \
    bin_tree_elem *temp = element->elem1;                                 \
    element->left  = temp->left;                                          \
    element->right = temp->right;                                         \
                                                                          \
    free(temp);                                                           \
                                                                          \
    (*optimize_state)++;                                                  \
}                                                                         \


void num_optimize (bin_tree_elem *element, int *optimize_state)
{
    if (LE != nullptr)
        num_optimize(LE, optimize_state);

    if (element->type == OPER)
    {
        if (LE->type == NUM && LE->value == 1 && (int) element->value == MUL)
            SUBST_REC(left, right)
        else if (RE->type == NUM && RE->value == 1 && ((int) element->value == MUL || (int) element->value == DIV || (int) element->value == POW))
            SUBST_REC(right, left)
        else if (LE->type == NUM && LE->value == 0 && ((int) element->value == ADD || (int) element->value == SUB))
            SUBST_REC(left, right)
        else if (RE->type == NUM && RE->value == 0 && ((int) element->value == ADD || (int) element->value == SUB))
            SUBST_REC(right, left)
        else if (LE->type == NUM && LE->value == 0 && ((int) element->value == MUL || (int) element->value == DIV))
            SUBST_MUL0(right, left)
        else if (RE->type == NUM && RE->value == 0 && (int) element->value == MUL)
            SUBST_MUL0(left, right)
    }

    if (RE != nullptr)
        num_optimize(RE, optimize_state);
}

void div_optimize (bin_tree_elem *element, int *optimize_state)
{
    if (LE != nullptr)
        div_optimize(LE, optimize_state);

    if (element->type == OPER && (int) element->value == MUL)
    {
        if (LE->type == OPER && (int) LE->value == DIV)
        {
            if (LE->left->type == NUM && (int) LE->left->value == 1 && (int) RE->value != DIV)
                SUBST_DIV(left, right)
        }

        if (RE->type == OPER && (int) RE->value == DIV)
        {
            if (RE->left->type == NUM && (int) RE->left->value == 1 && (int) LE->value != DIV)
                SUBST_DIV(right, left)
        }
    }

    if ((int) element->value == DIV && LE->type == VAR && RE->type == VAR && (int) LE->value == RE->value)
    {
        element->type  = NUM;
        element->value = 1;

        free(LE);
        free(RE);

        (*optimize_state)++;

        LE = nullptr;
        RE = nullptr;
    }

    if (RE != nullptr)
        div_optimize(RE, optimize_state);
}

void const_fold_optimize (bin_tree_elem *element, int *optimize_state)
{
    if (LE != nullptr)
        const_fold_optimize(LE, optimize_state);

    if (element->type == OPER && LE->type == NUM && RE->type == NUM)
    {
        element->type = NUM;

        switch ((int) element->value)
        {
            case ADD:
            {
                element->value = LE->value + RE->value;
                break;
            }
            case SUB:
            {
                element->value = LE->value - RE->value;
                break;
            }
            case MUL:
            {
                element->value = LE->value * RE->value;
                break;
            }
            case DIV:
            {
                element->value = LE->value / RE->value;
                break;
            }
            case POW:
            {
                element->value = pow(LE->value, RE->value);
                break;
            }
            default:
                break;
        }

        free(RE);
        free(LE);

        RE = nullptr;
        LE = nullptr;

        (*optimize_state)++;
    }

    if (RE != nullptr)
        const_fold_optimize(RE, optimize_state);
}

void diff_optimize(bin_tree_elem *element)
{
    if (LE != nullptr)
        diff_optimize(LE);

    if (element->right != nullptr && element->right->type == FUNC && (int) element->right->value == DIFF)
    {
        bin_tree_elem *diff = derivate_tree(element->right->right, (int) element->right->left->value);

        delete_tree_elem(element->right);

        element->right = diff;
    }

    if (RE != nullptr)
        diff_optimize(RE);
}

void optimize_tree (bin_tree *tree)
{
    ASSERT_TREE_OK_VOID

    int optimize_state = 0;
    int prev_state     = -1;

    diff_optimize(tree->root);

    while(optimize_state != prev_state)
    {
        prev_state = optimize_state;

        const_fold_optimize(tree->root, &optimize_state);

        num_optimize(tree->root, &optimize_state);
    }

    optimize_state = 0;
    prev_state     = -1;

    while(optimize_state != prev_state)
    {
        prev_state = optimize_state;

        div_optimize(tree->root, &optimize_state);
    }

    int counter = 0;
    recalc_size_tree(tree->root, &counter);
    tree->tree_size = counter;

    ASSERT_TREE_OK_VOID
}
