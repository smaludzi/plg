/*
 * Copyright (C) 2023 Slawomir Maludzinski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "semcheck.h"

#include <stdio.h>
#include <assert.h>

void term_semcheck(term * value, semcheck_result * result)
{
    printf("term %s\n", value->name);
    term_list_semcheck(value->terms, result);
}

void term_is_variable_semcheck(term * value, semcheck_result * result)
{
    if (value == NULL)
    {
        return;
    }
    if (value->type != TERM_VAR)
    {
        fprintf(stderr, "%u: term %s '%s' is not variable\n", value->line_no, term_type_to_str(value->type), value->name);
        *result = SEMCHECK_FAILURE;
    }
}

void term_list_is_variable_semcheck(List * list, semcheck_result * result)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        term_is_variable_semcheck((term *)list_iterator_data(iter), result);
        list_iterator_next(&iter);
    }    
}

void term_list_semcheck(List * list, semcheck_result * result)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        term_semcheck((term *)list_iterator_data(iter), result);
        list_iterator_next(&iter);
    }    
}

void goal_literal_semcheck(goal_literal value, semcheck_result * result)
{
    printf("%s\n", value.name);
    term_list_semcheck(value.terms, result);
}

void goal_unification_semcheck(goal_unification value, semcheck_result * result)
{
    printf("var %s\n", value.variable);
    term_semcheck(value.term_value, result);
}

void goal_semcheck(goal * value, semcheck_result * result)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            goal_literal_semcheck(value->literal, result);
            break;
        }
        case GOAL_TYPE_UNIFICATION:
        {
            goal_unification_semcheck(value->unification, result);
            break;
        }
        case GOAL_TYPE_UNKNOW:
        {
            assert(0);
        }
    }
}

void goal_list_semcheck(List * list, semcheck_result * result)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        goal_semcheck((goal *)list_iterator_data(iter), result);
        list_iterator_next(&iter);
    }    
}

void clause_semcheck(clause * value, semcheck_result * result)
{
    term_list_is_variable_semcheck(value->terms, result);
    goal_list_semcheck(value->goals, result);
}

void clause_list_semcheck(List * list, semcheck_result * result)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        clause_semcheck((clause *)list_iterator_data(iter), result);
        list_iterator_next(&iter);
    }    
}

void query_semcheck(query * value, semcheck_result * result)
{
    goal_list_semcheck(value->goals, result);
}

void query_list_semcheck(List * list, semcheck_result * result)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        query_semcheck((query *)list_iterator_data(iter), result);
        list_iterator_next(&iter);
    }    
}

void program_semcheck(program * value, semcheck_result * result)
{
    clause_list_semcheck(value->clausies, result);
    query_semcheck(value->query_value, result);
}


