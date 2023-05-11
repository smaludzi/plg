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
#include <string.h>
#include "program.h"
#include "clause.h"

program * program_new(clause_list * clausies, query * query_value)
{
    program * value = malloc(sizeof(program));

    var_list * vars = var_list_new();
    var_list_add_end(vars, var_new(strdup("X")));
    var_list_add_end(vars, var_new(strdup("Y")));

    clause * list_clause = clause_new(strdup("[|]"), vars, NULL);

    value->stab = symtab_new(32, NULL);
    value->list_clause = list_clause;
    value->clausies = clausies;
    value->query_value = query_value;

    return value;
}

void program_delete(program * value)
{
    if (value->stab)
    {
        symtab_delete(value->stab);
    }
    if (value->list_clause)
    {
        clause_delete(value->list_clause);
    }
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

