/** 
 * Copyright 2021 Slawomir Maludzinski
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
#ifndef __GENCODE_H__
#define __GENCODE_H__

#include "program.h"
#include "query.h"
#include "clause.h"
#include "goal.h"
#include "term.h"
#include "bytecode.h"
#include "strtab.h"

typedef enum gencode_result {
    GENCODE_SUCCESS = 0,
    GENCODE_FAILURE = 1
} gencode_result;

typedef struct gencode {
    strtab * strtab_value;

    unsigned int current_addr;
    bytecode_list * list;
} gencode;

typedef struct gencode_binary {
    char ** strtab_array;
    unsigned int strtab_size;

    bytecode * code_array;
    unsigned int code_size;
} gencode_binary;

gencode * gencode_new();
void gencode_delete(gencode * value);

gencode_binary * gencode_binary_new();
void gencode_binary_delete(gencode_binary * value);
void gencode_binary_generate(gencode_binary * value, gencode * gen);

bytecode * gencode_add_bytecode(gencode * value, bytecode * code);

void var_gencode(gencode * gen, var * value, gencode_result * result);
void var_unify_gencode(gencode * gen, var * value, gencode_result * result);
void var_check_gencode(gencode * gen, var * value, gencode_result * result);
void var_get_unbound_gencode(gencode * gen, var * value, var_list * local_vars);
void var_get_bound_vars_gencode(gencode * gen, var * value, var_list * bound_vars, gencode_result * result);
void var_list_check_gencode(gencode * gen, var_list * bound_vars, gencode_result * result);
void term_gencode(gencode * gen, term * value, gencode_result * result);
void term_unify_gencode(gencode * gen, term * value, gencode_result * result);
void term_get_local_vars_gencode(gencode * gen, term * value, var_list * local_vars);
void term_get_bound_vars_gencode(gencode * gen, term * value, var_list * bound_vars, gencode_result * result);
void term_list_gencode(gencode * gen, term_list * list, gencode_result * result);
void term_list_unify_gencode(gencode * gen, term_list * list, gencode_result * result);
void term_list_get_local_vars_gencode(gencode * gen, term_list * list, var_list * local_vars);
void term_list_get_bound_vars_gencode(gencode * gen, term_list * list, var_list * bound_vars, gencode_result * result);
void goal_list_get_local_vars_gencode(gencode * gen, goal_list * list, var_list * local_vars);
void goal_literal_get_local_vars_gencode(gencode * gen, goal_literal value, var_list * local_vars);
void goal_literal_gencode(gencode * gen, goal_literal * value, gencode_result * result);
void goal_unification_gencode(gencode * gen, goal_unification * value, gencode_result * result);
void goal_cut_gencode(gencode * gen, goal_cut * value, gencode_result * result);
void goal_fail_gencode(gencode * gen, goal * goal, gencode_result * result);
void goal_get_local_vars_gencode(gencode * gen, goal * value, var_list * local_vars);
void goal_gencode(gencode * gen, goal * value, gencode_result * result);
void goal_list_gencode(gencode * gen, goal_list * list, gencode_result * result);
void clause_head_get_local_vars_gencode(gencode * gen, var_list * vars, var_list * local_vars);
void clause_gencode(gencode * gen, clause * value, gencode_result * result);
void predicate_0_gencode(gencode * gen, clause * value, gencode_result * result);
void predicate_N_gencode(gencode * gen, clause_list * list, gencode_result * result);
void predicate_gencode(gencode * gen, clause_list * list, gencode_result * result);
void predicate_gather_gencode(gencode * gen, clause_node * first, gencode_result * result);
void clause_list_gencode(gencode * gen, clause_list * list, gencode_result * result);
void query_gencode(gencode * gen, query * value, gencode_result * result);
void program_gencode(gencode * gen, program * value, gencode_result * result);

#endif /* __GENCODE_H__ */
