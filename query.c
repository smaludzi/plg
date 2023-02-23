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
#include "query.h"

query * query_new(goal_list * goals)
{
    query * value = malloc(sizeof(query));
    value->goals = goals;
    value->stab = NULL;
    return value;
}

void query_delete(query * value)
{
    if (value == NULL)
    {
        return;
    }
    goal_list_delete(value->goals);
    symtab_delete(value->stab);
    free(value);
}

void query_deallocator(void * value)
{
    query_delete((query *)value);
}

void query_print(query * value)
{
    goal_list_print(value->goals);
}
