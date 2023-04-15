/**
 * Copyright (C) 2023 Slawomir Maludzinski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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
    VM_ERROR = 2
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
void vm_execute_reset(vm * machine, heap_ptr ref_x, heap_ptr ref_y);
void vm_execute_backtrack(vm *machine);
char vm_execute_unify(vm * machine, heap_ptr ref_u, heap_ptr ref_v);
char vm_execute_check_low(vm * machine, heap_ptr ref_u, heap_ptr ref_v);

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
void vm_execute_put_struct(vm * machine, bytecode * code);
void vm_execute_put_struct_addr(vm * machine, bytecode * code);
void vm_execute_u_atom(vm * machine, bytecode * code);
void vm_execute_u_struct(vm * machine, bytecode * code);
void vm_execute_u_struct_addr(vm * machine, bytecode * code);
void vm_execute_up(vm * machine, bytecode * code);
void vm_execute_bind(vm * machine, bytecode * code);
void vm_execute_son(vm * machine, bytecode * code);
void vm_execute_mark(vm * machine, bytecode * code);
void vm_execute_call(vm * machine, bytecode * code);
void vm_execute_call_addr(vm * machine, bytecode * code);
void vm_execute_push_env(vm * machine, bytecode * code);
void vm_execute_pop_env(vm * machine, bytecode * code);
void vm_execute_set_btp(vm * machine, bytecode * code);
void vm_execute_del_btp(vm * machine, bytecode * code);
void vm_execute_try(vm * machine, bytecode * code);
void vm_execute_init(vm * machine, bytecode * code);
void vm_execute_halt(vm * machine, bytecode * code);
void vm_execute_no(vm * machine, bytecode * code);
void vm_execute_jump(vm * machine, bytecode * code);
void vm_execute_label(vm * machine, bytecode * code);

#endif /* __VM_H__ */
