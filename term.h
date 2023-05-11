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
#ifndef __TERM_H__
#define __TERM_H__ 1

typedef struct var var;
typedef struct clause clause;

typedef enum term_type {
    TERM_TYPE_UNKNOWN = 0,
    TERM_TYPE_ANON = 1,
    TERM_TYPE_ATOM = 2,
    TERM_TYPE_VAR = 3,
    TERM_TYPE_STRUCT = 4,
    TERM_TYPE_LIST_EMPTY = 5,
    TERM_TYPE_LIST = 6,
    TERM_TYPE_INT = 7
} term_type;

typedef struct term {
    term_type type;
    union {
        struct {
            char * name;
        } t_basic;
        struct {
            char * name;
            struct term_list * terms;
        } t_struct;
        struct {
            var * value;
        } t_var;
        struct {
            int value;
        } t_int;
    };
    clause * predicate_ref;
    unsigned int line_no;
    struct term * next;
} term;

typedef struct term_list {
    term * head;
    term ** tail;
    unsigned int size;
} term_list;

term * term_new_basic(term_type type, char * name);
term * term_new_var(term_type, var * var_value);
term * term_new_struct(term_type type, char * name, term_list * terms);
term * term_new_int(term_type type, int int_val);
void term_delete(term * t);

unsigned int term_arity(term * t);
void term_print(term * t);

term_list * term_list_new();
void term_list_delete(term_list * list);
void term_list_add_end(term_list * list, term * value);

unsigned int term_list_arity(term_list * list);
void term_list_print(term_list * list);
unsigned int term_list_size(term_list * list);

char * term_type_to_str(term_type value);

/* private */
void term_print_rec(term * t);
void term_list_print_rec(term_list * list);

#endif /* __TERM_H__ */

