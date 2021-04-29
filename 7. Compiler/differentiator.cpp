#include "language_parsing.h"
                                                                          \
#define ORIG_FORMULA_PRINT                                                \
    fprintf(tex, "\n%s\n\\[ \\Big(", phrase_cond_print());                \
    printf_formula(element, tex, var, BRACKETS_OFF);                      \
    fprintf(tex, " \\Big)\'\\]\n%s\n\\[", phrase_eq_print());

#define DIFF_FUNC_PRINT(command)                                          \
    fprintf(tex, #command " \\left( ");                                   \
    printf_formula(L, tex, var, BRACKETS_OFF);                            \
    fprintf(tex, " \\right) \\cdot \\left( ");                            \
    printf_formula(L, tex, var, BRACKETS_OFF);                            \
    fprintf(tex, " \\right)\' \\]");

#define CREATE_FUNC(name, num)                                                                   \
{                                                                                                \
    elem->elements[elem->curr_size++] = create_tree_element(FUNC, name, nullptr, nullptr);       \
                                                                                                 \
    text->counter += num;                                                                        \
}                                                                                                \

void print_func_text (int value, FILE *tex)
{
    assert(tex);

    switch(value)
    {
        case SIN:
        {
            fprintf(tex, "sin");
            break;
        }
        case COS:
        {
            fprintf(tex, "cos");
            break;
        }
        case TG:
        {
            fprintf(tex, "tg");
            break;
        }
        case CTG:
        {
            fprintf(tex, "ctg");
            break;
        }
        case ARCSIN:
        {
            fprintf(tex, "arcsin");
            break;
        }
        case ARCCOS:
        {
            fprintf(tex, "arccos");
            break;
        }
        case ARCTG:
        {
            fprintf(tex, "arctg");
            break;
        }
        case ARCCTG:
        {
            fprintf(tex, "arcctg");
            break;
        }
        case SH:
        {
            fprintf(tex, "sh");
            break;
        }
        case CH:
        {
            fprintf(tex, "ch");
            break;
        }
        case TH:
        {
            fprintf(tex, "th");
            break;
        }
        case CTH:
        {
            fprintf(tex, "cth");
            break;
        }
        case LN :
        {
            fprintf(tex, "ln");
            break;
        }
        default:
            return;
    }
}

int is_elem_func (bin_tree_elem *element)
{
    int state1 = 0;
    int state2 = 0;

    if (element->type == VAR)
        return 1;

    if (LE != nullptr)
        state1 = is_elem_func(LE);

    if (RE != nullptr)
        state2 = is_elem_func(RE);

    return state1 + state2;
}

bin_tree_elem *derivate_tree (bin_tree_elem *element, int var_value)
{
    switch (element->type)
    {
        case NUM:
            return CR_NUM(0);
        case VAR:
        {
            if ((int) element->value == var_value)
                return CR_NUM(1);
            else
                return CR_NUM(0);
        }
        case OPER:
        {
            switch ((int) element->value)
            {
                case ADD:
                    return ADDITION(dL, dR);
                case SUB:
                    return SUBTRACT(dL, dR);
                case MUL:
                    return ADDITION(MULTIPLY(dL, cR), MULTIPLY(cL, dR));
                case DIV:
                    return DIVIDE(SUBTRACT(MULTIPLY(dL, cR), MULTIPLY(cL, dR)), POWER_NUM(cR, 2));
                case POW:
                {
                    if (LE->type == NUM && RE->type == NUM)
                        return CR_NUM(0);
                    else if (LE->type == NUM && RE->type == VAR)
                        return MULTIPLY(copy_tree(element), create_tree_element(FUNC, LN, CR_NUM(LE->value), nullptr));
                    else if (LE->type == NUM && (RE->type == FUNC || RE->type == OPER))
                        return MULTIPLY(MULTIPLY(copy_tree(element), create_tree_element(FUNC, LN, CR_NUM(LE->value), nullptr)), dR);
                    else if (LE->type == VAR && RE->type == NUM)
                        return MULTIPLY(CR_NUM(RE->value), POWER_NUM(CR_VAR(LE->value), RE->value - 1));
                    else if (LE->type == FUNC && RE->type == NUM)
                        return MULTIPLY(MULTIPLY(CR_NUM(RE->value), POWER_NUM(cL, RE->value - 1)), dL);
                    else if (is_elem_func(LE) != 0 && is_elem_func(RE) != 0)
                        return MULTIPLY(copy_tree(element), derivate_tree(MULTIPLY(cR, create_tree_element(FUNC, LN, cL, nullptr)), var_value));
                }
                default:
                    break;
            }
        }
        case FUNC:
        {
            switch ((int) element->value)
            {
                case SIN:
                    return MULTIPLY(create_tree_element(FUNC, COS, cL, nullptr), dL);
                case COS:
                    return MULTIPLY(MULTIPLY(create_tree_element(FUNC, COS, cL, nullptr), CR_NUM(-1)), dL);
                case TG:
                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(create_tree_element(FUNC, COS, cL, nullptr), CR_NUM(2))), dL);
                case CTG:
                    return MULTIPLY(CR_NUM(-1), MULTIPLY(create_tree_element(OPER, DIV, CR_NUM(1), create_tree_element(OPER, POW, create_tree_element(FUNC, SIN, cL, nullptr), CR_NUM(2))), dL));
                case ARCSIN:
                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(SUBTRACT(CR_NUM(1), POWER(cL, CR_NUM(2))), CR_NUM(0.5))), cL);
                case ARCCOS:
                    return MULTIPLY(CR_NUM(-1), MULTIPLY(DIVIDE(CR_NUM(1), POWER(SUBTRACT(CR_NUM(1), POWER(cL, CR_NUM(2))), CR_NUM(0.5))), cL));
                case ARCTG:
                    return DIVIDE(CR_NUM(1), ADDITION(CR_NUM(1), POWER(cL, CR_NUM(2))));
                case ARCCTG:
                    return MULTIPLY(CR_NUM(-1), DIVIDE(CR_NUM(1), ADDITION(CR_NUM(1), POWER(cL, CR_NUM(2)))));
                case SH:
                    return MULTIPLY(create_tree_element(FUNC, CH, cL, nullptr), dL);
                case CH:
                    return MULTIPLY(create_tree_element(FUNC, SH, cL, nullptr), dL);
                case TH:
                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(create_tree_element(FUNC, CH, cL, nullptr), CR_NUM(2))), dL);
                case CTH:
                    return MULTIPLY(create_tree_element(OPER, DIV, CR_NUM(1), create_tree_element(OPER, POW, create_tree_element(FUNC, SH, cL, nullptr), CR_NUM(2))), dL);
                case LN:
                    return MULTIPLY(DIVIDE(CR_NUM(1), cL), dL);
                default:
                    break;
            }
        }
        default:
                break;
    }
}

int hash_elem_count (bin_tree_elem *element)
{
    element->hash_tree = element->type + (int) element->value;

    if (LE != nullptr)
        element->hash_tree += hash_elem_count(LE);

    if (RE != nullptr)
        element->hash_tree += hash_elem_count(RE);

    return element->hash_tree;
}

int size_elem_count (bin_tree_elem *element)
{
    if (LE != nullptr)
        element->elem_size += size_elem_count(LE);

    if (RE != nullptr)
        element->elem_size += size_elem_count(RE);

    if (RE == nullptr && LE == nullptr)
        element->elem_size = 0;

    element->elem_size += 1;

    return element->elem_size;
}


