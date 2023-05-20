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
#include "bytecode.h"
#include "clause.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

bytecode_print_func bytecode_print_arr[] = {
    { BYTECODE_UNKNOWN, bytecode_print_unknown },
    { BYTECODE_POP, bytecode_print_pop },
    { BYTECODE_PUT_REF, bytecode_print_put_ref },
    { BYTECODE_PUT_VAR, bytecode_print_put_var },
    { BYTECODE_U_REF, bytecode_print_u_ref },
    { BYTECODE_U_VAR, bytecode_print_u_var },
    { BYTECODE_CHECK, bytecode_print_check },
    { BYTECODE_PUT_ANON, bytecode_print_put_anon },
    { BYTECODE_PUT_ATOM, bytecode_print_put_atom },
    { BYTECODE_PUT_INT, bytecode_print_put_int },
    { BYTECODE_PUT_STRUCT, bytecode_print_put_struct },
    { BYTECODE_PUT_STRUCT_ADDR, bytecode_print_put_struct_addr },
    { BYTECODE_U_ATOM, bytecode_print_u_atom },
    { BYTECODE_U_INT, bytecode_print_u_int },
    { BYTECODE_U_STRUCT, bytecode_print_u_struct },
    { BYTECODE_U_STRUCT_ADDR, bytecode_print_u_struct_addr },
    { BYTECODE_UP, bytecode_print_up },
    { BYTECODE_BIND, bytecode_print_bind },
    { BYTECODE_SON, bytecode_print_son },
    { BYTECODE_MARK, bytecode_print_mark },
    { BYTECODE_LAST_MARK, bytecode_print_last_mark },
    { BYTECODE_CALL, bytecode_print_call },
    { BYTECODE_CALL_ADDR, bytecode_print_call_addr },
    { BYTECODE_LAST_CALL, bytecode_print_last_call },
    { BYTECODE_LAST_CALL_ADDR, bytecode_print_last_call_addr },
    { BYTECODE_PUSH_ENV, bytecode_print_push_env },
    { BYTECODE_POP_ENV, bytecode_print_pop_env },
    { BYTECODE_SET_BTP, bytecode_print_set_btp },
    { BYTECODE_DEL_BTP, bytecode_print_del_btp },
    { BYTECODE_TRY, bytecode_print_try },
    { BYTECODE_PRUNE, bytecode_print_prune },
    { BYTECODE_SET_CUT, bytecode_print_set_cut },
    { BYTECODE_FAIL, bytecode_print_fail },
    { BYTECODE_INIT, bytecode_print_init },
    { BYTECODE_HALT, bytecode_print_halt },
    { BYTECODE_NO, bytecode_print_no },
    { BYTECODE_JUMP, bytecode_print_jump },
    { BYTECODE_LABEL, bytecode_print_label }
};

bytecode * bytecode_new()
{
    bytecode * value = (bytecode *)malloc(sizeof(bytecode));

    bytecode_print_test();

    return value;
}

void bytecode_delete(bytecode * value)
{
    free(value);
}

void bytecode_print_unknown(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_pop(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_put_ref(bytecode * value)
{
    printf("%d: %s index %u\n", value->addr, bytecode_type_str(value->type), value->put_ref.index);
}

void bytecode_print_put_var(bytecode * value)
{
    printf("%d: %s index %u\n", value->addr, bytecode_type_str(value->type), value->put_var.index);
}

void bytecode_print_u_ref(bytecode * value)
{
    printf("%d: %s index %u\n", value->addr, bytecode_type_str(value->type), value->u_ref.index);
}

void bytecode_print_u_var(bytecode * value)
{
    printf("%d: %s index %u\n", value->addr, bytecode_type_str(value->type), value->u_var.index);
}

void bytecode_print_check(bytecode * value)
{
    printf("%d: %s index %u\n", value->addr, bytecode_type_str(value->type), value->check.index);
}

void bytecode_print_put_anon(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_put_atom(bytecode * value)
{
    printf("%d: %s index %u\n", value->addr, bytecode_type_str(value->type), value->put_atom.idx);
}

void bytecode_print_put_int(bytecode * value)
{
    printf("%d: %s value %d\n", value->addr, bytecode_type_str(value->type), value->put_int.value);
}

void bytecode_print_put_struct(bytecode * value)
{
    printf("%d: %s %s/%u\n", value->addr, bytecode_type_str(value->type),
            value->put_struct.predicate_ref->name, clause_arity(value->put_struct.predicate_ref));
}

void bytecode_print_put_struct_addr(bytecode * value)
{
    printf("%d: %s addr %u n %u\n", value->addr, bytecode_type_str(value->type),
           value->put_struct.addr, value->put_struct.n);
}

void bytecode_print_u_atom(bytecode * value)
{
    printf("%d: %s idx %u\n", value->addr, bytecode_type_str(value->type), value->u_atom.idx);
}

void bytecode_print_u_int(bytecode * value)
{
    printf("%d: %s value %d\n", value->addr, bytecode_type_str(value->type), value->u_int.value);
}

void bytecode_print_u_struct(bytecode * value)
{
    printf("%d: %s %s/%u offset %d\n", value->addr, bytecode_type_str(value->type),
           value->u_struct.predicate_ref->name, clause_arity(value->u_struct.predicate_ref), value->u_struct.offset);
}

void bytecode_print_u_struct_addr(bytecode * value)
{
    printf("%d: %s addr %u n %u offset %d\n", value->addr, bytecode_type_str(value->type),
            value->u_struct.addr, value->u_struct.n, value->u_struct.offset);
}

void bytecode_print_up(bytecode * value)
{
    printf("%d: %s up %d\n", value->addr, bytecode_type_str(value->type), value->up.offset);
}

void bytecode_print_bind(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_son(bytecode * value)
{
    printf("%d: %s number %d\n", value->addr, bytecode_type_str(value->type), value->son.number);
}

void bytecode_print_mark(bytecode * value)
{
    printf("%d: %s addr %d\n", value->addr, bytecode_type_str(value->type), value->mark.offset);
}

void bytecode_print_last_mark(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_call(bytecode * value)
{
    printf("%d: %s %s/%u\n", value->addr, bytecode_type_str(value->type),
           value->call.predicate_ref->name, clause_arity(value->call.predicate_ref));
}

void bytecode_print_call_addr(bytecode * value)
{
    printf("%d: %s addr %u n %u\n", value->addr, bytecode_type_str(value->type), value->call.addr, value->call.n);
}

void bytecode_print_last_call(bytecode * value)
{
    printf("%d: %s %s/%u size %u\n", value->addr, bytecode_type_str(value->type),
           value->last_call.predicate_ref->name, clause_arity(value->last_call.predicate_ref),
           value->last_call.size);
}

void bytecode_print_last_call_addr(bytecode * value)
{
    printf("%d: %s addr %u n %u size %u\n", value->addr, bytecode_type_str(value->type),
           value->last_call.addr, value->last_call.n, value->last_call.size);
}

void bytecode_print_push_env(bytecode * value)
{
    printf("%d: %s size %u\n", value->addr, bytecode_type_str(value->type), value->push_env.size);
}

void bytecode_print_pop_env(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_set_btp(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_del_btp(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_try(bytecode * value)
{
    printf("%d: %s addr %d\n", value->addr, bytecode_type_str(value->type), value->try.offset);
}

void bytecode_print_prune(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_set_cut(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_fail(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_init(bytecode * value)
{
    printf("%d: %s addr %d\n", value->addr, bytecode_type_str(value->type), value->init.offset);
}

void bytecode_print_halt(bytecode * value)
{
    printf("%d: %s size %u\n", value->addr, bytecode_type_str(value->type), value->halt.size);
}

void bytecode_print_no(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_jump(bytecode * value)
{
    printf("%d: %s offset %d(%d)\n", value->addr, bytecode_type_str(value->type), value->jump.offset, value->addr + value->jump.offset);
}

void bytecode_print_label(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_test()
{
    unsigned int i = 0;
    for (i = 0; i < BYTECODE_END; i++)
    {
        assert(i == bytecode_print_arr[i].type);
    }
}

void bytecode_print(bytecode * value)
{
    bytecode_print_arr[value->type].print(value);
}

const char * bytecode_type_str(bytecode_type type)
{
    switch (type)
    {
        case BYTECODE_UNKNOWN: return "BYTECODE_UNKNOWN";
        case BYTECODE_POP: return "BYTECODE_POP";
        case BYTECODE_PUT_REF: return "BYTECODE_PUT_REF";
        case BYTECODE_PUT_VAR: return "BYTECODE_PUT_VAR";
        case BYTECODE_U_REF: return "BYTECODE_U_REF";
        case BYTECODE_U_VAR: return "BYTECODE_U_VAR";
        case BYTECODE_CHECK: return "BYTECODE_CHECK";
        case BYTECODE_PUT_ANON: return "BYTECODE_PUT_ANON";
        case BYTECODE_PUT_ATOM: return "BYTECODE_PUT_ATOM";
        case BYTECODE_PUT_INT: return "BYTECODE_PUT_INT";
        case BYTECODE_PUT_STRUCT: return "BYTECODE_PUT_STRUCT";
        case BYTECODE_PUT_STRUCT_ADDR: return "BYTECODE_PUT_STRUCT_ADDR";
        case BYTECODE_U_ATOM: return "BYTECODE_U_ATOM";
        case BYTECODE_U_INT: return "BYTECODE_U_INT";
        case BYTECODE_U_STRUCT: return "BYTECODE_U_STRUCT";
        case BYTECODE_U_STRUCT_ADDR: return "BYTECODE_U_STRUCT_ADDR";
        case BYTECODE_UP: return "BYTECODE_UP";
        case BYTECODE_BIND: return "BYTECODE_BIND";
        case BYTECODE_SON: return "BYTECODE_SON";
        case BYTECODE_MARK: return "BYTECODE_MARK";
        case BYTECODE_LAST_MARK: return "BYTECODE_LAST_MARK";
        case BYTECODE_CALL: return "BYTECODE_CALL";
        case BYTECODE_CALL_ADDR: return "BYTECODE_CALL_ADDR";
        case BYTECODE_LAST_CALL: return "BYTECODE_LAST_CALL";
        case BYTECODE_LAST_CALL_ADDR: return "BYTECODE_LAST_CALL_ADDR";
        case BYTECODE_PUSH_ENV: return "BYTECODE_PUSH_ENV";
        case BYTECODE_POP_ENV: return "BYTECODE_POP_ENV";
        case BYTECODE_SET_BTP: return "BYTECODE_SET_BTP";
        case BYTECODE_DEL_BTP: return "BYTECODE_DEL_BTP";
        case BYTECODE_TRY: return "BYTECODE_TRY";
        case BYTECODE_PRUNE: return "BYTECODE_PRUNE";
        case BYTECODE_SET_CUT: return "BYTECODE_SET_CUT";
        case BYTECODE_FAIL: return "BYTECODE_FAIL";
        case BYTECODE_JUMP: return "BYTECODE_JUMP";
        case BYTECODE_INIT: return "BYTECODE_INIT";
        case BYTECODE_HALT: return "BYTECODE_HALT";
        case BYTECODE_NO: return "BYTECODE_NO";
        case BYTECODE_LABEL: return "BYTECODE_LABEL";
        case BYTECODE_END: return "BYTECODE_END";
    }
    return "BYTECODE_UNKNOWN";
}

bytecode_node * bytecode_node_new(bytecode * value)
{
    bytecode_node * node = (bytecode_node *)malloc(sizeof(bytecode_node));

    node->value = *value;
    node->next = NULL;

    return node;
}

void bytecode_node_delete(bytecode_node * value)
{
    free(value);
}

bytecode_list * bytecode_list_new()
{
    bytecode_list * list = (bytecode_list *)malloc(sizeof(bytecode_list));

    list->head = NULL;
    list->tail = &list->head;
    list->size = 0;

    return list;
}

void bytecode_list_delete(bytecode_list * list)
{
    bytecode_node * node = list->head;
    while (node != NULL)
    {
        bytecode_node * next = node->next;
        bytecode_node_delete(node);
        node = next;
    }
    free(list);
}

void bytecode_list_set_addr(bytecode_list * list)
{
    bytecode_node * node = list->head;
    while (node != NULL)
    {
        bytecode * value = &node->value;
        
        if (value->type == BYTECODE_PUT_STRUCT)
        {
            value->type = BYTECODE_PUT_STRUCT_ADDR;
            unsigned int addr = value->put_struct.predicate_ref->addr;
            value->put_struct.addr = addr;
        }
        else if (value->type == BYTECODE_U_STRUCT)
        {
            value->type = BYTECODE_U_STRUCT_ADDR;
            unsigned int addr = value->u_struct.predicate_ref->addr;
            value->u_struct.addr = addr;
        }
        else if (value->type == BYTECODE_CALL)
        {
            value->type = BYTECODE_CALL_ADDR;
            unsigned int addr = value->call.predicate_ref->addr;
            value->call.addr = addr;
        }
        else if (value->type == BYTECODE_LAST_CALL)
        {
            value->type = BYTECODE_LAST_CALL_ADDR;
            unsigned int addr = value->last_call.predicate_ref->addr;
            value->last_call.addr = addr;
        }

        node = node->next;
    }
}

bytecode * bytecode_list_add_end(bytecode_list * list, bytecode * value)
{
    bytecode_node * node = bytecode_node_new(value);

    *(list->tail) = node;
    list->tail = &node->next;
    list->size++;

    return &node->value;
}

void bytecode_list_print(bytecode_list * list)
{
    bytecode_node * node = list->head;
    while (node != NULL)
    {
        bytecode_print(&node->value);
        node = node->next;
    }
}

void bytecode_to_array(bytecode_list * code, bytecode ** code_arr,
                       unsigned int * code_size)
{
    unsigned int addr = 0;
    bytecode_node * node = NULL;

    *code_size = code->size;
    *code_arr = (bytecode *)malloc(code->size * sizeof(bytecode));

    node = code->head;
    while (node != NULL)
    {
        if (node->value.addr != addr)
        {
            fprintf(stderr, "incorrectly generated code\n");
            assert(0);
        }
        if (node->value.type == BYTECODE_CALL)
        {
            fprintf(stderr, "cannot generate bytecode array with function "
                            "pointers, use bytecode_func_addr\n");
            assert(0);
        }

        (*code_arr)[addr++] = node->value;

        node = node->next;
    }
}

void bytecode_array_delete(bytecode * code_arr) { free(code_arr); }

void bytecode_array_print(bytecode * code_arr, unsigned int size)
{
    unsigned int i;

    printf("---- bytecode array beg ---\n");

    for (i = 0; i < size; i++)
    {
        bytecode_print_arr[code_arr[i].type].print(code_arr + i);
    }

    printf("---- bytecode array end ---\n");
}
