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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "goal.h"

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
        case GOAL_TYPE_UNKNOW:
            assert(0);
        break;
    }

    free(value);
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
