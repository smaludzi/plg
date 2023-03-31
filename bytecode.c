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
    { BYTECODE_PUT_STRUCT, bytecode_print_put_struct },
    { BYTECODE_U_ATOM, bytecode_print_u_atom },
    { BYTECODE_U_STRUCT, bytecode_print_u_struct },
    { BYTECODE_UP, bytecode_print_up },
    { BYTECODE_BIND, bytecode_print_bind },
    { BYTECODE_SON, bytecode_print_son },
    { BYTECODE_JUMP, bytecode_print_jump },
    { BYTECODE_LABEL, bytecode_print_label }
};

bytecode * bytecode_new()
{
    bytecode * value = (bytecode *)malloc(sizeof(bytecode));

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
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_put_struct(bytecode * value)
{
    printf("%d: %s size %u\n", value->addr, bytecode_type_str(value->type), value->put_struct.size);
}

void bytecode_print_u_atom(bytecode * value)
{
    printf("%d: %s\n", value->addr, bytecode_type_str(value->type));
}

void bytecode_print_u_struct(bytecode * value)
{
    printf("%d: %s size %u offset %d\n", value->addr, bytecode_type_str(value->type), value->u_struct.size, value->u_struct.offset);
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

void bytecode_print_jump(bytecode * value)
{
    printf("%d: %s index %d\n", value->addr, bytecode_type_str(value->type), value->jump.offset);
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
        case BYTECODE_PUT_STRUCT: return "BYTECODE_PUT_STRUCT";
        case BYTECODE_U_ATOM: return "BYTECODE_U_ATOM";
        case BYTECODE_U_STRUCT: return "BYTECODE_U_STRUCT";
        case BYTECODE_UP: return "BYTECODE_UP";
        case BYTECODE_BIND: return "BYTECODE_BIND";
        case BYTECODE_SON: return "BYTECODE_SON";
        case BYTECODE_JUMP: return "BYTECODE_JUMP";
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

bytecode * bytecode_list_add_end(bytecode_list * list, bytecode * value)
{
    bytecode_node * node = bytecode_node_new(value);

    *(list->tail) = node;
    list->tail = &node->next;

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
