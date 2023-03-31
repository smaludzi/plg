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

bytecode * bytecode_new()
{
    bytecode * value = (bytecode *)malloc(sizeof(bytecode));

    return value;
}

void bytecode_delete(bytecode * value)
{
    free(value);
}

void bytecode_print(bytecode * value)
{
    printf("bytecode type %s\n", bytecode_type_str(value->type));
}

const char * bytecode_type_str(bytecode_type type)
{
    switch (type)
    {
        case BYTECODE_UNKNOWN: return "BYTECODE_UNKNOWN";
        case BYTECODE_POP: return "BYTECODE_POP";
        case BYTECODE_PUT_REF: return "BYTECODE_PUT_REF";
        case BYTECODE_PUT_VAR: return "BYTECODE_PUT_VAR";
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

void bytecode_list_add_end(bytecode_list * list, bytecode * value)
{
    bytecode_node * node = bytecode_node_new(value);

    *(list->tail) = node;
    list->tail = &node->next;
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
