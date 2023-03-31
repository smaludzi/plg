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
#include "bytecode.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

gencode * gencode_new()
{
    gencode * value = (gencode *)malloc(sizeof(gencode));

    value->list = bytecode_list_new();

    return value;
}

void gencode_delete(gencode * value)
{
    if (value->list)
    {
        bytecode_list_delete(value->list);
    }
    free(value);
}

void gencode_add_bytecode(gencode * value, bytecode * code)
{
    bytecode_list_add_end(value->list, code);
}

void var_gencode(gencode * gen, var * value, gencode_result * result)
{
    switch (value->type)
    {
        case VAR_TYPE_BOUND:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_PUT_REF;
            bc.put_ref.index = value->bound_to->index;
            gencode_add_bytecode(gen, &bc);

            printf("PUT_REF name %s index %d\n", value->name, value->bound_to->index);
        }
        break;
        case VAR_TYPE_UNBOUND:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_PUT_VAR;
            bc.put_ref.index = value->index;
            gencode_add_bytecode(gen, &bc);

            printf("PUT_VAR name %s index %d\n", value->name, value->index);
        }
        break;
        case VAR_TYPE_UNKNOWN:
            printf("PUT_VAR_UNKNOWN!!! %d: %s\n", value->line_no, value->name);
            assert(0);
        break;
    }
}

void var_unify_gencode(gencode * gen, var * value, gencode_result * result)
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

void var_check_gencode(gencode * gen, var * value, gencode_result * result)
{
     printf("CHECK name %s index %d\n", value->name, value->bound_to->index);
}

void var_get_unbound_gencode(gencode * gen, var * value, var_list * local_vars)
{
    switch (value->type)
    {
        case VAR_TYPE_BOUND:
        break;
        case VAR_TYPE_UNBOUND:
            var_list_add_end(local_vars, value);
        break;
        case VAR_TYPE_UNKNOWN:
            assert(0);
        break;
    }
}

void var_get_bound_vars_gencode(gencode * gen, var * value, var_list * bound_vars, gencode_result * result)
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

void var_list_check_gencode(gencode * gen, var_list * bound_vars, gencode_result * result)
{
    var_node * node = bound_vars->head;
    while (node != NULL)
    {
        var * var_value = node->value;
        if (var_value)
        {
            var_check_gencode(gen, var_value, result);
        }
        node = node->next;
    }
}

void term_gencode(gencode * gen, term * value, gencode_result * result)
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
            var_gencode(gen, value->var_value, result);
        break;
        case TERM_TYPE_TERM:
            term_list_gencode(gen, value->terms, result);
            printf("PUT_STRUCT %s/%d\n", value->name, term_list_size(value->terms));
        break;
    }
}

void term_unify_gencode(gencode * gen, term * value, gencode_result * result)
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
            var_unify_gencode(gen, value->var_value, result);
        break;
        case TERM_TYPE_TERM:
        {
            printf("USTRUCT %s/%d\n", value->name, term_list_size(value->terms));
            term_list_unify_gencode(gen, value->terms, result);
            printf("UP B\n");
            printf("A:\n");
            var_list * bound_vars = var_list_new();
            term_list_get_bound_vars_gencode(gen, value->terms, bound_vars, result);
            var_list_check_gencode(gen, bound_vars, result);
            var_list_delete_null(bound_vars);
            term_gencode(gen, value, result);
            printf("BIND\n");
            printf("B:\n");
        }
        break;
    }
}

void term_get_local_vars_gencode(gencode * gen, term * value, var_list * local_vars)
{    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
            assert(0);
        break;
        case TERM_TYPE_ANON:
        break;
        case TERM_TYPE_ATOM:
        break;
        case TERM_TYPE_VAR:
            var_get_unbound_gencode(gen, value->var_value, local_vars);
        break;
        case TERM_TYPE_TERM:
            term_list_get_local_vars_gencode(gen, value->terms, local_vars);
        break;
    }
}

void term_get_bound_vars_gencode(gencode * gen, term * value, var_list * bound_vars, gencode_result * result)
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
            var_get_bound_vars_gencode(gen, value->var_value, bound_vars, result);
        break;
        case TERM_TYPE_TERM:
            term_list_get_bound_vars_gencode(gen, value->terms, bound_vars, result);
        break;
    }
}

void term_list_gencode(gencode * gen, term_list * list, gencode_result * result)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_gencode(gen, node, result);
        node = node->next;
    }
}

void term_list_unify_gencode(gencode * gen, term_list * list, gencode_result * result)
{
    unsigned int son_number = 1;

    term * node = list->head;
    while (node != NULL)
    {
        printf("SON %u\n", son_number++);
        term_unify_gencode(gen, node, result);
        node = node->next;
    }
}

void term_list_get_local_vars_gencode(gencode * gen, term_list * list, var_list * local_vars)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_get_local_vars_gencode(gen, node, local_vars);
        node = node->next;
    }
}

void term_list_get_bound_vars_gencode(gencode * gen, term_list * list, var_list * bound_vars, gencode_result * result)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_get_bound_vars_gencode(gen, node, bound_vars, result);
        node = node->next;
    }
}

void goal_literal_get_local_vars_gencode(gencode * gen, goal_literal value, var_list * local_vars)
{
    if (value.terms != NULL)
    {
        term_list_get_local_vars_gencode(gen, value.terms, local_vars);
    }
}

void goal_literal_gencode(gencode * gen, goal_literal value, gencode_result * result)
{
    printf("MARK B\n");
    if (value.terms != NULL)
    {
        term_list_gencode(gen, value.terms, result);
    }
    printf("CALL %s/%u\n", value.name, term_list_size(value.terms));
    printf("B: ...\n");
}

void goal_unification_gencode(gencode * gen, goal_unification value, gencode_result * result)
{
    switch (value.variable->type)
    {
    case VAR_TYPE_UNBOUND:
    {
        var_gencode(gen, value.variable, result);
        term_gencode(gen, value.term_value, result);
        printf("BIND\n");
        break;
    }
    case VAR_TYPE_BOUND:
    {
        var_gencode(gen, value.variable, result);
        term_unify_gencode(gen, value.term_value, result);
        break;
    }
    default:
        var_gencode(gen, value.variable, result);
        term_gencode(gen, value.term_value, result);
        assert(0);
    }
}

void goal_get_local_vars_gencode(gencode * gen, goal * value, var_list * local_vars)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            goal_literal_get_local_vars_gencode(gen, value->literal, local_vars);
            break;
        }
        case GOAL_TYPE_UNIFICATION:
        {
            /* not possible to get local variables */
            break;
        }
        case GOAL_TYPE_UNKNOW:
        {
            assert(0);
        }
    }
}

void goal_gencode(gencode * gen, goal * value, gencode_result * result)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            goal_literal_gencode(gen, value->literal, result);
            break;
        }
        case GOAL_TYPE_UNIFICATION:
        {
            goal_unification_gencode(gen, value->unification, result);
            break;
        }
        case GOAL_TYPE_UNKNOW:
        {
            assert(0);
        }
    }
}

void goal_list_get_local_vars_gencode(gencode * gen, goal_list * list, var_list * local_vars)
{
    goal * node = list->head;
    while (node != NULL)
    {
        goal_get_local_vars_gencode(gen, node, local_vars);
        node = node->next;
    }
}

void goal_list_gencode(gencode * gen, goal_list * list, gencode_result * result)
{
    goal * node = list->head;
    while (node != NULL)
    {
        goal_gencode(gen, node, result);
        node = node->next;
    }
}

void clause_head_get_local_vars_gencode(gencode * gen, var_list * vars, var_list * local_vars)
{
    var_node * node = vars->head;
    while (node != NULL)
    {
        if (node->value != NULL)
        {
            var_list_add_end(local_vars, node->value);
        }
        node = node->next;
    }
}

void clause_gencode(gencode * gen, clause * value, gencode_result * result)
{
    var_list * local_vars = var_list_new();

    if (value->vars != NULL)
    {
        clause_head_get_local_vars_gencode(gen, value->vars, local_vars);
    }
    if (value->goals != NULL)
    {
        goal_list_get_local_vars_gencode(gen, value->goals, local_vars);
    }

    printf("PUSHENV %u\n", local_vars->size);

    var_list_delete_null(local_vars);

    goal_list_gencode(gen, value->goals, result);

    printf("POPENV\n");
}

void predicate_0_gencode(gencode * gen, clause * value, gencode_result * result)
{
    clause_gencode(gen, value, result);
}

void predicate_N_gencode(gencode * gen, clause_list * list, gencode_result * result)
{
    printf("SETBTP\n");
    unsigned int clause_nbr = 1;
    clause_node * clause = list->head;
    assert(clause);
    clause = clause->next; /* omit one clause */
    while (clause != NULL)
    {
        if (clause->value != NULL)
        {
            printf("TRY A%u\n", clause_nbr++);
        }
        clause = clause->next;
    }
    printf("DELBTP\n");
    printf("JUMP A%u\n", clause_nbr);
    clause_nbr = 1;
    clause = list->head;
    while (clause != NULL)
    {
        if (clause->value != NULL)
        {
            printf("A%u:\n", clause_nbr++);
            clause_gencode(gen, clause->value, result);
        }
        clause = clause->next;
    }
}

void predicate_gencode(gencode * gen, clause_list * list, gencode_result * result)
{
    if (list->size == 1)
    {
        clause_node * node = list->head;
        if (node && node->value != NULL)
        {
            predicate_0_gencode(gen, node->value, result);
        }
    }
    else
    {
        predicate_N_gencode(gen, list, result);
    }
}

void predicate_gather_gencode(gencode * gen, clause_node * first, gencode_result * result)
{
    char first_clause = 0;
    char * predicate_name = NULL;
    unsigned int predicate_arity = 0;
    clause_list * predicates = clause_list_new();

    clause_node * node = first;
    while (node != NULL)
    {
        clause * value = node->value;
        if (value && value->gencode == 0)
        {
            if (first_clause == 0)
            {
                first_clause = 1;
                value->gencode = 1;
                predicate_name = value->name;
                predicate_arity = clause_arity(value);
                clause_list_add_end(predicates, value);
            }
            else
            {
                if (strcmp(value->name, predicate_name) == 0 &&
                    predicate_arity == clause_arity(value))
                {
                    value->gencode = 1;
                    clause_list_add_end(predicates, value);
                }
            }
        }
        node = node->next;
    }

    if (predicates->size > 0)
    {
        predicate_gencode(gen, predicates, result);
    }

    clause_list_delete_null(predicates);
}

void clause_list_gencode(gencode * gen, clause_list * list, gencode_result * result)
{
    clause_node * node = list->head;
    while (node != NULL)
    {
        predicate_gather_gencode(gen, node, result);
        node = node->next;
    }
}

void query_gencode(gencode * gen, query * value, gencode_result * result)
{
    printf("INIT A\n");
    printf("PUSHENV %u\n", symtab_size_type(value->stab, SYMTAB_VAR));
    goal_list_gencode(gen, value->goals, result);
    printf("HALT %u\n", symtab_size_type(value->stab, SYMTAB_VAR));
    printf("A:\n");
    printf("NO\n");
}

void program_gencode(gencode * gen, program * value, gencode_result * result)
{
    query_gencode(gen, value->query_value, result);
    clause_list_gencode(gen, value->clausies, result);
}

