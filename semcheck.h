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
#ifndef __SEMCHECK_H__
#define __SEMCHECK_H__

#include "var.h"
#include "term.h"
#include "goal.h"
#include "clause.h"
#include "query.h"
#include "program.h"
#include "symtab.h"

typedef enum semcheck_result {
    SEMCHECK_SUCCESS = 0,
    SEMCHECK_FAILURE = 1
} semcheck_result;

void var_is_bound_semcheck(symtab * stab, var * value, semcheck_result * result);
void var_get_unbound_semcheck(symtab * stab, var * value, var_list * local_vars);
void var_semcheck(symtab * stab, var_list * freevars, var * value, semcheck_result * result);

void var_add_symtab_semcheck(symtab * stab, var * value, semcheck_result * result);
void var_list_enumerate(var_list * list, unsigned int start);
void var_list_add_symtab_semcheck(symtab * stab, var_list * list, semcheck_result * result);
void var_add_to_symtab(symtab * stab, var * var_value, semcheck_result * result);
void var_list_add_to_symtab(symtab * stab, var_list * freevars, semcheck_result * result);

void term_get_local_vars_semcheck(symtab * stab, term * value, var_list * local_vars);
void term_semcheck(symtab * stab, var_list * freevars, term * value, semcheck_result * result);
void term_is_variable_semcheck(term * value, semcheck_result * result);

void term_list_get_local_vars_semcheck(symtab * stab, term_list * list, var_list * local_vars);
void term_list_semcheck(symtab * stab, var_list * freevars, term_list * list, semcheck_result * result);

void var_get_vars_semcheck(symtab * stab, var * value);
void var_list_get_vars_semcheck(symtab * stab, var_list * list);
void term_get_vars_semcheck(symtab * stab, term * value);
void term_list_get_vars_semcheck(symtab * stab, term_list * list);

void goal_list_get_vars_from_semcheck(symtab * stab, goal * from_goal);

void goal_literal_get_local_vars_semcheck(symtab * stab, goal_literal value, var_list * local_vars);
void goal_literal_semcheck(symtab * stab, goal * goal_value, goal_literal * value, semcheck_result * result);
void goal_unification_semcheck(symtab * stab, goal * goal_value, goal_unification * value, semcheck_result * result);
void goal_cut_semcheck(symtab * stab, char * with_cut, goal * goal_value, goal_cut * value, semcheck_result * result);
void goal_get_local_vars_semcheck(symtab * stab, goal * value, var_list * local_vars);
void goal_semcheck(symtab * stab, char * with_cut, goal * value, semcheck_result * result);
void goal_get_vars_semcheck(symtab * stab, goal * value);

void goal_list_get_local_vars_gencode(symtab * stab, goal_list * list, var_list * local_vars);
void goal_list_semcheck(symtab * stab, char * with_cut, goal_list * list, semcheck_result * result);

void clause_enumerate_vars(symtab * stab, unsigned int start);
void clause_head_get_local_vars_semcheck(var_list * vars, var_list * local_vars);
void clause_semcheck(symtab * stab, clause * value, semcheck_result * result);
void clause_list_semcheck(symtab * stab, clause_list * list, semcheck_result * result);

void query_semcheck(symtab * stab, query * value, semcheck_result * result);

void program_add_clause_semcheck(symtab * stab, clause * value, semcheck_result * result);
void program_add_predicates_semcheck(symtab * stab, clause_list * list, semcheck_result * result);
void program_semcheck(program * value, semcheck_result * result);

#endif /* __SEMCHECK_H__ */
