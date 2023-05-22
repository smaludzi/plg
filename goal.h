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
#ifndef __GOAL_H__
#define __GOAL_H__

#include "var.h"
#include "term.h"

typedef struct clause clause;
typedef struct symtab symtab;
typedef struct expr expr;

typedef enum goal_type { 
    GOAL_TYPE_UNKNOW,
    GOAL_TYPE_LITERAL,
    GOAL_TYPE_UNIFICATION,
    GOAL_TYPE_IS,
    GOAL_TYPE_CUT,
    GOAL_TYPE_FAIL
} goal_type;

typedef struct goal_literal {
    char * name;
    term_list * terms;
    clause * predicate_ref;
} goal_literal;

typedef struct goal_unification {
    var * variable;
    term * term_value;
} goal_unification;

typedef struct goal_is {
    var * var_value;
    expr * expr_value;
} goal_is;

typedef struct goal_cut {
} goal_cut;

typedef struct goal_fail {
    char * name;
} goal_fail;

typedef struct goal {
    goal_type type;
    union {
        goal_literal literal;
        goal_unification unification;
        goal_is is;
        goal_cut cut;
        goal_fail fail;
    };
    unsigned int line_no;
    struct goal * next;
} goal;

typedef struct goal_list {
    goal * head;
    goal ** tail;
} goal_list;

goal * goal_new_literal(char * name, term_list * terms);
goal * goal_new_unification(var * variable, term * term_value);
goal * goal_new_is(var * var_value, expr * expr_value);
goal * goal_new_cut();
goal * goal_new_fail(char * name);
void goal_delete(goal * value);
char goal_is_last(goal * value);
void goal_print(goal * value);

goal_list * goal_list_new();
void goal_list_delete(goal_list * list);
void goal_list_add_end(goal_list * list, goal * value);

void goal_list_print(goal_list * list);

#endif /* __GOAL_H__ */
