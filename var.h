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
#ifndef __VAR_H__
#define __VAR_H__

typedef enum var_type {
    VAR_TYPE_UNKNOWN = 0,
    VAR_TYPE_BOUND = 1,
    VAR_TYPE_UNBOUND = 2
} var_type;

typedef struct var
{
    var_type type;
    char * name;
    struct var * bound_to;
    unsigned int index;
    unsigned int line_no;
} var;

typedef struct var_node
{
    var * value;
    struct var_node * next;
} var_node;

typedef struct var_list
{
    var_node * head;
    var_node ** tail;
    unsigned int size;
} var_list;

var * var_new(char * name);
void var_delete(var * value);

void var_print(var * value);

var_node * var_node_new(var * value);
void var_node_delete(var_node * value);
void var_node_delete_null(var_node * value);

var_list * var_list_new();
void var_list_delete(var_list * list);
void var_list_delete_null(var_list * list);
void var_list_add_end(var_list * list, var * value);
unsigned int var_list_size(var_list * list);
void var_list_print(var_list * list);

char * var_type_to_str(var_type type);

#endif /* __VAR_H__ */

