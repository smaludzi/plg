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
#include "vm.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

vm_execute_str vm_execute_op[] = {
    { BYTECODE_UNKNOWN, vm_execute_unknown },
    { BYTECODE_POP, vm_execute_pop },
    { BYTECODE_PUT_REF, vm_execute_put_ref },
    { BYTECODE_PUT_VAR, vm_execute_put_var },
    { BYTECODE_U_REF, vm_execute_u_ref },
    { BYTECODE_U_VAR, vm_execute_u_var },
    { BYTECODE_CHECK, vm_execute_check },
    { BYTECODE_PUT_ANON, vm_execute_put_anon },
    { BYTECODE_PUT_ATOM, vm_execute_put_atom },
    { BYTECODE_PUT_STRUCT, vm_execute_put_struct },
    { BYTECODE_PUT_STRUCT_ADDR, vm_execute_put_struct_addr },
    { BYTECODE_U_ATOM, vm_execute_u_atom },
    { BYTECODE_U_STRUCT, vm_execute_u_struct },
    { BYTECODE_U_STRUCT_ADDR, vm_execute_u_struct_addr },
    { BYTECODE_UP, vm_execute_up },
    { BYTECODE_BIND, vm_execute_bind },
    { BYTECODE_SON, vm_execute_son },
    { BYTECODE_MARK, vm_execute_mark },
    { BYTECODE_CALL, vm_execute_call },
    { BYTECODE_CALL_ADDR, vm_execute_call_addr },
    { BYTECODE_PUSH_ENV, vm_execute_push_env },
    { BYTECODE_POP_ENV, vm_execute_pop_env },
    { BYTECODE_SET_BTP, vm_execute_set_btp },
    { BYTECODE_DEL_BTP, vm_execute_del_btp },
    { BYTECODE_TRY, vm_execute_try },
    { BYTECODE_INIT, vm_execute_init },
    { BYTECODE_HALT, vm_execute_halt },
    { BYTECODE_NO, vm_execute_no },
    { BYTECODE_JUMP, vm_execute_jump },
    { BYTECODE_LABEL, vm_execute_label }
};

vm * vm_new(
    heap_size_t heap_size,
    stack_size_t stack_size,
    stack_size_t trail_size)
{
    vm * machine = (vm *)malloc(sizeof(vm));

    machine->pc = 0;
    machine->hp = 0;
    machine->sp = -1;
    machine->fp = -1;
    machine->bp = -1;
    machine->tp = -1;

    machine->heap_size = heap_size;
    machine->stack_size = stack_size;
    machine->trail_size = trail_size;
    machine->binary_value_ref = NULL;
    machine->state = VM_STOP;

    machine->collector = gc_new(heap_size);
    machine->stack = gc_stack_new(stack_size);
    machine->trail = gc_stack_new(trail_size);

    vm_execute_test();

    return machine;
}

void vm_delete(vm * machine)
{
    if (machine->collector != NULL)
    {
        gc_delete(machine->collector);
    }
    if (machine->stack != NULL)
    {
        gc_stack_delete(machine->stack);
    }
    if (machine->trail != NULL)
    {
        gc_stack_delete(machine->stack);
    }
    free(machine);
}

void vm_execute_test()
{
    unsigned int i = 0;
    for (i = 0; i < BYTECODE_END; i++)
    {
        assert(i == vm_execute_op[i].type);
    }
}

void vm_execute_unknown(vm * machine, bytecode * code)
{
    assert(0);
}

void vm_execute_pop(vm * machine, bytecode * code)
{
    machine->sp--;
}

void vm_execute_put_ref(vm * machine, bytecode * code)
{
    machine->sp++;
    machine->stack[machine->sp].addr = gc_alloc_ref(machine->collector, vm_execute_deref(machine, machine->stack[machine->fp + code->put_ref.index].addr));
}

void vm_execute_put_var(vm * machine, bytecode * code)
{
    machine->sp++;
    machine->stack[machine->fp + code->put_var.index].addr = machine->stack[machine->sp].addr = gc_alloc_var(machine->collector);
}

void vm_execute_u_ref(vm * machine, bytecode * code)
{
    vm_execute_unify(machine,
                     machine->stack[machine->sp].addr,
                     vm_execute_deref(machine, machine->stack[machine->fp + code->u_ref.index].addr));
    machine->sp--;
}

void vm_execute_u_var(vm * machine, bytecode * code)
{
    machine->stack[machine->fp + code->u_var.index].addr = machine->stack[machine->sp].addr;
    machine->sp--;
}

void vm_execute_check(vm * machine, bytecode * code)
{
    if (!vm_execute_check_low(machine,
                              machine->stack[machine->sp].addr,
                              vm_execute_deref(machine, machine->stack[machine->fp + code->check.index].addr)))
    {
        vm_execute_backtrack(machine);
    }
}

void vm_execute_put_anon(vm * machine, bytecode * code)
{
    machine->sp++;
    machine->stack[machine->sp].addr = gc_alloc_anon(machine->collector);
}

void vm_execute_put_atom(vm * machine, bytecode * code)
{
    machine->sp++;
    machine->stack[machine->sp].addr = gc_alloc_atom(machine->collector, code->put_atom.idx);
}

void vm_execute_put_struct(vm * machine, bytecode * code)
{
    bytecode_print(code);
    fprintf(stderr, " %u: cannot execute bytecode %s\n", code->addr, bytecode_type_str(code->type));
}

void vm_execute_put_struct_addr(vm * machine, bytecode * code)
{
    unsigned int i = 0;
    machine->sp = machine->sp - code->put_struct.n + 1;
    heap_ptr value = gc_alloc_struct(machine->collector, code->put_struct.n, code->put_struct.addr);
    for (i = 0; i < code->put_struct.n; i++)
    {
        gc_set_struct_ref(machine->collector, value, i, machine->stack[machine->sp + i].addr);
    }
    machine->stack[machine->sp].addr = value;
}

void vm_execute_u_atom(vm * machine, bytecode * code)
{
    heap_ptr h_ref = machine->stack[machine->sp].addr;
    machine->sp--;
    switch (gc_get_object_type(machine->collector, h_ref))
    {
        case OBJECT_UNKNOWN:
            assert(0);
        break;
        case OBJECT_ATOM:
        break;
        case OBJECT_REF:
        {
            /* TODO: check */
            gc_set_var_ref(machine->collector, machine->hp, gc_alloc_atom(machine->collector, code->u_atom.idx));
            gc_set_var_ref(machine->collector, h_ref, gc_alloc_ref(machine->collector, machine->hp));
            vm_execute_trail(machine, h_ref);
            machine->hp++;
        }
        break;
        case OBJECT_STRUCT:
            vm_execute_backtrack(machine);
        break;
    }
}

void vm_execute_u_struct(vm * machine, bytecode * code)
{
    bytecode_print(code);
    fprintf(stderr, " %u: cannot execute bytecode %s\n", code->addr, bytecode_type_str(code->type));
}

void vm_execute_u_struct_addr(vm * machine, bytecode * code)
{
    /* TODO: check */
    switch(gc_get_object_type(machine->collector, gc_get_ref_ref(machine->collector, machine->stack[machine->sp].addr)))
    {
        case OBJECT_UNKNOWN:
            assert(0);
        break;
        case OBJECT_ATOM:
            vm_execute_backtrack(machine);
        break;
        case OBJECT_REF:
        {
            machine->pc = code->u_struct.addr;
        }
        break;
        case OBJECT_STRUCT:
        break;
    }
}

void vm_execute_up(vm * machine, bytecode * code)
{
    machine->sp--;
    machine->pc = code->up.offset;
}

void vm_execute_bind(vm * machine, bytecode * code)
{
    /* TODO: check this */
    gc_set_var_ref(machine->collector, machine->stack[machine->sp - 1].addr, gc_alloc_ref(machine->collector, machine->stack[machine->sp].addr));
    vm_execute_trail(machine, machine->stack[machine->sp - 1].addr);
    machine->sp = machine->sp - 2;
}

void vm_execute_son(vm * machine, bytecode * code)
{
    machine->stack[machine->sp + 1].addr = vm_execute_deref(machine, gc_get_struct_ref(machine->collector, machine->stack[machine->sp].addr, code->son.number));
    machine->sp++;
}

void vm_execute_mark(vm * machine, bytecode * code)
{
    gc_stack b_entry = { 0 };
    gc_stack fp_entry = { 0 };

    b_entry.type = STACK_TYPE_PC_OFFSET;
    b_entry.offset = code->mark.offset;

    fp_entry.type = STACK_TYPE_STACK_PTR;
    fp_entry.saddr= machine->fp;

    machine->stack[machine->sp + 6] = b_entry;
    machine->stack[machine->sp + 5] = fp_entry;

    machine->sp = machine->sp + 6;
}

void vm_execute_call(vm * machine, bytecode * code)
{
    bytecode_print(code);
    fprintf(stderr, " %u: cannot execute bytecode %s\n", code->addr, bytecode_type_str(code->type));
}

void vm_execute_call_addr(vm * machine, bytecode * code)
{
    machine->fp = machine->sp - code->call.n;
    machine->pc = code->call.addr;
}

void vm_execute_push_env(vm * machine, bytecode * code)
{

}

void vm_execute_pop_env(vm * machine, bytecode * code)
{

}

void vm_execute_set_btp(vm * machine, bytecode * code)
{

}

void vm_execute_del_btp(vm * machine, bytecode * code)
{

}

void vm_execute_try(vm * machine, bytecode * code)
{

}

void vm_execute_init(vm * machine, bytecode * code)
{

}

void vm_execute_halt(vm * machine, bytecode * code)
{

}

void vm_execute_no(vm * machine, bytecode * code)
{
    machine->state = VM_STOP;
}

void vm_execute_jump(vm * machine, bytecode * code)
{

}

void vm_execute_label(vm * machine, bytecode * code)
{

}

heap_ptr vm_execute_deref(vm * machine, heap_ptr ref)
{
    if (gc_get_object_type(machine->collector, ref) == OBJECT_REF &&
        gc_get_ref_ref(machine->collector, ref) != ref) {
            return vm_execute_deref(machine, gc_get_ref_ref(machine->collector, ref));
    } else {
        return ref;
    }
}

void vm_execute_trail(vm * machine, heap_ptr ref)
{
    assert(machine->stack[machine->bp - 2].type == STACK_TYPE_HEAP_PTR);

    if (ref < machine->stack[machine->bp - 2].addr) {
        machine->tp = machine->tp + 1;

        machine->trail[machine->tp].type = STACK_TYPE_HEAP_PTR;
        machine->trail[machine->tp].addr = ref;
    }
}

void vm_execute_reset(vm * machine, heap_ptr ref_x, heap_ptr ref_y)
{
    heap_ptr ref_u;
    for (ref_u = ref_y; ref_x < ref_u; ref_u--)
    {
        /* TODO: check this */
        gc_set_var_ref(machine->collector, machine->trail[ref_u].addr, gc_alloc_ref(machine->collector, machine->trail[ref_u].addr));
    }
}

void vm_execute_backtrack(vm * machine)
{
    machine->fp = machine->bp;
    machine->hp = machine->stack[machine->fp - 2].addr;
    vm_execute_reset(machine, machine->stack[machine->fp - 3].addr, machine->tp);
    machine->tp = machine->stack[machine->fp - 3].addr;
    machine->pc = machine->stack[machine->fp - 5].offset;
}

char vm_execute_unify(vm * machine, heap_ptr ref_u, heap_ptr ref_v)
{
    if (ref_u == ref_v)
    {
        return 1;
    }
    if (gc_get_object_type(machine->collector, ref_u) == OBJECT_REF)
    {
        if (gc_get_object_type(machine->collector, ref_v) == OBJECT_REF)
        {
            if (ref_u > ref_v)
            {
                gc_set_ref_ref(machine->collector, ref_u, ref_v);
                vm_execute_trail(machine, ref_u);
                return 1;
            }
            else
            {
                gc_set_ref_ref(machine->collector, ref_v, ref_u);
                vm_execute_trail(machine, ref_v);
                return 1;
            }
        }
        else if (vm_execute_check_low(machine, ref_u, ref_v))
        {
            gc_set_ref_ref(machine->collector, ref_u, ref_v);
            vm_execute_trail(machine, ref_u);
            return 1;
        }
        else
        {
            vm_execute_backtrack(machine);
            return 0;
        }
    }
    if (gc_get_object_type(machine->collector, ref_v) == OBJECT_REF)
    {
        if (vm_execute_check_low(machine, ref_v, ref_u))
        {
            gc_set_ref_ref(machine->collector, ref_v, ref_u);
            vm_execute_trail(machine, ref_v);
            return 1;
        }
        else
        {
            vm_execute_backtrack(machine);
            return 0;
        }
    }
    if (gc_get_object_type(machine->collector, ref_u) == OBJECT_ATOM &&
        gc_get_object_type(machine->collector, ref_v) == OBJECT_ATOM &&
        gc_get_atom_idx(machine->collector, ref_u) == gc_get_atom_idx(machine->collector, ref_u))
    {
        return 1;
    }
    if (gc_get_object_type(machine->collector, ref_u) == OBJECT_STRUCT &&
        gc_get_object_type(machine->collector, ref_v) == OBJECT_STRUCT &&
        gc_get_struct_addr(machine->collector, ref_u) == gc_get_struct_addr(machine->collector, ref_v))
    {
        unsigned int i = 0;
        unsigned int size = gc_get_struct_size(machine->collector, ref_u);
        for (i = 0; i < size; i++)
        {
            if (!vm_execute_unify(machine,
                                  vm_execute_deref(machine, gc_get_struct_ref(machine->collector, ref_u, i)),
                                  vm_execute_deref(machine, gc_get_struct_ref(machine->collector, ref_v, i))))
            {
                return 0;
            }
        }
    }
    vm_execute_backtrack(machine);
    return 0;
}

char vm_execute_check_low(vm * machine, heap_ptr ref_u, heap_ptr ref_v)
{
    if (ref_u == ref_v)
    {
        return 0;
    }
    if (gc_get_object_type(machine->collector, ref_v) == OBJECT_STRUCT)
    {
        unsigned int i = 0;
        unsigned int size = gc_get_struct_size(machine->collector, ref_v);
        for (i = 0; i < size; i++)
        {
            if (!vm_execute_check_low(machine, ref_u,
                                      vm_execute_deref(machine, gc_get_struct_ref(machine->collector, ref_v, i))))
            {
                return 0;
            }
        }
    }
    return 1;
}

int vm_execute(vm * machine, gencode_binary * binary_value)
{
    bytecode * bc = NULL;
    machine->binary_value_ref = binary_value;

    machine->state = VM_RUNNING;
    while (machine->state == VM_RUNNING)
    {
        bc = machine->binary_value_ref->code_array + machine->pc;
        machine->pc++;
        vm_execute_op[bc->type].execute(machine, bc);
    }

    if (machine->state == VM_ERROR)
    {
        // print machine error state
        return 1;
    }
    else if (machine->state == VM_STOP)
    {
        return 0;
    }

    return 1;
}
