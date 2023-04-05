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

typedef struct gc_mem
{
    char mark;
    union {
        object * object_value;
        mem_ptr back_ptr; // used to correct references
    };
} gc_mem;

typedef struct gc_stack
{
    mem_ptr addr;
} gc_stack;

typedef struct gc
{
    mem_ptr free[2];
    gc_mem * mem[2];
    unsigned int mem_idx;
    unsigned int mem_size;
} gc;

gc * gc_new(unsigned int mem_size);
void gc_delete(gc * collector);

void gc_mark(gc * collector, mem_ptr addr);
void gc_run(gc * collector, gc_stack * omfalos, stack_ptr stack_size);

mem_ptr gc_alloc_any(gc * collector, object * value);
mem_ptr gc_alloc_atom(gc * collector, unsigned int idx);
mem_ptr gc_alloc_anon(gc * collector);
mem_ptr gc_alloc_var(gc * collector);
mem_ptr gc_alloc_ref(gc * collector, mem_ptr ptr_value);
mem_ptr gc_alloc_struct(gc * collector, unsigned int size);

unsigned int gc_get_atom_idx(gc * collector, mem_ptr addr);
mem_ptr gc_get_anon_ref(gc * collector, mem_ptr addr);
mem_ptr gc_get_var_ref(gc * collector, mem_ptr addr);
mem_ptr gc_get_ref_ref(gc * collector, mem_ptr addr);
unsigned int gc_get_struct_size(gc * collector, mem_ptr addr);
mem_ptr gc_get_struct_ref(gc * collector, mem_ptr addr, unsigned int idx);

unsigned int gc_set_atom_idx(gc * collector, mem_ptr addr, unsigned int idx);
mem_ptr gc_set_anon_ref(gc * collector, mem_ptr addr, mem_ptr ref);
mem_ptr gc_set_var_ref(gc * collector, mem_ptr addr, mem_ptr ref);
mem_ptr gc_set_ref_ref(gc * collector, mem_ptr addr, mem_ptr ref);
mem_ptr gc_set_struct_ref(gc * collector, mem_ptr addr, unsigned int idx, mem_ptr ref);

gc_stack * gc_stack_new(int stack_size);
void gc_stack_delete(gc_stack * stack);

#endif /* __GC_H__ */
