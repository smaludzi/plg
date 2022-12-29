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
            printf("PUT_VAR name %s index %d\n", value->name, value->bound_to->index);
        break;
        case VAR_TYPE_UNBOUND:
            printf("PUT_REF name %s index %d\n", value->name, value->bound_to->index);
        break;
        case VAR_TYPE_UNKNOWN:
            printf("PUT_VAR_UNKNOWN!!! %d: %s\n", value->line_no, value->name);
        break;
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
            printf("PUT_STRUCT %s/%d\n", value->name, list_size(value->terms));
        break;
    }
}

void term_list_gencode(List * list, gencode_result * result)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        term_gencode((term *)list_iterator_data(iter), result);
        list_iterator_next(&iter);
    }    
}

void goal_literal_gencode(goal_literal value, gencode_result * result)
{
    printf("MARK B\n");
    term_list_gencode(value.terms, result);
    printf("CALL %s/%u\n", value.name, list_size(value.terms));
    printf("B: ...\n");
}

void goal_unification_gencode(goal_unification value, gencode_result * result)
{
    term_gencode(value.term_value, result);
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

void goal_list_gencode(List * list, gencode_result * result)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        goal_gencode((goal *)list_iterator_data(iter), result);
        list_iterator_next(&iter);
    }    
}

void clause_gencode(clause * value, gencode_result * result)
{
    goal_list_gencode(value->goals, result);
}

void clause_list_gencode(List * list, gencode_result * result)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        clause_gencode((clause *)list_iterator_data(iter), result);
        list_iterator_next(&iter);
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

