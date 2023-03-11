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
#include "clause.h"
#include "term.h"
#include "goal.h"

clause * clause_new(char * name, var_list * vars, goal_list * goals)
{
    clause * value = malloc(sizeof(clause));

    value->name = name;
    value->vars = vars;
    value->goals = goals;
    value->stab = NULL;
    value->next = NULL;

    return value;
}

void clause_delete(clause * value)
{
    if (value->name)
    {
        free(value->name);
    }
    if (value->vars)
    {
        var_list_delete(value->vars);
    }
    if (value->goals)
    {
        goal_list_delete(value->goals);
    }
    if (value->stab)
    {
        symtab_delete(value->stab);
    }
    free(value);
}

void clause_deallocator(void * data)
{
    clause_delete((clause *)data);
}

void clause_print(clause * value)
{
    if (value == NULL)
    {
        return;
    }
    printf("clause %s\n", value->name);
    var_list_print(value->vars);
    goal_list_print(value->goals);
}

clause_list * clause_list_new()
{
    clause_list * list = malloc(sizeof(clause_list));
    
    list->head = NULL;
    list->tail = &list->head;

    return list;
}

void clause_list_delete(clause_list * list)
{
    clause * node = list->head;
    while (node != NULL) {
        clause * next = node->next;
        clause_delete(node);
        node = next;
    }
    free(list);
}

void clause_list_add_end(clause_list * list, clause * value)
{
    (*list->tail) = value;
    list->tail = &value->next;
}

void clause_list_print(clause_list * list)
{
    clause * node = list->head;
    while (node != NULL)
    {
        clause_print(node);
        node = node->next;
    }    
}
