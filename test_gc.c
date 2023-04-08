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

void test_one()
{
    gc * collector = gc_new(64);
    gc_delete(collector);
}

void test_two()
{
    gc * collector = gc_new(64);

    heap_ptr atom1 = gc_alloc_atom(collector, 12);
    heap_ptr atom2 = gc_alloc_atom(collector, 10);
    gc_set_atom_idx(collector, atom2, 22);

    heap_ptr ref1 = gc_alloc_ref(collector, atom1);
    heap_ptr ref2 = gc_alloc_ref(collector, atom2);

    assert(gc_get_atom_idx(collector, atom1) == 12);
    assert(gc_get_atom_idx(collector, atom2) == 22);

    assert(gc_get_ref_ref(collector, ref1) == atom1);
    assert(gc_get_ref_ref(collector, ref2) == atom2);

    gc_delete(collector);
}

void test_three()
{
    gc * collector = gc_new(64);

    heap_ptr anon1 = gc_alloc_anon(collector);
    heap_ptr anon2 = gc_alloc_anon(collector);

    heap_ptr ref1 = gc_alloc_ref(collector, anon1);
    heap_ptr ref2 = gc_alloc_ref(collector, anon1);
    gc_set_ref_ref(collector, ref2, anon2);

    assert(gc_get_anon_ref(collector, anon1) == anon1);
    assert(gc_get_anon_ref(collector, anon2) == anon2);

    assert(gc_get_ref_ref(collector, ref1) == anon1);
    assert(gc_get_ref_ref(collector, ref2) == anon2);

    gc_delete(collector);
}

void test_four()
{
    gc * collector = gc_new(64);

    heap_ptr var1 = gc_alloc_var(collector);
    heap_ptr var2 = gc_alloc_var(collector);

    heap_ptr ref1 = gc_alloc_ref(collector, var1);
    heap_ptr ref2 = gc_alloc_ref(collector, var2);

    assert(gc_get_var_ref(collector, var1) == var1);
    assert(gc_get_var_ref(collector, var2) == var2);

    assert(gc_get_ref_ref(collector, ref1) == var1);
    assert(gc_get_ref_ref(collector, ref2) == var2);

    gc_delete(collector);
}

void test_five()
{
    gc * collector = gc_new(64);

    heap_ptr atom1 = gc_alloc_atom(collector, 12);
    heap_ptr atom2 = gc_alloc_atom(collector, 22);

    heap_ptr struct1 = gc_alloc_struct(collector, 2);
    gc_set_struct_ref(collector, struct1, 0, atom1);
    gc_set_struct_ref(collector, struct1, 1, atom2);

    heap_ptr ref1 = gc_alloc_ref(collector, struct1);

    assert(gc_get_atom_idx(collector, atom1) == 12);
    assert(gc_get_atom_idx(collector, atom2) == 22);
    assert(gc_get_struct_size(collector, struct1) == 2);
    assert(gc_get_struct_ref(collector, struct1, 0) == atom1);
    assert(gc_get_struct_ref(collector, struct1, 1) == atom2);
    assert(gc_get_ref_ref(collector, ref1) == struct1);

    gc_delete(collector);
}

void test_six()
{
    gc * collector = gc_new(64);
    gc_stack stack[2] = { 0 };

    heap_ptr atom1 = gc_alloc_atom(collector, 12);
    heap_ptr atom2 = gc_alloc_atom(collector, 22);

    heap_ptr struct1 = gc_alloc_struct(collector, 2);
    gc_set_struct_ref(collector, struct1, 0, atom1);
    gc_set_struct_ref(collector, struct1, 1, atom2);

    heap_ptr ref1 = gc_alloc_ref(collector, struct1);

    stack[0].addr = ref1;
    stack[1].addr = struct1;

    gc_run(collector, stack, 2);

    assert(gc_get_ref_ref(collector, stack[0].addr) == stack[1].addr);
    assert(gc_get_struct_size(collector, stack[1].addr) == 2);
    assert(gc_get_atom_idx(collector, gc_get_struct_ref(collector, stack[1].addr, 0)) == 12);
    assert(gc_get_atom_idx(collector, gc_get_struct_ref(collector, stack[1].addr, 1)) == 22);

    gc_delete(collector);
}

void test_seven()
{
    gc * collector = gc_new(64);
    gc_stack stack[2] = { 0 };

    gc_alloc_anon(collector);
    heap_ptr atom1 = gc_alloc_atom(collector, 12);
    heap_ptr atom2 = gc_alloc_atom(collector, 22);

    gc_alloc_anon(collector);
    heap_ptr struct1 = gc_alloc_struct(collector, 2);
    gc_set_struct_ref(collector, struct1, 0, atom1);
    gc_set_struct_ref(collector, struct1, 1, atom2);

    heap_ptr ref1 = gc_alloc_ref(collector, struct1);
    gc_alloc_anon(collector);

    stack[0].addr = ref1;
    stack[1].addr = struct1;

    assert(collector->free[collector->mem_idx] == 8);
    gc_run(collector, stack, 2);
    assert(collector->free[collector->mem_idx] == 5);

    assert(gc_get_ref_ref(collector, stack[0].addr) == stack[1].addr);
    assert(gc_get_struct_size(collector, stack[1].addr) == 2);
    assert(gc_get_atom_idx(collector, gc_get_struct_ref(collector, stack[1].addr, 0)) == 12);
    assert(gc_get_atom_idx(collector, gc_get_struct_ref(collector, stack[1].addr, 1)) == 22);

    gc_delete(collector);
}

void test_eight()
{
    gc * collector = gc_new(64);
    gc_stack stack[2] = { 0 };

    gc_alloc_anon(collector);
    heap_ptr atom1 = gc_alloc_atom(collector, 12);
    heap_ptr atom2 = gc_alloc_atom(collector, 22);

    gc_alloc_anon(collector);
    heap_ptr struct1 = gc_alloc_struct(collector, 2);
    gc_set_struct_ref(collector, struct1, 0, atom1);
    gc_set_struct_ref(collector, struct1, 1, atom2);

    gc_alloc_anon(collector);
    heap_ptr ref1 = gc_alloc_ref(collector, struct1);

    stack[0].addr = ref1;
    stack[1].addr = struct1;

    assert(collector->mem_idx == 0);
    assert(collector->free[collector->mem_idx] == 8);

    gc_run(collector, stack, 2);
    assert(collector->mem_idx == 1);
    assert(collector->free[collector->mem_idx] == 5);

    gc_run(collector, stack, 2);
    assert(collector->mem_idx == 0);
    assert(collector->free[collector->mem_idx] == 5);

    assert(gc_get_ref_ref(collector, stack[0].addr) == stack[1].addr);
    assert(gc_get_struct_size(collector, stack[1].addr) == 2);
    assert(gc_get_atom_idx(collector, gc_get_struct_ref(collector, stack[1].addr, 0)) == 12);
    assert(gc_get_atom_idx(collector, gc_get_struct_ref(collector, stack[1].addr, 1)) == 22);

    gc_delete(collector);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();
    test_four();
    test_five();
    test_six();
    test_seven();
    test_eight();

    return 0;
}
