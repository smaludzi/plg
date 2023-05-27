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
    value->predicate_ref = NULL;
    value->gencode = 0;
    value->with_cut = 0;
    value->is_last = 0;
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
    return var_list_size(value->vars);
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
