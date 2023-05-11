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
#include <assert.h>
#include <stdio.h>
#include <string.h>

void var_is_bound_semcheck(symtab * stab, var * value, semcheck_result * result)
{
    symtab_entry * entry = symtab_lookup(stab, value->name, SYMTAB_LOOKUP_LOCAL);
    if (entry == NULL)
    {
        *result = SEMCHECK_FAILURE;
        fprintf(stderr, "%u: var '%s' is not declared\n", value->line_no, value->name);
    }
}

void var_get_unbound_semcheck(var * value, var_list * local_vars)
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

void var_add_to_symtab(symtab * stab, var * var_value, semcheck_result * result)
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

void var_list_add_to_symtab(symtab * stab, var_list * freevars, semcheck_result * result)
{
    var_node * node = freevars->head;
    while (node != NULL)
    {
        var * var_value = node->value;
        if (var_value != NULL)
        {
            var_add_to_symtab(stab, var_value, result);
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
            var_semcheck(stab, freevars, value->t_var.value, result);
        break;
        case TERM_TYPE_LIST:
        case TERM_TYPE_STRUCT:
        {
            symtab_entry * entry = symtab_lookup_arity(stab, value->t_struct.name, term_arity(value), SYMTAB_LOOKUP_GLOBAL);
            if (entry != NULL)
            {
                assert(entry->type == SYMTAB_CLAUSE &&
                       entry->arity == term_arity(value) &&
                       strcmp(entry->id, value->t_struct.name) == 0);
                value->predicate_ref = entry->predicate_value;
            }
            else
            {
                *result = SEMCHECK_FAILURE;
                fprintf(stderr, "%u: unknown predicate '%s/%u'\n", value->line_no, value->t_struct.name, term_arity(value));
            }
            term_list_semcheck(stab, freevars, value->t_struct.terms, result);
        }
        break;
        case TERM_TYPE_LIST_EMPTY:
            /* Empty list is fine */
        break;
        case TERM_TYPE_INT:
            /* Int is fine */
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
        fprintf(stderr, "%u: term %s is not variable\n", value->line_no, term_type_to_str(value->type));
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

void var_get_vars_semcheck(symtab * stab, var * value)
{
    symtab_entry * entry = symtab_lookup(stab, value->name, SYMTAB_LOOKUP_LOCAL);
    if (entry == NULL)
    {
        symtab_add_var(stab, value);
    }
}

void var_list_get_vars_semcheck(symtab * stab, var_list * list)
{
    var_node * node = list->head;
    while (node != NULL)
    {
        var * var_value = node->value;
        if (var_value != NULL)
        {
            var_get_vars_semcheck(stab, var_value);
        }
        node = node->next;
    }
}

void term_get_vars_semcheck(symtab * stab, term * value)
{
    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
        break;
        case TERM_TYPE_ANON:
        break;
        case TERM_TYPE_ATOM:
        break;
        case TERM_TYPE_VAR:
            var_get_vars_semcheck(stab, value->t_var.value);
        break;
        case TERM_TYPE_LIST:
        case TERM_TYPE_STRUCT:
            term_list_get_vars_semcheck(stab, value->t_struct.terms);
        break;
        case TERM_TYPE_LIST_EMPTY:
        break;
        case TERM_TYPE_INT:
        break;
    }
}

void term_get_local_vars_semcheck(term * value, var_list * local_vars)
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
            var_get_unbound_semcheck(value->t_var.value, local_vars);
        break;
        case TERM_TYPE_LIST:
        case TERM_TYPE_STRUCT:
            term_list_get_local_vars_semcheck(value->t_struct.terms, local_vars);
        break;
        case TERM_TYPE_LIST_EMPTY:
        break;
        case TERM_TYPE_INT:
        break;
    }
}

void term_list_get_vars_semcheck(symtab * stab, term_list * list)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_get_vars_semcheck(stab, node);
        node = node->next;
    }
}

void term_list_get_local_vars_semcheck(term_list * list, var_list * local_vars)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_get_local_vars_semcheck(node, local_vars);
        node = node->next;
    }
}

void goal_get_vars_semcheck(symtab * stab, goal * value)
{
    switch (value->type)
    {
        case GOAL_TYPE_UNKNOW:
        break;
        case GOAL_TYPE_LITERAL:
        {
            term_list_get_vars_semcheck(stab, value->literal.terms);
        }
        break;
        case GOAL_TYPE_UNIFICATION:
        {
            var_get_vars_semcheck(stab, value->unification.variable);
            term_get_vars_semcheck(stab, value->unification.term_value);
        }
        break;
        case GOAL_TYPE_CUT:
        case GOAL_TYPE_FAIL:
        break;
    }
}

void goal_literal_get_local_vars_semcheck(goal_literal value, var_list * local_vars)
{
    if (value.terms != NULL)
    {
        term_list_get_local_vars_semcheck(value.terms, local_vars);
    }
}

void goal_get_local_vars_semcheck(goal * value, var_list * local_vars)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            goal_literal_get_local_vars_semcheck(value->literal, local_vars);
            break;
        }
        case GOAL_TYPE_UNIFICATION:
        {
            var_get_unbound_semcheck(value->unification.variable, local_vars);
            break;
        }
        case GOAL_TYPE_CUT:
        case GOAL_TYPE_FAIL:
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

void goal_list_get_vars_from_semcheck(symtab * stab, goal * from_goal)
{
    while (from_goal != NULL)
    {
        goal_get_vars_semcheck(stab, from_goal);
        from_goal = from_goal->next;
    }
}

void goal_literal_semcheck(symtab * stab, goal * goal_value, goal_literal * value, semcheck_result * result)
{
    symtab_entry * entry = symtab_lookup_arity(stab, value->name, term_list_arity(value->terms), SYMTAB_LOOKUP_GLOBAL);
    if (entry != NULL)
    {
        assert(entry->type == SYMTAB_CLAUSE &&
               entry->arity == term_list_arity(value->terms) &&
               strcmp(entry->id, value->name) == 0);
        value->predicate_ref = entry->predicate_value;
    }
    else
    {
        *result = SEMCHECK_FAILURE;
        fprintf(stderr, "%d: cannot find predicate %s/%u\n", goal_value->line_no, value->name, term_list_arity(value->terms));
    }
    var_list * freevars = var_list_new();
    if (value->terms != NULL)
    {
        term_list_semcheck(stab, freevars, value->terms, result);
    }
    var_list_enumerate(freevars, stab->count + 1);
    var_list_add_to_symtab(stab, freevars, result);

    var_list_delete_null(freevars);
}

void goal_unification_semcheck(symtab * stab, goal * goal_value, goal_unification * value, semcheck_result * result)
{
    var_list * freevars = var_list_new();

    var_semcheck(stab, freevars, value->variable, result);
    var_list_enumerate(freevars, stab->count + 1);
    var_list_add_to_symtab(stab, freevars, result);

    var_list_delete_null(freevars);

    freevars = var_list_new();
    term_semcheck(stab, freevars, value->term_value, result);
    if (var_list_size(freevars) > 0)
    {
        *result = SEMCHECK_FAILURE;
        fprintf(stderr, "%d: found free variables in goal unification\n", goal_value->line_no);
        var_list_print(freevars);
    }
    var_list_delete_null(freevars);
}

void goal_cut_semcheck(symtab * stab, char * with_cut, goal * goal_value, goal_cut * value, semcheck_result * result)
{
    *with_cut = 1;
}

void goal_semcheck(symtab * stab, char * with_cut, goal * value, semcheck_result * result)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            goal_literal_semcheck(stab, value, &value->literal, result);
            break;
        }
        case GOAL_TYPE_UNIFICATION:
        {
            goal_unification_semcheck(stab, value, &value->unification, result);
            break;
        }
        case GOAL_TYPE_CUT:
        {
            goal_cut_semcheck(stab, with_cut, value, &value->cut, result);
            break;
        }
        case GOAL_TYPE_FAIL:
        {
            break;
        }
        case GOAL_TYPE_UNKNOW:
        {
            assert(0);
        }
    }
}

void goal_list_get_local_vars_semcheck(goal_list * list, var_list * local_vars)
{
    goal * node = list->head;
    while (node != NULL)
    {
        goal_get_local_vars_semcheck(node, local_vars);
        node = node->next;
    }
}

void goal_list_semcheck(symtab * stab, char * with_cut, goal_list * list, semcheck_result * result)
{
    goal * node = list->head;
    while (node != NULL)
    {
        goal_semcheck(stab, with_cut, node, result);
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

void clause_head_get_local_vars_semcheck(var_list * vars, var_list * local_vars)
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

void clause_semcheck(symtab * stab, clause * value, semcheck_result * result)
{
    if (value->stab == NULL)
    {
        value->stab = symtab_new(16, stab);
    }
    if (value->vars != NULL)
    {
        var_list_enumerate(value->vars, 1);
    }
    if (value->vars != NULL)
    {
        var_list_add_symtab_semcheck(value->stab, value->vars, result);
    }
    if (value->goals != NULL)
    {
        goal_list_semcheck(value->stab, &value->with_cut, value->goals, result);
    }
    if (value->vars != NULL)
    {
        clause_enumerate_vars(value->stab, var_list_size(value->vars) + 1);
        clause_head_get_local_vars_semcheck(value->vars, value->local_vars);
    }
    if (value->goals != NULL)
    {
        goal_list_get_local_vars_semcheck(value->goals, value->local_vars);
    }
}

void clause_list_semcheck(symtab * stab, clause_list * list, semcheck_result * result)
{
    clause_node * node = list->head;
    while (node != NULL)
    {
        if (node->value)
        {
            clause_semcheck(stab, node->value, result);
        }
        node = node->next;
    }    
}

void query_semcheck(symtab * stab, query * value, semcheck_result * result)
{
    if (value->stab == NULL)
    {
        value->stab = symtab_new(16, stab);
    }
    goal_list_semcheck(value->stab, &value->with_cut, value->goals, result);
}

void program_add_clause_semcheck(symtab * stab, clause * value, semcheck_result * result)
{
    symtab_entry * entry = symtab_lookup_arity(stab, value->name, clause_arity(value), SYMTAB_LOOKUP_GLOBAL);
    if (entry == NULL)
    {
        symtab_add_predicate(stab, value);
        value->predicate_ref = value;
    }
    else
    {
        value->predicate_ref = entry->predicate_value;
    }
}

void program_add_predicates_semcheck(symtab * stab, clause_list * list, semcheck_result * result)
{
    clause_node * node = list->head;
    while (node != NULL)
    {
        clause * value = node->value;
        if (value != NULL)
        {
            program_add_clause_semcheck(stab, value, result);
        }
        node = node->next;
    }
}

void program_semcheck(program * value, semcheck_result * result)
{
    program_add_clause_semcheck(value->stab, value->list_clause, result);
    if (value->clausies != NULL)
    {
        program_add_predicates_semcheck(value->stab, value->clausies, result);
        clause_list_semcheck(value->stab, value->clausies, result);
    }
    if (value->query_value != NULL)
    {
        query_semcheck(value->stab, value->query_value, result);
    }
}


