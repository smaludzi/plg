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
#ifndef __CLAUSE_H__
#define __CLAUSE_H__

#include "symtab.h"
#include "goal.h"

typedef struct clause {
    char * name;
    var_list * vars;
    goal_list * goals;
    symtab * stab;
    struct clause * predicate_ref;
    char gencode;
    char with_cut;
    char is_last;
    unsigned int addr;
    unsigned int line_no;
} clause;

typedef struct clause_node {
    clause * value;
    struct clause_node * next;
} clause_node;

typedef struct clause_list {
    clause_node * head;
    clause_node ** tail;
    unsigned int size;
} clause_list;

clause * clause_new(char * name, var_list * vars, goal_list * goals);
void clause_delete(clause * value);

unsigned int clause_arity(clause * value);

void clause_print(clause * value);

clause_node * clause_node_new(clause * value);
void clause_node_delete(clause_node * value);
void clause_node_delete_null(clause_node * value);

clause_list * clause_list_new();
void clause_list_delete(clause_list * list);
void clause_list_delete_null(clause_list * list);
void clause_list_add_end(clause_list * list, clause * value);

void clause_list_print(clause_list * list);

#endif /* __CLAUSE_H__ */
