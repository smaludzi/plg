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
    BYTECODE_PUT_VAR = 3
} bytecode_type;

typedef struct bytecode {
    bytecode_type type;
    union {
        struct {
            unsigned int index;
        } put_ref;
        struct {
            unsigned int index;
        } put_var;
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

bytecode * bytecode_new_pop();
void bytecode_delete(bytecode * value);

void bytecode_print(bytecode * value);
const char * bytecode_type_str(bytecode_type type);

bytecode_node * bytecode_node_new();
void bytecode_node_delete(bytecode_node * value);

bytecode_list * bytecode_list_new();
void bytecode_list_delete(bytecode_list * list);

void bytecode_list_add_end(bytecode_list * list, bytecode * value);
void bytecode_list_print(bytecode_list * list);

#endif /* __BYTECODE_H__ */
