/**
 * Copyright 2023 Slawomir Maludzinski
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "goal.h"
#include "clause.h"
#include "expr.h"

goal * goal_new_literal(char * name, term_list * terms)
{
    goal * value = malloc(sizeof(goal));

    value->type = GOAL_TYPE_LITERAL;
    value->literal.name = name;
    value->literal.terms = terms;
    value->literal.predicate_ref = NULL;
    value->line_no = 0;
    value->next = NULL;

    return value;
}

goal * goal_new_unification(var * variable, term * term_value)
{
    goal * value = malloc(sizeof(goal));

    value->type = GOAL_TYPE_UNIFICATION;
    value->unification.variable = variable;
    value->unification.term_value = term_value;
    value->line_no = 0;
    value->next = NULL;

    return value;
}

goal * goal_new_is(var * var_value, expr * expr_value)
{
    goal * value = malloc(sizeof(goal));

    value->type = GOAL_TYPE_IS;
    value->is.var_value = var_value;
    value->is.expr_value = expr_value;
    value->line_no = 0;
    value->next = NULL;

    return value;
}

goal * goal_new_cut()
{
    goal * value = malloc(sizeof(goal));

    value->type = GOAL_TYPE_CUT;
    value->line_no = 0;
    value->next = NULL;

    return value;
}

goal * goal_new_fail(char * name)
{
    goal * value = malloc(sizeof(goal));

    value->type = GOAL_TYPE_FAIL;
    value->fail.name = name;
    value->line_no = 0;
    value->next = NULL;

    return value;
}

goal * goal_new_builtin(unsigned int id)
{
    goal * value = malloc(sizeof(goal));

    value->type = GOAL_TYPE_BUILTIN;
    value->builtin.id = id;
    value->line_no = 0;
    value->next = NULL;

    return value;
}

void goal_delete(goal * value)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
            if (value->literal.name)
            {
                free(value->literal.name);
            }
            if (value->literal.terms)
            {
                term_list_delete(value->literal.terms);
            }
        break;
        case GOAL_TYPE_UNIFICATION:
            if (value->unification.variable)
            {
                var_delete(value->unification.variable);
            }
            if (value->unification.term_value)
            {
                term_delete(value->unification.term_value);
            }
        break;
        case GOAL_TYPE_IS:
            if (value->is.var_value)
            {
                var_delete(value->is.var_value);
            }
            if (value->is.expr_value)
            {
                expr_delete(value->is.expr_value);
            }
        break;
        case GOAL_TYPE_CUT:
        break;
        case GOAL_TYPE_FAIL:
            if (value->fail.name)
            {
                free(value->fail.name);
            }
        break;
        case GOAL_TYPE_BUILTIN:
        break;
        case GOAL_TYPE_UNKNOW:
            assert(0);
        break;
    }

    free(value);
}

char goal_is_last(goal * value)
{
    if (value->next == NULL)
    {
        return 1;
    }
    return 0;
}

void goal_print(goal * value)
{
    if (value == NULL)
    {
        return;
    }

    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            unsigned arrity = 0;
            if (value->literal.terms != NULL)
            {
                arrity = value->literal.terms->size;
            }
            printf("goal literal %s/%u\n", value->literal.name, arrity);
            term_list_print(value->literal.terms);
            printf("\n");
        }
        break;
        case GOAL_TYPE_UNIFICATION:
        {
            printf("goal unification\n");
            var_print(value->unification.variable);
            term_print(value->unification.term_value);
            printf("\n");
        }
        break;
        case GOAL_TYPE_IS:
        {
            if (value->is.var_value)
            {
                var_print(value->is.var_value);
            }
            if (value->is.expr_value)
            {
                expr_print(value->is.expr_value);
            }
        }
        break;
        case GOAL_TYPE_CUT:
        {
            printf("goal cut\n");
        }
        break;
        case GOAL_TYPE_FAIL:
        {
            printf("goal fail\n");
        }
        break;
        case GOAL_TYPE_BUILTIN:
        {
            printf("goal builtin %u\n", value->builtin.id);
        }
        break;
        case GOAL_TYPE_UNKNOW:
        {
            assert(0);
        }
        break;
    }
}

goal_list * goal_list_new()
{
    goal_list * list = NULL;

    list = (goal_list *)malloc(sizeof(goal_list));
    list->head = NULL;
    list->tail = &list->head;

    return list;
}

void goal_list_delete(goal_list * list)
{
    goal * node = list->head;
    while (node != NULL)
    {
        goal * next = node->next;
        goal_delete(node);
        node = next;
    }
    free(list);
}

void goal_list_add_end(goal_list * list, goal * value)
{
    *(list->tail) = value;
    list->tail = &value->next;
}

void goal_list_print(goal_list * list)
{
    if (list == NULL)
    {
        return;
    }
    goal * node = list->head;
    while (node)
    {
        goal_print(node);
        node = node->next;
    }
}
