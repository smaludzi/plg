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
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "vm_types.h"

typedef enum object_type
{
    OBJECT_UNKNOWN = 0,
    OBJECT_ATOM = 1,
    OBJECT_REF = 4,
    OBJECT_STRUCT = 5,
} object_type;

typedef struct obj_atom {
    atom_idx_t idx;
} obj_atom;

typedef struct obj_ref {
    heap_ptr ref;
} obj_ref;

typedef struct obj_struct {
    heap_size_t size;
    pc_ptr addr;
    heap_ptr * refs;
} obj_struct;

typedef struct object
{
    object_type type;
    union {
        obj_atom atom_value;
        obj_ref ref_value;
        obj_struct struct_value;
    };
} object;

object * object_new_atom(atom_idx_t idx);
object * object_new_anon();
object * object_new_var();
object * object_new_ref(heap_ptr ptr_value);
object * object_new_struct(heap_size_t size, pc_ptr addr);

void object_delete(object * value);

void object_print(object * value);
void object_print_str(object * value, char ** strtab_array, unsigned int strtab_size);
const char * object_type_str(object_type type);

#endif /* __OBJECT_H__ */
