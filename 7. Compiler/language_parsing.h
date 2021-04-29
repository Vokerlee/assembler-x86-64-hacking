#ifndef LANGUAGE_PARSING_H_INCLUDED
#define LANGUAGE_PARSING_H_INCLUDED

#include "tree.h"
#include "differentiator.h"
#include "language_parsing_config.h"

#include "text.h"

#include <algorithm>

struct variables
{
    int curr_size = 0;

    char var[MAX_VAR_NUM][MAX_VAR_NAME_LENGTH] = {0};
};

struct elements
{
    bin_tree_elem **elements_ = nullptr;

    int curr_size_ = 0;
};

int var_search (variables *var, char *temp_var_name);

bin_tree_elem *create_prog_tree (elements *elem, int *counter);

bin_tree_elem *create_user_func_tree (elements *elem, int *counter);

bin_tree_elem *create_var_func_tree (elements *elem, int *counter);

bin_tree_elem *create_param_func_tree (elements *elem, int *counter);

bin_tree_elem *create_main_tree (elements *elem, int *counter);

bin_tree_elem *create_body_tree (elements *elem, int *counter);

bin_tree_elem *create_cmd_tree (elements *elem, int *counter);

bin_tree_elem *create_n_tree (elements *elem, int *counter);

bin_tree_elem *read_w_tree (elements *elem, int *counter);

bin_tree_elem *create_w_tree (elements *elem, int *counter);

bin_tree_elem *create_t_tree (elements *elem, int *counter);

bin_tree_elem *create_p_tree (elements *elem, int *counter);

bin_tree_elem *create_e_tree (elements *elem, int *counter);

void syntax_error(text_t *text, elements *elem, int line, const char *file);

int is_keyword (char *temp_var_name, int *type);

void fill_tree (bin_tree *tree, FILE *formula, variables *var, elements *elem);

void analyse_expr (bin_tree_elem *element, FILE *assmbl, variables *var);

void command_asm_tree (bin_tree_elem *element, FILE *assmbl, variables *var);

void body_asm (bin_tree_elem *vertex, FILE *assmbl, variables *var);

void find_func_var (bin_tree_elem *element, int *param, int *n_param, int first_loc_var);

void param_pass_func (bin_tree_elem *element, FILE *assmbl);

void user_func_optimize (bin_tree_elem *element, int *param, int n_param);

void func_asm (bin_tree_elem *vertex, FILE *assmbl, variables *var);

void main_var_optimize (bin_tree_elem *element, int n_glob_vars);

void find_glob_vars (bin_tree_elem *element, int n_glob_vars);

void asm_trans (bin_tree *tree, FILE *assmbl, variables *var);

void lang_asm (FILE *formula);

#endif // LANGUAGE_PARSING_H_INCLUDED
