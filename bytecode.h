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

typedef enum bytecode_type {
    BYTECODE_UNKNOWN = 0,
    BYTECODE_POP = 1,
    BYTECODE_PUT_REF = 2,
    BYTECODE_PUT_VAR = 3,
    BYTECODE_U_REF = 4,
    BYTECODE_U_VAR = 5,
    BYTECODE_CHECK = 6,
    BYTECODE_PUT_ANON,
    BYTECODE_PUT_ATOM,
    BYTECODE_PUT_STRUCT,
    BYTECODE_U_ATOM,
    BYTECODE_U_STRUCT,
    BYTECODE_UP,
    BYTECODE_BIND,
    BYTECODE_SON,
    BYTECODE_MARK,
    BYTECODE_CALL,
    BYTECODE_PUSH_ENV,
    BYTECODE_POP_ENV,
    BYTECODE_SET_BTP,
    BYTECODE_DEL_BTP,
    BYTECODE_TRY,
    BYTECODE_INIT,
    BYTECODE_HALT,
    BYTECODE_NO,
    BYTECODE_JUMP,
    BYTECODE_LABEL,
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
            unsigned int size;
        } put_struct;
        struct {
            unsigned int size;
            int offset;
        } u_struct;
        struct {
            int offset;
        } up;
        struct {
            unsigned int number;
        } son;
        struct {
            int offset;
        } mark;
        struct {
            unsigned int size;
        } push_env;
        struct {
            int offset;
        } try;
        struct {
            unsigned int size;
        } call;
        struct {
            int offset;
        } init;
        struct {
            unsigned int size;
        } halt;
        struct {
            int offset;
        } jump;
    };
} bytecode;

typedef struct bytecode_node {
    bytecode value;
    struct bytecode_node * next;
} bytecode_node;

typedef struct bytecode_list {
    bytecode_node * head;
    bytecode_node ** tail;
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
void bytecode_print_put_struct(bytecode * value);
void bytecode_print_u_atom(bytecode * value);
void bytecode_print_u_struct(bytecode * value);
void bytecode_print_up(bytecode * value);
void bytecode_print_bind(bytecode * value);
void bytecode_print_son(bytecode * value);
void bytecode_print_mark(bytecode * value);
void bytecode_print_call(bytecode * value);
void bytecode_print_push_env(bytecode * value);
void bytecode_print_pop_env(bytecode * value);
void bytecode_print_set_btp(bytecode * value);
void bytecode_print_del_btp(bytecode * value);
void bytecode_print_try(bytecode * value);
void bytecode_print_init(bytecode * value);
void bytecode_print_halt(bytecode * value);
void bytecode_print_no(bytecode * value);
void bytecode_print_jump(bytecode * value);
void bytecode_print_label(bytecode * value);

void bytecode_print(bytecode * value);
void bytecode_print_test();
const char * bytecode_type_str(bytecode_type type);

bytecode_node * bytecode_node_new();
void bytecode_node_delete(bytecode_node * value);

bytecode_list * bytecode_list_new();
void bytecode_list_delete(bytecode_list * list);

bytecode * bytecode_list_add_end(bytecode_list * list, bytecode * value);
void bytecode_list_print(bytecode_list * list);

#endif /* __BYTECODE_H__ */
