#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "tree_config.h"

#define ASSERT_TREE_OK             \
    if (error_tree(tree) != 0)     \
    {                              \
        dump_tree(tree);           \
        return nullptr;            \
    }

#define ASSERT_TREE_OK_VOID        \
    if (error_tree(tree) != 0)     \
    {                              \
        dump_tree(tree);           \
        return;                    \
    }

#define TREE_CONSTR(name)          \
    construct_tree(&name, #name);

#define LE element->left
#define RE element->right
#define dR derivate_tree(element->right, var_value)
#define dL derivate_tree(element->left, var_value)
#define cL copy_tree(element->left)
#define cR copy_tree(element->right)

#define MULTIPLY(left, right)                                                                   \
    create_tree_element(OPER, MUL, left, right)

#define DIVIDE(left, right)                                                                     \
    create_tree_element(OPER, DIV, left, right)

#define ADDITION(left, right)                                                                   \
    create_tree_element(OPER, ADD, left, right)

#define SUBTRACT(left, right)                                                                   \
    create_tree_element(OPER, SUB, left, right)

#define POWER(left, right)                                                                      \
    create_tree_element(OPER, POW, left, right)

#define POWER_NUM(element, value)                                                               \
    create_tree_element(OPER, POW, element, create_tree_element(NUM, value, nullptr, nullptr))

#define CR_NUM(value)                                                                           \
    create_tree_element(NUM, value, nullptr, nullptr)

#define CR_VAR(value)                                                                           \
    create_tree_element(VAR, value, nullptr, nullptr)

struct bin_tree_elem
{
    int type      = 0;
    double value  = 0;
    int hash_tree = 0;
    int elem_size = 0;

    bin_tree_elem *left = nullptr;
    bin_tree_elem *right = nullptr;
};

struct bin_tree
{
    bin_tree_elem *root = nullptr;

    int tree_size = 0;

    int error_state = 0;

    char *tree_name = nullptr;
};

//-----------------------------------------------------------------------------
//! Tree-constructor
//! @param [in] tree - the structure of binary tree
//! @param [in] name - the name of binary_tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Constructs binary tree, preparing it to be used
//-----------------------------------------------------------------------------

void construct_tree (bin_tree *tree, const char *name);

//-----------------------------------------------------------------------------
//! Tree-destructor
//! @param [in] tree - the structure of text
//! @version 1.0
//! @authors Vokerlee
//! @brief Destructs text, releasing all buff-memory
//-----------------------------------------------------------------------------

void destruct_tree (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Deletes tree-elements
//! @param [in] element - the element of tree, memory of all its daughters which will be released
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes tree-elements, beginning from set element
//-----------------------------------------------------------------------------

void delete_tree_elem (bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Inserts the element in tree from left position of "element"
//! @param [in] tree    - the structure of tree
//! @param [in] element - the element of tree, from left position of which will be pushed new element with set data
//! @param [in] type    - the type of inserting element
//! @param [in] value   - the value of inserting element
//! @version 1.0
//! @authors Vokerlee
//! @brief Inserts the element in tree from left position of "element" with set data values
//-----------------------------------------------------------------------------

bin_tree_elem *insert_left_tree (bin_tree *tree, bin_tree_elem *element, int type, double value);

//-----------------------------------------------------------------------------
//! Inserts the element in tree from right position of "element"
//! @param [in] tree    - the structure of tree
//! @param [in] element - the element of tree, from right position of which will be pushed new element with set data
//! @param [in] type    - the type of inserting element
//! @param [in] value   - the value of inserting element
//! @version 1.0
//! @authors Vokerlee
//! @brief Inserts the element in tree from right position of "element" with set data values
//-----------------------------------------------------------------------------

bin_tree_elem *insert_right_tree (bin_tree *tree, bin_tree_elem *element, int type, double value);

bin_tree_elem *create_tree_element (int type, double value, bin_tree_elem *left, bin_tree_elem *right);

bin_tree_elem *copy_tree_elem (bin_tree_elem *element);

bin_tree_elem *copy_tree (bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Recalculates the size of tree
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] counter - the counter of tree-elements (in the end contains the number of all elements)
//! @version 1.0
//! @authors Vokerlee
//! @brief Recalculates the size of tree
//-----------------------------------------------------------------------------

void recalc_size_tree (bin_tree_elem *element, int *size_tree);

//-----------------------------------------------------------------------------
//! Prints all info about tree in case of its error (in tree_log.txt)
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints all info about tree in case of its error (in tree_log.txt), also open graph-dump-pdf
//-----------------------------------------------------------------------------

void dump_tree (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Checks tree for different errors
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Checks tree for different errors
//-----------------------------------------------------------------------------

int error_tree (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Checks tree for valid size
//! @param [in] tree    - the structure of tree
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] counter - the counter of tree-elements (is used to escape looping, counter = 0 - required initial value)
//! @version 1.0
//! @authors Vokerlee
//! @brief Checks tree for valid size
//-----------------------------------------------------------------------------

void check_size_tree (bin_tree *tree, bin_tree_elem *element, int *counter);

//-----------------------------------------------------------------------------
//! Prints all info for text-dump in "output"
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] output  - the stream of output
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints all info for text-dump in "output" (mainly is used for dump_tree)
//-----------------------------------------------------------------------------

void print_tree (bin_tree_elem *element, FILE *output);

//-----------------------------------------------------------------------------
//! Declares all tree-elements in output for graphviz
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] output  - the stream of output
//! @version 1.0
//! @authors Vokerlee
//! @brief Declares all tree-elements in output for graphviz (mainly is used for dump_tree)
//-----------------------------------------------------------------------------

void declare_vertex_tree (bin_tree_elem *element, FILE *output);

//-----------------------------------------------------------------------------
//! Links all tree-elements in output for graphviz
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] output  - the stream of output
//! @version 1.0
//! @authors Vokerlee
//! @brief Links all tree-elements in output for graphviz (mainly is used for dump_tree)
//-----------------------------------------------------------------------------

void link_vertex_tree (bin_tree_elem *element, FILE *output);

//-----------------------------------------------------------------------------
//! Returns the line of error-message
//! @param [in] error - the error, discovered in tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the line of error-message
//-----------------------------------------------------------------------------

const char *print_tree_error (int error);

//-----------------------------------------------------------------------------
//! Optimizes tree
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Optimizes tree, using different maths properties
//-----------------------------------------------------------------------------

void optimize_tree (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Optimizes tree by folding multiplication by zero and one
//! @param [in] element        - the element of tree (usually the root of tree)
//! @param [in] optimize_state - the parameter, which changes in case of optimization
//! @version 1.0
//! @authors Vokerlee
//! @brief Optimizes tree by folding multiplication by zero and one
//-----------------------------------------------------------------------------

void num_optimize (bin_tree_elem *element, int *optimize_state);

//-----------------------------------------------------------------------------
//! Optimizes tree by folding multiplication by a fraction of the form 1/f(x1,...,x2)
//! @param [in] element        - the element of tree (usually the root of tree)
//! @param [in] optimize_state - the parameter, which changes in case of optimization
//! @version 1.0
//! @authors Vokerlee
//! @brief Optimizes tree by folding multiplication by a fraction of the form 1/f(x1,...,x2)
//-----------------------------------------------------------------------------

void div_optimize (bin_tree_elem *element, int *optimize_state);

//-----------------------------------------------------------------------------
//! Optimizes tree by replacing all expressions consisting of operators and numbers with a number
//! @param [in] element        - the element of tree (usually the root of tree)
//! @param [in] optimize_state - the parameter, which changes in case of optimization
//! @version 1.0
//! @authors Vokerlee
//! @brief Optimizes tree by replacing all expressions consisting of operators and numbers with a number
//-----------------------------------------------------------------------------

void const_fold_optimize (bin_tree_elem *element, int *optimize_state);


#endif // TREE_H_INCLUDED
