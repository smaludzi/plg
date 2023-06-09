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
#ifndef __UNIFICATION_H__
#define __UNIFICATION_H__ 1

typedef struct u_part {
    int multi_eq_number;
    struct multi_equation_list * zero_counter_multi_equation;
    struct multi_equation_list * equations;
} u_part;

typedef struct u_system {
    struct u_part U;
    struct multi_equation_list * T;
} u_system;

typedef struct multi_equation {
	int ref_cnt;
    int counter;
    int var_number;
    struct variable_list * S;
    struct multi_term * M;
} multi_equation;

typedef struct multi_equation_list {
    multi_equation * value;
    struct multi_equation_list * next;
} multi_equation_list;

typedef struct multi_term {
    const char * f_symb;
    struct temp_mult_eq_list * args;
} multi_term;

typedef struct temp_mult_eq {
    struct variable_queue * S;
    multi_term * M;
} temp_mult_eq;

typedef struct temp_mult_eq_list {
    temp_mult_eq * value;
    struct temp_mult_eq_list * next;
} temp_mult_eq_list;

typedef struct variable {
    const char * name;
    struct multi_equation * M;
} variable;

typedef struct variable_list {
    variable * value;
    struct variable_list * next;
} variable_list;

typedef struct variable_queue {
    struct variable_list * head;
    struct variable_list * tail;
} variable_queue;

int unify(u_system * R);
int select_multi_equation(u_part * U, multi_equation ** mult);
int reduce(multi_term * M, temp_mult_eq_list ** frontier);
int compact(temp_mult_eq_list ** frontier, u_part * U);
int merge_mult_eq(u_part * U, multi_equation ** mult, multi_equation ** mult1);
int merge_multi_terms(multi_term ** M, multi_term ** M1);

void frontier_delete(temp_mult_eq_list ** frontier);

u_system * u_system_new();
void u_system_delete(u_system * system);
void u_system_print(u_system * system);

multi_equation * multi_equation_new(multi_term * mult);
void multi_equation_delete(multi_equation * multi);
void multi_equation_print(multi_equation * multi);

multi_equation_list * multi_equation_node_new(multi_equation * value, multi_equation_list * next);
void multi_equation_node_delete(multi_equation_list * multi);

void multi_equation_list_delete(multi_equation_list * multi);
void multi_equation_list_print(multi_equation_list * multi);

multi_term * multi_term_new(const char * f_symb, temp_mult_eq_list * args);
void multi_term_delete(multi_term * multi);
void multi_term_print(multi_term * multi);

temp_mult_eq * temp_mult_eq_new(variable_queue * S, multi_term * M);
void temp_mult_eq_delete(temp_mult_eq * mult);
void temp_mult_eq_print(temp_mult_eq * mult);

temp_mult_eq_list * temp_mult_eq_node_new(temp_mult_eq * value, temp_mult_eq_list * next);
void temp_mult_eq_node_delete(temp_mult_eq_list * multi);

void temp_mult_eq_list_delete(temp_mult_eq_list * multi);
void temp_mult_eq_list_print(temp_mult_eq_list * multi);

variable_queue * variable_queue_new();
variable_queue * variable_queue_new_var(variable * var);
void variable_queue_delete(variable_queue * q);
variable * variable_queue_head(variable_queue * q1);
void variable_queue_add_var(variable_queue * q1, variable * var);
void variable_queue_remove_head(variable_queue * q1);
char variable_queue_is_empty(variable_queue * q1);
void variable_queue_append(variable_queue ** q1, variable_queue ** q2);
void variable_queue_print(variable_queue * q);

variable_list * variable_node_new(variable * value, variable_list * next);
void variable_node_delete(variable_list * node);

void variable_list_delete(variable_list * vars);
int variable_list_size(variable_list * vars);
char variable_list_is_empty(variable_list * vars);
void variable_list_print(variable_list * vars);

variable * variable_new(const char * name);
void variable_delete(variable * var);
void variable_hash_delete(const char * key, void * value);
void variable_inc_mult_eq_counter(variable * var, int inc);
void variable_print(variable * var);
void variable_hash_print(void * data);

#endif /* __UNIFICATION_H__ */

