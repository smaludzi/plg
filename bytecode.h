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
#ifndef __BYTECODE_H__
#define __BYTECODE_H__

#include "vm_types.h"

typedef struct clause clause;

typedef enum bytecode_type {
    BYTECODE_UNKNOWN,
    BYTECODE_POP,
    BYTECODE_PUT_REF,
    BYTECODE_PUT_VAR,
    BYTECODE_U_REF,
    BYTECODE_U_VAR,
    BYTECODE_CHECK,
    BYTECODE_PUT_ANON,
    BYTECODE_PUT_ATOM,
    BYTECODE_PUT_INT,
    BYTECODE_PUT_STRUCT,
    BYTECODE_PUT_STRUCT_ADDR,
    BYTECODE_U_ATOM,
    BYTECODE_U_INT,
    BYTECODE_U_STRUCT,
    BYTECODE_U_STRUCT_ADDR,
    BYTECODE_UP,
    BYTECODE_BIND,
    BYTECODE_SON,
    BYTECODE_MARK,
    BYTECODE_LAST_MARK,
    BYTECODE_CALL,
    BYTECODE_CALL_ADDR,
    BYTECODE_LAST_CALL,
    BYTECODE_LAST_CALL_ADDR,
    BYTECODE_PUSH_ENV,
    BYTECODE_POP_ENV,
    BYTECODE_SET_BTP,
    BYTECODE_DEL_BTP,
    BYTECODE_TRY,
    BYTECODE_PRUNE,
    BYTECODE_SET_CUT,
    BYTECODE_FAIL,
    BYTECODE_INIT,
    BYTECODE_HALT,
    BYTECODE_NO,
    BYTECODE_JUMP,
    BYTECODE_LABEL,
    BYTECODE_INT_NEG,
    BYTECODE_INT_ADD,
    BYTECODE_INT_SUB,
    BYTECODE_INT_MUL,
    BYTECODE_INT_DIV,
    BYTECODE_BUILTIN,
    BYTECODE_END
} bytecode_type;

typedef struct bytecode {
    bytecode_type type;
    unsigned int addr;
    union {
        struct {
            unsigned int index;
        } put_ref;
        struct {
            unsigned int index;
        } put_var;
        struct {
            unsigned int index;
        } u_ref;
        struct {
            unsigned int index;
        } u_var;
        struct {
            unsigned int index;
        } check;
        struct {
            atom_idx_t idx;
        } put_atom;
        struct {
            int value;
        } put_int;
        struct {
            unsigned int n;
            union {
                pc_ptr addr;
                clause * predicate_ref;
            };
        } put_struct;
        struct {
            atom_idx_t idx;
        } u_atom;
        struct {
            int value;
        } u_int;
        struct {
            pc_offset offset;
            unsigned int n;
            union {
                pc_ptr addr;
                clause * predicate_ref;
            };
        } u_struct;
        struct {
            pc_offset offset;
        } up;
        struct {
            unsigned int number;
        } son;
        struct {
            pc_offset offset;
        } mark;
        struct {
            unsigned int size;
        } push_env;
        struct {
            pc_offset offset;
        } try;
        struct {
            unsigned int n;
            union {
                pc_ptr addr;
                clause * predicate_ref;
            };
        } call;
        struct {
            unsigned int size;
            unsigned int n;
            union {
                pc_ptr addr;
                clause * predicate_ref;
            };
        } last_call;
        struct {
            pc_offset offset;
        } init;
        struct {
            unsigned int size;
        } halt;
        struct {
            pc_offset offset;
        } jump;
        struct {
            unsigned int id;
        } builtin;
    };
} bytecode;

typedef struct bytecode_node {
    bytecode value;
    struct bytecode_node * next;
} bytecode_node;

typedef struct bytecode_list {
    bytecode_node * head;
    bytecode_node ** tail;
    unsigned int size;
} bytecode_list;

typedef struct bytecode_print_func {
    bytecode_type type;
    void (*print)(bytecode * value);
} bytecode_print_func;

bytecode * bytecode_new_pop();
void bytecode_delete(bytecode * value);

void bytecode_print_unknown(bytecode * value);
void bytecode_print_pop(bytecode * value);
void bytecode_print_put_ref(bytecode * value);
void bytecode_print_put_var(bytecode * value);
void bytecode_print_u_ref(bytecode * value);
void bytecode_print_u_var(bytecode * value);
void bytecode_print_check(bytecode * value);
void bytecode_print_put_anon(bytecode * value);
void bytecode_print_put_atom(bytecode * value);
void bytecode_print_put_int(bytecode * value);
void bytecode_print_put_struct(bytecode * value);
void bytecode_print_put_struct_addr(bytecode * value);
void bytecode_print_u_atom(bytecode * value);
void bytecode_print_u_int(bytecode * value);
void bytecode_print_u_struct(bytecode * value);
void bytecode_print_u_struct_addr(bytecode * value);
void bytecode_print_up(bytecode * value);
void bytecode_print_bind(bytecode * value);
void bytecode_print_son(bytecode * value);
void bytecode_print_mark(bytecode * value);
void bytecode_print_last_mark(bytecode * value);
void bytecode_print_call(bytecode * value);
void bytecode_print_call_addr(bytecode * value);
void bytecode_print_last_call(bytecode * value);
void bytecode_print_last_call_addr(bytecode * value);
void bytecode_print_push_env(bytecode * value);
void bytecode_print_pop_env(bytecode * value);
void bytecode_print_set_btp(bytecode * value);
void bytecode_print_del_btp(bytecode * value);
void bytecode_print_try(bytecode * value);
void bytecode_print_prune(bytecode * value);
void bytecode_print_set_cut(bytecode * value);
void bytecode_print_fail(bytecode * value);
void bytecode_print_init(bytecode * value);
void bytecode_print_halt(bytecode * value);
void bytecode_print_no(bytecode * value);
void bytecode_print_jump(bytecode * value);
void bytecode_print_label(bytecode * value);
void bytecode_print_int_neg(bytecode * value);
void bytecode_print_int_add(bytecode * value);
void bytecode_print_int_sub(bytecode * value);
void bytecode_print_int_mul(bytecode * value);
void bytecode_print_int_div(bytecode * value);
void bytecode_print_builtin(bytecode * value);

void bytecode_print(bytecode * value);
void bytecode_print_test();
const char * bytecode_type_str(bytecode_type type);

bytecode_node * bytecode_node_new();
void bytecode_node_delete(bytecode_node * value);

bytecode_list * bytecode_list_new();
void bytecode_list_delete(bytecode_list * list);

void bytecode_list_set_addr(bytecode_list * list);
bytecode * bytecode_list_add_end(bytecode_list * list, bytecode * value);
void bytecode_list_print(bytecode_list * list);

void bytecode_to_array(bytecode_list * code, bytecode ** code_arr,
                       unsigned int * code_size);
void bytecode_array_delete(bytecode * code_arr);
void bytecode_array_print(bytecode * code_arr, unsigned int size);

#endif /* __BYTECODE_H__ */
