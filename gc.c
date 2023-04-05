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
#include "gc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

gc * gc_new(unsigned int mem_size)
{
    unsigned int i = 0;
    gc * collector = (gc *)malloc(sizeof(gc));

    collector->free[0] = 1;
    collector->free[1] = 1;
    collector->mem[0] = (gc_mem *)malloc(sizeof(gc_mem) * mem_size);
    collector->mem[1] = (gc_mem *)malloc(sizeof(gc_mem) * mem_size);
    collector->mem_idx = 0;
    collector->mem_size = mem_size;

    for (i = 0; i < mem_size; i++)
    {
        collector->mem[0][i].mark = 0;
        collector->mem[0][i].object_value = NULL;
        collector->mem[1][i].mark = 0;
        collector->mem[1][i].object_value = NULL;
    }

    return collector;
}

void gc_delete(gc * collector)
{
    mem_ptr mi;
    for (mi = 0; mi < collector->mem_size; mi++)
    {
        void * object_value;
        if ((object_value = collector->mem[collector->mem_idx][mi].object_value) != NULL)
        {
            object_delete(object_value);
        }
    }
    if (collector->mem[0])
    {
        free(collector->mem[0]);
    }
    if (collector->mem[1])
    {
        free(collector->mem[1]);
    }
    free(collector);
}

void gc_mark(gc * collector, mem_ptr addr)
{
    if (addr == 0)
    {
        return;
    }
    if (collector->mem[collector->mem_idx][addr].mark == 1)
    {
        return;
    }
    if (collector->mem[collector->mem_idx][addr].object_value == NULL)
    {
        return;
    }
    switch (collector->mem[collector->mem_idx][addr].object_value->type)
    {
        case OBJECT_UNKNOWN:
            assert(0);
        break;
        case OBJECT_ATOM:
        case OBJECT_ANON:
        case OBJECT_VAR:
            collector->mem[collector->mem_idx][addr].mark = 1;
        break;
        case OBJECT_REF:
            collector->mem[collector->mem_idx][addr].mark = 1;
            gc_mark(collector, collector->mem[collector->mem_idx][addr].object_value->ref_value.ref);
        break;
        case OBJECT_STRUCT:
        {
            unsigned int i;
            collector->mem[collector->mem_idx][addr].mark = 1;
            object * value = collector->mem[collector->mem_idx][addr].object_value;
            for (i = 0; i < value->struct_value.size; i++)
            {
                gc_mark(collector, value->struct_value.refs[i]);
            }
        }
        break;
    }
}

void gc_run(gc * collector, gc_stack * omfalos, stack_ptr stack_size)
{
    // mark reachable objects
    stack_ptr si;
    for (si = 0; si < stack_size; si++)
    {
        gc_mark(collector, omfalos[si].addr);
    }
    // move reachable objects
    mem_ptr mi;
    unsigned int curr_mem = collector->mem_idx;
    unsigned int next_mem = 1 - curr_mem; 
    for (mi = 0; mi < collector->free[curr_mem]; mi++)
    {
        if (collector->mem[curr_mem][mi].mark == 1)
        {   // move object from one side to other
            collector->mem[next_mem][collector->free[next_mem]].object_value = collector->mem[curr_mem][mi].object_value;
            collector->mem[curr_mem][mi].back_ptr = collector->free[next_mem];
            collector->mem[curr_mem][mi].mark = 0;
            collector->free[next_mem]++;
        }
        else
        {   // reclaim memory
            if (collector->mem[curr_mem][mi].object_value != NULL)
            {
                object_delete(collector->mem[curr_mem][mi].object_value);
                collector->mem[curr_mem][mi].object_value = NULL;
            }
        }
    }
    // change addresses in objects
    for (mi = 0; mi < collector->free[next_mem]; mi++)
    {
        if (collector->mem[next_mem][mi].object_value != NULL)
        {
            switch (collector->mem[next_mem][mi].object_value->type)
            {
                case OBJECT_UNKNOWN:
                    assert(0);
                break;
                case OBJECT_ATOM:
                break;
                case OBJECT_ANON:
                {
                    mem_ptr * ref_ptr = &(collector->mem[next_mem][mi].object_value->anon_value.ref);
                    *ref_ptr = collector->mem[curr_mem][*ref_ptr].back_ptr;
                }
                break;
                case OBJECT_VAR:
                {
                    mem_ptr * ref_ptr = &(collector->mem[next_mem][mi].object_value->var_value.ref);
                    *ref_ptr = collector->mem[curr_mem][*ref_ptr].back_ptr;
                }
                break;
                case OBJECT_REF:
                {
                    mem_ptr * ref_ptr = &(collector->mem[next_mem][mi].object_value->ref_value.ref);
                    *ref_ptr = collector->mem[curr_mem][*ref_ptr].back_ptr;
                }
                break;
                case OBJECT_STRUCT:
                {
                    object * object_value = collector->mem[next_mem][mi].object_value;
                    for (unsigned int idx = 0; idx < object_value->struct_value.size; idx++)
                    {
                        mem_ptr * ref_ptr = &(object_value->struct_value.refs[idx]);
                        *ref_ptr = collector->mem[curr_mem][*ref_ptr].back_ptr;
                    }
                }
                break;
            }
        }
    }
    // change addresses in stack
    for (si = 0; si < stack_size; si++)
    {
        mem_ptr * ref_ptr = &(omfalos[si].addr);
        *ref_ptr = collector->mem[curr_mem][*ref_ptr].back_ptr;
    }
    // change memory side
    collector->free[curr_mem] = 1;
    collector->mem_idx = next_mem;
}

mem_ptr gc_alloc_any(gc * collector, object * value)
{
    mem_ptr loc = collector->free[collector->mem_idx];

    if (loc == 0)
    {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    collector->mem[collector->mem_idx][loc].mark = 0;
    collector->mem[collector->mem_idx][loc].object_value = value;
    collector->free[collector->mem_idx]++;

    return loc;
}

mem_ptr gc_alloc_atom(gc * collector, unsigned int idx)
{
    return gc_alloc_any(collector, object_new_atom(idx));
}

mem_ptr gc_alloc_anon(gc * collector)
{
    return gc_alloc_any(collector, object_new_anon());
}

mem_ptr gc_alloc_var(gc * collector)
{
    return gc_alloc_any(collector, object_new_var());
}

mem_ptr gc_alloc_ref(gc * collector, mem_ptr ptr_value)
{
    return gc_alloc_any(collector, object_new_ref(ptr_value));
}

mem_ptr gc_alloc_struct(gc * collector, unsigned int size)
{
    return gc_alloc_any(collector, object_new_struct(size));
}

unsigned int gc_get_atom_idx(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_ATOM);

    return collector->mem[collector->mem_idx][addr].object_value->atom_value.idx;
}

mem_ptr gc_get_anon_ref(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_ANON);

    return collector->mem[collector->mem_idx][addr].object_value->anon_value.ref;
}

mem_ptr gc_get_var_ref(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_VAR);

    return collector->mem[collector->mem_idx][addr].object_value->var_value.ref;
}

mem_ptr gc_get_ref_ref(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_REF);

    return collector->mem[collector->mem_idx][addr].object_value->ref_value.ref;
}

unsigned int gc_get_struct_size(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_STRUCT);

    return collector->mem[collector->mem_idx][addr].object_value->struct_value.size;
}

mem_ptr gc_get_struct_ref(gc * collector, mem_ptr addr, unsigned int idx)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_STRUCT);
    assert(collector->mem[collector->mem_idx][addr].object_value->struct_value.size >= addr);

    return collector->mem[collector->mem_idx][addr].object_value->struct_value.refs[idx];
}

unsigned int gc_set_atom_idx(gc * collector, mem_ptr addr, unsigned int idx)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_ATOM);

    return collector->mem[collector->mem_idx][addr].object_value->atom_value.idx = idx;
}

mem_ptr gc_set_anon_ref(gc * collector, mem_ptr addr, mem_ptr ref)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_ANON);

    return collector->mem[collector->mem_idx][addr].object_value->anon_value.ref = ref;
}

mem_ptr gc_set_var_ref(gc * collector, mem_ptr addr, mem_ptr ref)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_VAR);

    return collector->mem[collector->mem_idx][addr].object_value->var_value.ref = ref;
}

mem_ptr gc_set_ref_ref(gc * collector, mem_ptr addr, mem_ptr ref)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_REF);

    return collector->mem[collector->mem_idx][addr].object_value->ref_value.ref = ref;
}

mem_ptr gc_set_struct_ref(gc * collector, mem_ptr addr, unsigned int idx, mem_ptr ref)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[collector->mem_idx][addr].object_value->type == OBJECT_STRUCT);
    assert(collector->mem[collector->mem_idx][addr].object_value->struct_value.size >= addr);

    return collector->mem[collector->mem_idx][addr].object_value->struct_value.refs[idx] = ref;
}

gc_stack * gc_stack_new(int stack_size)
{
    gc_stack * stack = (gc_stack *)malloc(stack_size * sizeof(gc_stack));

    return stack;
}

void gc_stack_delete(gc_stack * stack) { free(stack); }


