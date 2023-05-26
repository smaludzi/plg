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
#include <string.h>
#include "builtin.h"

clause * builtin_write()
{
    var_list * vars = var_list_new();
    var_list_add_end(vars, var_new(strdup("X")));

    goal_list * goals = goal_list_new();
    goal_list_add_end(goals, goal_new_builtin(BUILT_IN_WRITE));

    return clause_new(strdup("write"), vars, goals);
}

clause * builtin_nl()
{
    goal_list * goals = goal_list_new();
    goal_list_add_end(goals, goal_new_builtin(BUILT_IN_NL));

    return clause_new(strdup("nl"), NULL, goals);
}

void builtin_add_all(clause_list * clauses)
{
    clause_list_add_end(clauses, builtin_write());
    clause_list_add_end(clauses, builtin_nl());
}




