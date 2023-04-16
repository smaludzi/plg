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
#ifndef __GC_H__
#define __GC_H__

#include "object.h"

typedef struct gc_heap
{
    char mark;
    union {
        object * object_value;
        heap_ptr back_ptr; // used to correct references
    };
} gc_heap;

typedef enum gc_stack_type {
    STACK_TYPE_UNKNOWN = 0,
    STACK_TYPE_HEAP_PTR = 1,
    STACK_TYPE_STACK_PTR = 2,
    STACK_TYPE_PC_OFFSET = 3
} gc_stack_type;

typedef struct gc_stack
{
    gc_stack_type type;
    union {
        heap_ptr addr;
        stack_ptr saddr;
        pc_offset offset;
    };
} gc_stack;

typedef struct gc
{
    gc_heap * heap[2];
    heap_size_t free[2];
    unsigned int heap_idx;
    heap_size_t size;
} gc;

gc * gc_new(heap_size_t size);
void gc_delete(gc * collector);

void gc_mark(gc * collector, heap_size_t addr);
void gc_run(gc * collector, gc_stack * omfalos, stack_ptr stack_size);

heap_ptr gc_alloc_any(gc * collector, object * value);
heap_ptr gc_alloc_atom(gc * collector, atom_idx_t idx);
heap_ptr gc_alloc_anon(gc * collector);
heap_ptr gc_alloc_var(gc * collector);
heap_ptr gc_alloc_ref(gc * collector, heap_ptr ptr_value);
heap_ptr gc_alloc_struct(gc * collector, heap_size_t size, pc_ptr addr);

object_type gc_get_object_type(gc * collector, heap_ptr addr);

atom_idx_t gc_get_atom_idx(gc * collector, heap_ptr addr);
heap_ptr gc_get_anon_ref(gc * collector, heap_ptr addr);
heap_ptr gc_get_var_ref(gc * collector, heap_ptr addr);
heap_ptr gc_get_ref_ref(gc * collector, heap_ptr addr);
heap_size_t gc_get_struct_size(gc * collector, heap_ptr addr);
pc_ptr gc_get_struct_addr(gc * collector, heap_ptr addr);
heap_ptr gc_get_struct_ref(gc * collector, heap_ptr addr, heap_size_t idx);

atom_idx_t gc_set_atom_idx(gc * collector, heap_ptr addr, atom_idx_t idx);
heap_ptr gc_set_anon_ref(gc * collector, heap_ptr addr, heap_ptr ref);
heap_ptr gc_set_var_ref(gc * collector, heap_ptr addr, heap_ptr ref);
heap_ptr gc_set_ref_ref(gc * collector, heap_ptr addr, heap_ptr ref);
heap_ptr gc_set_struct_ref(gc * collector, heap_ptr addr, heap_size_t idx, heap_ptr ref);

void gc_print_ref(gc * collector, heap_ptr addr);
void gc_print_ref_str(gc * collector, heap_ptr addr, char ** strtab_array, unsigned int strtab_size);

gc_stack * gc_stack_new(stack_size_t size);
void gc_stack_delete(gc_stack * stack);

#endif /* __GC_H__ */
