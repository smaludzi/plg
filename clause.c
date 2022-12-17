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

clause * clause_new(char * name, List * terms, List * goals)
{
    clause * value = malloc(sizeof(clause));

    value->name = name;
    value->terms = terms;
    value->goals = goals;
    value->stab = NULL;

    return value;
}

void clause_delete(clause * value)
{
    if (value == NULL)
    {
        return;
    }
    free(value->name);
    list_delete(value->terms);
    list_delete(value->goals);
    symtab_delete(value->stab);
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
    printf("%s", value->name);
    var_list_print(value->terms);
    goal_list_print(value->goals);
}

void clause_list_print(List * list)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        clause_print((clause *)list_iterator_data(iter));
        list_iterator_next(&iter);
    }    
}
