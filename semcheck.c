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

void var_is_bound_semcheck(symtab * stab, var * value, semcheck_result * result)
{
    symtab_entry * entry = symtab_lookup(stab, value->name, SYMTAB_LOOKUP_LOCAL);
    if (entry == NULL)
    {
        *result = SEMCHECK_FAILURE;
        fprintf(stderr, "%u: var '%s' is not declared\n", value->line_no, value->name);
    }
}

void var_semcheck(symtab * stab, var_list * freevars, var * value, semcheck_result * result)
{
    if (stab == NULL)
    {
        return;
    }

    symtab_entry * entry = symtab_lookup(stab, value->name, SYMTAB_LOOKUP_LOCAL);
    if (entry != NULL && entry->type == SYMTAB_VAR)
    {
        value->type = VAR_TYPE_BOUND;
        value->bound_to = entry->var_value;
    }
    else
    {
        value->type = VAR_TYPE_UNBOUND;
        value->bound_to = value;
        var_list_add_end(freevars, value);
    }
}

void var_add_symtab_semcheck(symtab * stab, var * value, semcheck_result * result)
{
    symtab_entry * entry = symtab_lookup(stab, value->name, SYMTAB_LOOKUP_LOCAL);
    if (entry != NULL)
    {
        *result = SEMCHECK_FAILURE;
        fprintf(stderr, "%u: var '%s' duplicated\n", value->line_no, value->name);
    }
    else
    {
        value->type = VAR_TYPE_BOUND;
        value->bound_to = value;
        symtab_add_var(stab, value);
    }
}

void var_list_enumerate(var_list * list, unsigned int start)
{
    unsigned int index = start;
    var_node * node = list->head;
    while (node != NULL)
    {
        var * var_value = node->value;
        if (var_value)
        {
            var_value->index = index++;
        }
        node = node->next;
    }    
}

void var_list_add_symtab_semcheck(symtab * stab, var_list * list, semcheck_result * result)
{
    var_node * node = list->head;
    while (node != NULL)
    {
        var * var_value = node->value;
        if (var_value)
        {
            var_add_symtab_semcheck(stab, var_value, result);
        }
        node = node->next;
    }
}

void var_list_add_to_symtab(symtab * stab, var_list * freevars, semcheck_result * result)
{
    var_node * node = freevars->head;
    while (node != NULL)
    {
        var * var_value = node->value;
        if (var_value)
        {
            symtab_entry * entry = symtab_lookup(stab, var_value->name, SYMTAB_LOOKUP_LOCAL);
            if (entry != NULL)
            {
                var_value->bound_to = entry->var_value;
            }
            else
            {
                symtab_add_var(stab, var_value);
            }
        }
        node = node->next;
    }
}

void term_semcheck(symtab * stab, var_list * freevars, term * value, semcheck_result * result)
{
    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
            assert(0);
        break;
        case TERM_TYPE_ANON:
            /* TODO: what should it be ? */
        break;
        case TERM_TYPE_ATOM:
            /* Atom. It is fine */
        break;
        case TERM_TYPE_VAR:
            var_semcheck(stab, freevars, value->var_value, result);
        break;
        case TERM_TYPE_TERM:
            term_list_semcheck(stab, freevars, value->terms, result);
        break;
    }
}

void term_is_variable_semcheck(term * value, semcheck_result * result)
{
    if (value == NULL)
    {
        return;
    }
    if (value->type != TERM_TYPE_VAR)
    {
        fprintf(stderr, "%u: term %s '%s' is not variable\n", value->line_no, term_type_to_str(value->type), value->name);
        *result = SEMCHECK_FAILURE;
    }
}

void term_list_semcheck(symtab * stab, var_list * freevars, term_list * list, semcheck_result * result)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_semcheck(stab, freevars, node, result);
        node = node->next;
    }    
}

void goal_literal_semcheck(symtab * stab, goal_literal value, semcheck_result * result)
{
    var_list * freevars = var_list_new();

    term_list_semcheck(stab, freevars, value.terms, result);
    var_list_enumerate(freevars, stab->count + 1);
    var_list_add_to_symtab(stab, freevars, result);

    var_list_delete_null(freevars);
}

void goal_unification_semcheck(symtab * stab, goal_unification value, semcheck_result * result)
{
    var_list * freevars = var_list_new();

    var_semcheck(stab, freevars, value.variable, result);
    term_semcheck(stab, freevars, value.term_value, result);

    if (var_list_size(freevars) > 0)
    {
        *result = SEMCHECK_FAILURE;
        fprintf(stderr, "found free variables in goal unification\n");
        var_list_print(freevars);
    }

    var_list_delete_null(freevars);
}

void goal_semcheck(symtab * stab, goal * value, semcheck_result * result)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            goal_literal_semcheck(stab, value->literal, result);
            break;
        }
        case GOAL_TYPE_UNIFICATION:
        {
            goal_unification_semcheck(stab, value->unification, result);
            break;
        }
        case GOAL_TYPE_UNKNOW:
        {
            assert(0);
        }
    }
}

void goal_list_semcheck(symtab * stab, goal_list * list, semcheck_result * result)
{
    goal * node = list->head;
    while (node != NULL)
    {
        goal_semcheck(stab, node, result);
        node = node->next;
    }
}

void clause_enumerate_vars(symtab * stab, unsigned int start)
{
    if (stab == NULL)
    {
        return;
    }

    unsigned int index = start;
    for (unsigned int i = 0; i < stab->size; i++)
    {
        if (stab->entries[i].type == SYMTAB_VAR)
        {
            var * var_value = stab->entries[i].var_value;
            if (var_value->index == 0)
            {
                var_value->index = index++;
            }
        }
    }
}

void clause_semcheck(clause * value, semcheck_result * result)
{
    if (value->stab == NULL)
    {
        value->stab = symtab_new(10, NULL);
    }
    if (value->vars != NULL)
    {
        var_list_enumerate(value->vars, 1);
    }
    if (value->vars != NULL)
    {
        var_list_add_symtab_semcheck(value->stab, value->vars, result);
    }
    goal_list_semcheck(value->stab, value->goals, result);
    if (value->vars != NULL)
    {
        clause_enumerate_vars(value->stab, var_list_size(value->vars) + 1);
    }
}

void clause_list_semcheck(clause_list * list, semcheck_result * result)
{
    clause * node = list->head;
    while (node != NULL)
    {
        clause_semcheck(node, result);
        node = node->next;
    }    
}

void query_semcheck(query * value, semcheck_result * result)
{
    if (value->stab == NULL)
    {
        value->stab = symtab_new(10, NULL);
    }
    goal_list_semcheck(value->stab, value->goals, result);
}

void program_semcheck(program * value, semcheck_result * result)
{
    if (value->clausies != NULL)
    {
        clause_list_semcheck(value->clausies, result);
    }
    if (value->query_value != NULL)
    {
        query_semcheck(value->query_value, result);
    }
}


