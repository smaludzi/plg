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
    value->local_vars = var_list_new();
    value->goals = goals;
    value->stab = NULL;
    value->predicate_ref = NULL;
    value->gencode = 0;
    value->with_cut = 0;
    value->addr = 0;
    value->line_no = 0;

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
    if (value->local_vars)
    {
        var_list_delete_null(value->local_vars);
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

unsigned int clause_arity(clause * value)
{
    if (value == NULL || value->vars == NULL)
    {
        return 0;
    }
    return value->vars->size;
}

void clause_print(clause * value)
{
    if (value == NULL)
    {
        return;
    }
    printf("clause %s\n", value->name);
    if (value->vars != NULL)
    {
        var_list_print(value->vars);
    }
    if (value->goals != NULL)
    {
        goal_list_print(value->goals);
    }
}

clause_node * clause_node_new(clause * value)
{
    clause_node * node = (clause_node *)malloc(sizeof(clause_node));

    node->value = value;
    node->next = NULL;

    return node;
}

void clause_node_delete(clause_node * value)
{
    if (value->value)
    {
        clause_delete(value->value);
    }
    free(value);
}

void clause_node_delete_null(clause_node * value)
{
    free(value);
}

clause_list * clause_list_new()
{
    clause_list * list = malloc(sizeof(clause_list));
    
    list->head = NULL;
    list->tail = &list->head;
    list->size = 0;

    return list;
}

void clause_list_delete(clause_list * list)
{
    clause_node * node = list->head;
    while (node != NULL) {
        clause_node * next = node->next;
        clause_node_delete(node);
        node = next;
    }
    free(list);
}

void clause_list_delete_null(clause_list * list)
{
    clause_node * node = list->head;
    while (node != NULL) {
        clause_node * next = node->next;
        clause_node_delete_null(node);
        node = next;
    }
    free(list);
}

void clause_list_add_end(clause_list * list, clause * value)
{
    clause_node * node = clause_node_new(value);
    *list->tail = node;
    list->tail = &node->next;
    list->size++;
}

void clause_list_print(clause_list * list)
{
    clause_node * node = list->head;
    while (node != NULL)
    {
        if (node->value)
        {
            clause_print(node->value);
        }
        node = node->next;
    }
}
