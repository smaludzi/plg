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
#include "program.h"
#include "clause.h"

program * program_new(clause_list * clausies, query * query_value)
{
    program * value = malloc(sizeof(program));

    value->clausies = clausies;
    value->query_value = query_value;

    return value;
}

void program_delete(program * value)
{
    if (value->clausies)
    {
        clause_list_delete(value->clausies);
    }
    if (value->query_value)
    {
        query_delete(value->query_value);
    }
    free(value);
}

void program_print(program * value)
{
    if (value == NULL)
    {
        return;
    }
    clause_list_print(value->clausies);
    query_print(value->query_value);
}

