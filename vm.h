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
#ifndef __VM_H__
#define __VM_H__

#include "bytecode.h"
#include "gencode.h"
#include "gc.h"

typedef enum vm_state
{
    VM_STOP = 0,
    VM_RUNNING = 1,
    VM_ERROR = 2,
    VM_ERROR_OUT_OF_MEMORY = 3
} vm_state;

typedef struct vm {
    pc_ptr pc; /* program counter */
    heap_ptr hp; /* heap pointer */
    stack_ptr sp; /* stack pointer */
    stack_ptr fp; /* frame pointer */
    stack_ptr bp; /* backtrack pointer */
    stack_ptr tp; /* trail pointer */
    heap_size_t heap_size; /* heap size */
    stack_size_t stack_size; /* stack size */
    stack_size_t trail_size; /* trail size */

    gc * collector;
    gc_stack * stack;
    gc_stack * trail;

    vm_state state;
    gencode_binary * binary_value_ref;
} vm;

typedef struct vm_execute_str
{
    bytecode_type type;
    void (*execute)(vm * machine, bytecode * code);
} vm_execute_str;

vm * vm_new(heap_size_t heap_size, stack_size_t stack_size, stack_size_t trail_size);
void vm_delete(vm * machine);

heap_ptr vm_execute_deref(vm * machine, heap_ptr ref);
void vm_execute_trail(vm * machine, heap_ptr ref);
void vm_execute_reset(vm * machine, stack_ptr ref_x, stack_ptr ref_y);
void vm_execute_backtrack(vm *machine);
char vm_execute_unify(vm * machine, heap_ptr ref_u, heap_ptr ref_v);
char vm_execute_check_low(vm * machine, heap_ptr ref_u, heap_ptr ref_v);
char vm_execute_check_size(vm * machine, stack_size_t new_stack_size, stack_size_t new_trail_size);

int vm_execute(vm * machine, gencode_binary * binary_value);
void vm_execute_test();
void vm_execute_print(vm * machine);

void vm_execute_unknown(vm * machine, bytecode * code);
void vm_execute_pop(vm * machine, bytecode * code);
void vm_execute_put_ref(vm * machine, bytecode * code);
void vm_execute_put_var(vm * machine, bytecode * code);
void vm_execute_u_ref(vm * machine, bytecode * code);
void vm_execute_u_var(vm * machine, bytecode * code);
void vm_execute_check(vm * machine, bytecode * code);
void vm_execute_put_anon(vm * machine, bytecode * code);
void vm_execute_put_atom(vm * machine, bytecode * code);
void vm_execute_put_int(vm * machine, bytecode * code);
void vm_execute_put_struct(vm * machine, bytecode * code);
void vm_execute_put_struct_addr(vm * machine, bytecode * code);
void vm_execute_u_atom(vm * machine, bytecode * code);
void vm_execute_u_int(vm * machine, bytecode * code);
void vm_execute_u_struct(vm * machine, bytecode * code);
void vm_execute_u_struct_addr(vm * machine, bytecode * code);
void vm_execute_up(vm * machine, bytecode * code);
void vm_execute_bind(vm * machine, bytecode * code);
void vm_execute_son(vm * machine, bytecode * code);
void vm_execute_mark(vm * machine, bytecode * code);
void vm_execute_last_mark(vm * machine, bytecode * code);
void vm_execute_call(vm * machine, bytecode * code);
void vm_execute_call_addr(vm * machine, bytecode * code);
void vm_execute_last_call(vm * machine, bytecode * code);
void vm_execute_last_call_addr(vm * machine, bytecode * code);
void vm_execute_push_env(vm * machine, bytecode * code);
void vm_execute_pop_env(vm * machine, bytecode * code);
void vm_execute_set_btp(vm * machine, bytecode * code);
void vm_execute_del_btp(vm * machine, bytecode * code);
void vm_execute_try(vm * machine, bytecode * code);
void vm_execute_prune(vm * machine, bytecode * code);
void vm_execute_set_cut(vm * machine, bytecode * code);
void vm_execute_fail(vm * machine, bytecode * code);
void vm_execute_init(vm * machine, bytecode * code);
void vm_execute_halt(vm * machine, bytecode * code);
void vm_execute_no(vm * machine, bytecode * code);
void vm_execute_jump(vm * machine, bytecode * code);
void vm_execute_label(vm * machine, bytecode * code);

const char * vm_state_to_str(vm_state state);

#endif /* __VM_H__ */
