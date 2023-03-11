/** 
 * Copyright 2021 Slawomir Maludzinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "gencode.h"
#include <assert.h>
#include <stdio.h>

void var_gencode(var * value, gencode_result * result)
{
    switch (value->type)
    {
        case VAR_TYPE_BOUND:
            printf("PUT_REF name %s index %d\n", value->name, value->bound_to->index);
        break;
        case VAR_TYPE_UNBOUND:
            printf("PUT_VAR name %s index %d\n", value->name, value->bound_to->index);
        break;
        case VAR_TYPE_UNKNOWN:
            printf("PUT_VAR_UNKNOWN!!! %d: %s\n", value->line_no, value->name);
        break;
    }
}

void var_unify_gencode(var * value, gencode_result * result)
{
    switch (value->type)
    {
        case VAR_TYPE_BOUND:
            printf("U_REF name %s index %d\n", value->name, value->bound_to->index);
        break;
        case VAR_TYPE_UNBOUND:
            printf("U_VAR name %s index %d\n", value->name, value->index);
        break;
        case VAR_TYPE_UNKNOWN:
            printf("PUT_VAR_UNKNOWN!!! %d: %s\n", value->line_no, value->name);
        break;
    }
}

void var_check_gencode(var * value, gencode_result * result)
{
     printf("CHECK name %s index %d\n", value->name, value->bound_to->index);
}

void var_get_bound_vars_gencode(var * value, var_list * bound_vars, gencode_result * result)
{
    switch (value->type)
    {
        case VAR_TYPE_BOUND:
            var_list_add_end(bound_vars, value);
        break;
        case VAR_TYPE_UNBOUND:
        break;
        case VAR_TYPE_UNKNOWN:
            assert(0);
        break;
    }
}

void var_list_check_gencode(var_list * bound_vars, gencode_result * result)
{
    var_node * node = bound_vars->head;
    while (node != NULL)
    {
        var * var_value = node->value;
        if (var_value)
        {
            var_check_gencode(var_value, result);
        }
        node = node->next;
    }
}

void term_gencode(term * value, gencode_result * result)
{
    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
            assert(0);
        break;
        case TERM_TYPE_ANON:
            printf("PUT_ANON _\n");
        break;
        case TERM_TYPE_ATOM:
            printf("PUT_ATOM %s\n", value->name);
        break;
        case TERM_TYPE_VAR:
            var_gencode(value->var_value, result);
        break;
        case TERM_TYPE_TERM:
            term_list_gencode(value->terms, result);
            printf("PUT_STRUCT %s/%d\n", value->name, term_list_size(value->terms));
        break;
    }
}

void term_unify_gencode(term * value, gencode_result * result)
{
    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
            assert(0);
        break;
        case TERM_TYPE_ANON:
            printf("POP \n");
        break;
        case TERM_TYPE_ATOM:
            printf("UATOM %s\n", value->name);
        break;
        case TERM_TYPE_VAR:
            var_unify_gencode(value->var_value, result);
        break;
        case TERM_TYPE_TERM:
        {
            printf("USTRUCT %s/%d\n", value->name, term_list_size(value->terms));
            term_list_unify_gencode(value->terms, result);
            printf("UP B\n");
            printf("A:\n");
            var_list * bound_vars = var_list_new();
            term_list_get_bound_vars_gencode(value->terms, bound_vars, result);
            var_list_check_gencode(bound_vars, result);
            var_list_delete_null(bound_vars);
            term_gencode(value, result);
            printf("BIND\n");
            printf("B:\n");
        }
        break;
    }
}

void term_get_bound_vars_gencode(term * value, var_list * bound_vars, gencode_result * result)
{
    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
            assert(0);
        break;
        case TERM_TYPE_ANON:
        break;
        case TERM_TYPE_ATOM:
        break;
        case TERM_TYPE_VAR:
            var_get_bound_vars_gencode(value->var_value, bound_vars, result);
        break;
        case TERM_TYPE_TERM:
            term_list_get_bound_vars_gencode(value->terms, bound_vars, result);
        break;
    }
}

void term_list_gencode(term_list * list, gencode_result * result)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_gencode(node, result);
        node = node->next;
    }
}

void term_list_unify_gencode(term_list * list, gencode_result * result)
{
    unsigned int son_number = 1;

    term * node = list->head;
    while (node != NULL)
    {
        printf("SON %u\n", son_number++);
        term_unify_gencode(node, result);
        node = node->next;
    }
}

void term_list_get_bound_vars_gencode(term_list * list, var_list * bound_vars, gencode_result * result)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_get_bound_vars_gencode(node, bound_vars, result);
        node = node->next;
    }
}

void goal_literal_gencode(goal_literal value, gencode_result * result)
{
    printf("MARK B\n");
    term_list_gencode(value.terms, result);
    printf("CALL %s/%u\n", value.name, term_list_size(value.terms));
    printf("B: ...\n");
}

void goal_unification_gencode(goal_unification value, gencode_result * result)
{
    switch (value.variable->type)
    {
    case VAR_TYPE_UNBOUND:
    {
        var_gencode(value.variable, result);
        term_gencode(value.term_value, result);
        printf("BIND\n");
        break;
    }
    case VAR_TYPE_BOUND:
    {
        var_gencode(value.variable, result);
        term_unify_gencode(value.term_value, result);
        break;
    }
    default:
        var_gencode(value.variable, result);
        term_gencode(value.term_value, result);
        assert(0);
    }
}

void goal_gencode(goal * value, gencode_result * result)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            goal_literal_gencode(value->literal, result);
            break;
        }
        case GOAL_TYPE_UNIFICATION:
        {
            goal_unification_gencode(value->unification, result);
            break;
        }
        case GOAL_TYPE_UNKNOW:
        {
            assert(0);
        }
    }
}

void goal_list_gencode(goal_list * list, gencode_result * result)
{
    goal * node = list->head;
    while (node != NULL)
    {
        goal_gencode(node, result);
        node = node->next;
    }
}

void clause_gencode(clause * value, gencode_result * result)
{
    goal_list_gencode(value->goals, result);
}

void clause_list_gencode(clause_list * list, gencode_result * result)
{
    clause * node = list->head;
    while (node != NULL)
    {
        clause_gencode(node, result);
        node = node->next;
    }    
}

void query_gencode(query * value, gencode_result * result)
{
    goal_list_gencode(value->goals, result);
}

void program_gencode(program * value, gencode_result * result)
{
    clause_list_gencode(value->clausies, result);
    query_gencode(value->query_value, result);
}

