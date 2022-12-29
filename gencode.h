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
#include "list.h"

typedef enum gencode_result {
    GENCODE_SUCCESS = 0,
    GENCODE_FAILURE = 1
} gencode_result;

void var_gencode(var * value, gencode_result * result);
void term_gencode(term * value, gencode_result * result);
void term_list_gencode(List * list, gencode_result * result);
void goal_literal_gencode(goal_literal value, gencode_result * result);
void goal_unification_gencode(goal_unification value, gencode_result * result);
void goal_gencode(goal * value, gencode_result * result);
void goal_list_gencode(List * list, gencode_result * result);
void clause_gencode(clause * value, gencode_result * result);
void clause_list_gencode(List * list, gencode_result * result);
void query_gencode(query * value, gencode_result * result);
void program_gencode(program * value, gencode_result * result);

#endif /* __GENCODE_H__ */
