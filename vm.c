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
    { BYTECODE_PUT_INT, vm_execute_put_int },
    { BYTECODE_PUT_STRUCT, vm_execute_put_struct },
    { BYTECODE_PUT_STRUCT_ADDR, vm_execute_put_struct_addr },
    { BYTECODE_U_ATOM, vm_execute_u_atom },
    { BYTECODE_U_INT, vm_execute_u_int },
    { BYTECODE_U_STRUCT, vm_execute_u_struct },
    { BYTECODE_U_STRUCT_ADDR, vm_execute_u_struct_addr },
    { BYTECODE_UP, vm_execute_up },
    { BYTECODE_BIND, vm_execute_bind },
    { BYTECODE_SON, vm_execute_son },
    { BYTECODE_MARK, vm_execute_mark },
    { BYTECODE_LAST_MARK, vm_execute_last_mark },
    { BYTECODE_CALL, vm_execute_call },
    { BYTECODE_CALL_ADDR, vm_execute_call_addr },
    { BYTECODE_LAST_CALL, vm_execute_last_call },
    { BYTECODE_LAST_CALL_ADDR, vm_execute_last_call_addr },
    { BYTECODE_PUSH_ENV, vm_execute_push_env },
    { BYTECODE_POP_ENV, vm_execute_pop_env },
    { BYTECODE_SET_BTP, vm_execute_set_btp },
    { BYTECODE_DEL_BTP, vm_execute_del_btp },
    { BYTECODE_TRY, vm_execute_try },
    { BYTECODE_PRUNE, vm_execute_prune },
    { BYTECODE_SET_CUT, vm_execute_set_cut },
    { BYTECODE_FAIL, vm_execute_fail },
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
        gc_stack_delete(machine->trail);
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

void vm_execute_print(vm * machine)
{
    printf("------------------------\n");
    printf("program counter   pc: %u\n", machine->pc);
    printf("heap pointer      hp: %u\n", gc_get_hp(machine->collector));
    printf("stack pointer     sp: %d\n", machine->sp);
    printf("frame pointer     fp: %d\n", machine->fp);
    printf("backtrack pointer bp: %d\n", machine->bp);
    printf("trail pointer     tp: %d\n", machine->tp);
    printf("heap_size           : %u\n", machine->heap_size);
    printf("stack size          : %d\n", machine->stack_size);
    printf("trail size          : %d\n", machine->trail_size);
    printf("state               : %s\n", vm_state_to_str(machine->state));
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
    if (!vm_execute_check_size(machine, machine->sp + 1, machine->tp))
    {
        return;
    }

    assert(machine->stack[machine->fp + code->put_ref.index].type == STACK_TYPE_HEAP_PTR);
    heap_ptr ref_d = vm_execute_deref(machine, machine->stack[machine->fp + code->put_ref.index].addr);

    gc_stack entry = { 0 };
    entry.type = STACK_TYPE_HEAP_PTR;
    entry.addr = ref_d;

    machine->sp++;
    machine->stack[machine->sp] = entry;
}

void vm_execute_put_var(vm * machine, bytecode * code)
{
    if (!vm_execute_check_size(machine, machine->sp + 1, machine->tp))
    {
        return;
    }

    gc_stack entry = { 0 };
    entry.type = STACK_TYPE_HEAP_PTR;
    entry.addr = gc_alloc_var(machine->collector);
    if (entry.addr == 0)
    {
        machine->state = VM_ERROR_OUT_OF_MEMORY;
        return;
    }

    machine->sp++;
    machine->stack[machine->fp + code->put_var.index] = entry;
    machine->stack[machine->sp] = entry;
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
    if (!vm_execute_check_size(machine, machine->sp + 1, machine->tp))
    {
        return;
    }

    gc_stack entry = { 0 };
    entry.type = STACK_TYPE_HEAP_PTR;
    entry.addr = gc_alloc_anon(machine->collector);
    if (entry.addr == 0)
    {
        machine->state = VM_ERROR_OUT_OF_MEMORY;
        return;
    }

    machine->sp++;
    machine->stack[machine->sp] = entry;
}

void vm_execute_put_atom(vm * machine, bytecode * code)
{
    if (!vm_execute_check_size(machine, machine->sp + 1, machine->tp))
    {
        return;
    }

    gc_stack entry = { 0 };
    entry.type = STACK_TYPE_HEAP_PTR;
    entry.addr = gc_alloc_atom(machine->collector, code->put_atom.idx);
    if (entry.addr == 0)
    {
        machine->state = VM_ERROR_OUT_OF_MEMORY;
        return;
    }

    machine->sp++;
    machine->stack[machine->sp] = entry;
}

void vm_execute_put_int(vm * machine, bytecode * code)
{
    if (!vm_execute_check_size(machine, machine->sp + 1, machine->tp))
    {
        return;
    }

    gc_stack entry = { 0 };
    entry.type = STACK_TYPE_HEAP_PTR;
    entry.addr = gc_alloc_int(machine->collector, code->put_int.value);
    if (entry.addr == 0)
    {
        machine->state = VM_ERROR_OUT_OF_MEMORY;
        return;
    }

    machine->sp++;
    machine->stack[machine->sp] = entry;
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

    gc_stack entry = { 0 };
    entry.type = STACK_TYPE_HEAP_PTR;
    entry.addr = gc_alloc_struct(machine->collector, code->put_struct.n, code->put_struct.addr);

    if (entry.addr == 0)
    {
        machine->state = VM_ERROR_OUT_OF_MEMORY;
        return;
    }

    for (i = 0; i < code->put_struct.n; i++)
    {
        gc_set_struct_ref(machine->collector, entry.addr, i, machine->stack[machine->sp + i].addr);
    }
    machine->stack[machine->sp] = entry;
}

void vm_execute_u_atom(vm * machine, bytecode * code)
{
    heap_ptr h_ref = machine->stack[machine->sp].addr;
    machine->sp--;
    switch (gc_get_object_type(machine->collector, h_ref))
    {
        case OBJECT_UNKNOWN:
            printf("unknow object %u\n", h_ref);
            assert(0);
        break;
        case OBJECT_ATOM:
            if (gc_get_atom_idx(machine->collector, h_ref) != code->u_atom.idx)
            {
                vm_execute_backtrack(machine);
            }
        break;
        case OBJECT_INT:
            vm_execute_backtrack(machine);
        break;
        case OBJECT_REF:
        {
            /* NOTE: second version, Fig. 4.17 p. 121 */
            heap_ptr a_value = gc_alloc_atom(machine->collector, code->u_atom.idx);
            if (a_value == 0)
            {
                machine->state = VM_ERROR_OUT_OF_MEMORY;
                return;
            }

            gc_set_ref_ref(machine->collector, h_ref, a_value);
            vm_execute_trail(machine, h_ref);
        }
        break;
        case OBJECT_STRUCT:
            vm_execute_backtrack(machine);
        break;
    }
}

void vm_execute_u_int(vm * machine, bytecode * code)
{
    heap_ptr h_ref = machine->stack[machine->sp].addr;
    machine->sp--;
    switch (gc_get_object_type(machine->collector, h_ref))
    {
        case OBJECT_UNKNOWN:
            printf("unknow object %u\n", h_ref);
            assert(0);
        break;
        case OBJECT_ATOM:
            vm_execute_backtrack(machine);
        break;
        case OBJECT_INT:
            if (gc_get_int_value(machine->collector, h_ref) != code->u_int.value)
            {
                vm_execute_backtrack(machine);
            }
        break;
        case OBJECT_REF:
        {
            heap_ptr a_value = gc_alloc_int(machine->collector, code->u_int.value);
            if (a_value == 0)
            {
                machine->state = VM_ERROR_OUT_OF_MEMORY;
                return;
            }

            gc_set_ref_ref(machine->collector, h_ref, a_value);
            vm_execute_trail(machine, h_ref);
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
    /* NOTE: third version, Fig. 4.21 p.124 */
    switch(gc_get_object_type(machine->collector, machine->stack[machine->sp].addr))
    {
        case OBJECT_UNKNOWN:
            printf("unknow object %u\n", machine->stack[machine->sp].addr);
            assert(0);
        break;
        case OBJECT_ATOM:
            vm_execute_backtrack(machine);
        break;
        case OBJECT_INT:
            vm_execute_backtrack(machine);
        break;
        case OBJECT_REF:
        {
            machine->pc = code->u_struct.offset;
        }
        break;
        case OBJECT_STRUCT:
        {
            if (!(gc_get_struct_addr(machine->collector, machine->stack[machine->sp].addr) == code->u_struct.addr &&
                  gc_get_struct_size(machine->collector, machine->stack[machine->sp].addr) == code->u_struct.n))
            {
                vm_execute_backtrack(machine);
            }
        }
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
    /* NOTE: second version, Fig 4.13 p. 117 */
    gc_set_ref_ref(machine->collector, machine->stack[machine->sp - 1].addr, machine->stack[machine->sp].addr);
    vm_execute_trail(machine, machine->stack[machine->sp - 1].addr);
    machine->sp = machine->sp - 2;
}

void vm_execute_son(vm * machine, bytecode * code)
{
    if (!vm_execute_check_size(machine, machine->sp + 1, machine->tp))
    {
        return;
    }
    
    machine->stack[machine->sp + 1].type = STACK_TYPE_HEAP_PTR;
    machine->stack[machine->sp + 1].addr = vm_execute_deref(machine, gc_get_struct_ref(machine->collector, machine->stack[machine->sp].addr, code->son.number));
    machine->sp++;
}

void vm_execute_mark(vm * machine, bytecode * code)
{
    if (!vm_execute_check_size(machine, machine->sp + 6, machine->tp))
    {
        return;
    }

    gc_stack b_entry = { 0 };
    gc_stack fp_entry = { 0 };
    gc_stack zero_entry = { 0 };

    b_entry.type = STACK_TYPE_PC_OFFSET;
    b_entry.offset = code->mark.offset;

    fp_entry.type = STACK_TYPE_STACK_PTR;
    fp_entry.saddr= machine->fp;

    machine->stack[machine->sp + 6] = b_entry;
    machine->stack[machine->sp + 5] = fp_entry;
    machine->stack[machine->sp + 4] = zero_entry;
    machine->stack[machine->sp + 3] = zero_entry;
    machine->stack[machine->sp + 2] = zero_entry;
    machine->stack[machine->sp + 1] = zero_entry;

    machine->sp = machine->sp + 6;
}

void vm_execute_last_mark(vm * machine, bytecode * code)
{
    if (!vm_execute_check_size(machine, machine->sp + 6, machine->tp))
    {
        return;
    }

    if (machine->fp <= machine->bp)
    {
        machine->sp = machine->sp + 6;
        machine->stack[machine->sp] = machine->stack[machine->fp];

        gc_stack fp_entry = { 0 };
        fp_entry.type = STACK_TYPE_STACK_PTR;
        fp_entry.saddr= machine->fp;

        machine->stack[machine->sp - 1] = fp_entry;
    }
}

void vm_execute_call(vm * machine, bytecode * code)
{
    bytecode_print(code);
    fprintf(stderr, " %u: cannot execute bytecode %s\n", code->addr, bytecode_type_str(code->type));
    assert(0);
}

void vm_execute_call_addr(vm * machine, bytecode * code)
{
    machine->fp = machine->sp - code->call.n;
    machine->pc = code->call.addr;
}

void vm_execute_last_call(vm * machine, bytecode * code)
{
    bytecode_print(code);
    fprintf(stderr, " %u: cannot execute bytecode %s\n", code->addr, bytecode_type_str(code->type));
    assert(0);
}

void vm_execute_last_call_addr(vm * machine, bytecode * code)
{
    if (machine->fp <= machine->bp)
    {
        // call q/h
        machine->fp = machine->sp - code->last_call.n;
        machine->pc = code->last_call.addr;
    }
    else
    {
        // slide size (down) h (variables)
        if (code->last_call.size > 0)
        {
            if (code->last_call.n == 0)
            {
                machine->sp = machine->sp - code->last_call.size;
            }
            else
            {
                unsigned int i;
                machine->sp = machine->sp - code->last_call.size - code->last_call.n;

                for (i = 0; i < code->last_call.n; i++)
                {
                    machine->sp++;
                    machine->stack[machine->sp] =
                        machine->stack[machine->sp + code->last_call.size];
                }
            }
        }
        // jump q/h
        machine->pc = code->last_call.addr;
    }
}

void vm_execute_push_env(vm * machine, bytecode * code)
{
    if (!vm_execute_check_size(machine, machine->fp + code->push_env.size, machine->tp))
    {
        return;
    }

    gc_stack zero_entry = { 0 };
    for (stack_ptr sp = machine->sp + 1; sp <= machine->fp + (stack_ptr)code->push_env.size; sp++)
    {
        machine->stack[sp] = zero_entry;
    }

    machine->sp = machine->fp + code->push_env.size;
}

void vm_execute_pop_env(vm * machine, bytecode * code)
{
    if (machine->bp < machine->fp)
    {
        machine->sp = machine->fp - 6;
    }

    assert(machine->stack[machine->fp].type == STACK_TYPE_PC_OFFSET);
    machine->pc = machine->stack[machine->fp].offset;

    assert(machine->stack[machine->fp - 1].type == STACK_TYPE_STACK_PTR);
    machine->fp = machine->stack[machine->fp - 1].saddr;
}

void vm_execute_set_btp(vm * machine, bytecode * code)
{
    gc_stack hp_entry = { 0 };
    hp_entry.type = STACK_TYPE_HEAP_SIZE;
    hp_entry.addr = gc_get_hp(machine->collector);

    gc_stack tp_entry = { 0 };
    tp_entry.type = STACK_TYPE_STACK_PTR;
    tp_entry.saddr = machine->tp;

    gc_stack bp_entry = { 0 };
    bp_entry.type = STACK_TYPE_STACK_PTR;
    bp_entry.saddr = machine->bp;

    machine->stack[machine->fp - 2] = hp_entry;
    machine->stack[machine->fp - 3] = tp_entry;
    machine->stack[machine->fp - 4] = bp_entry;

    machine->bp = machine->fp;
}

void vm_execute_del_btp(vm * machine, bytecode * code)
{
    assert(machine->stack[machine->fp - 4].type == STACK_TYPE_STACK_PTR);
    machine->bp = machine->stack[machine->fp - 4].saddr;
}

void vm_execute_try(vm * machine, bytecode * code)
{
    gc_stack pc_entry = { 0 };
    pc_entry.type = STACK_TYPE_PC_OFFSET;
    pc_entry.offset = machine->pc;

    machine->stack[machine->fp - 5] = pc_entry;
    machine->pc = code->try.offset;
}

void vm_execute_prune(vm * machine, bytecode * code)
{
    assert(machine->stack[machine->fp - 4].type == STACK_TYPE_STACK_PTR);
    machine->bp = machine->stack[machine->fp - 4].saddr;
}

void vm_execute_set_cut(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    entry.type = STACK_TYPE_STACK_PTR;
    entry.saddr = machine->bp;

    machine->stack[machine->fp - 4] = entry;
}

void vm_execute_fail(vm * machine, bytecode * code)
{
    vm_execute_backtrack(machine);
}

void vm_execute_init(vm * machine, bytecode * code)
{
    machine->sp = machine->fp = machine->bp = 5;

    gc_stack zero_entry = { 0 };
    zero_entry.type = STACK_TYPE_PC_OFFSET;
    zero_entry.offset = 0;

    gc_stack zero_heap = { 0 };
    zero_heap.type = STACK_TYPE_HEAP_PTR;
    zero_heap.addr = 0;

    gc_stack zero_size = { 0 };
    zero_size.type = STACK_TYPE_HEAP_SIZE;
    zero_size.addr = 0;

    gc_stack minus_one_stack = { 0 };
    minus_one_stack.type = STACK_TYPE_STACK_PTR;
    minus_one_stack.saddr = -1;

    gc_stack offset = { 0 };
    offset.type = STACK_TYPE_PC_OFFSET;
    offset.offset = code->init.offset;

    machine->stack[5] = zero_entry;
    machine->stack[4] = zero_heap;
    machine->stack[3] = zero_size;
    machine->stack[2] = minus_one_stack;
    machine->stack[1] = minus_one_stack;
    machine->stack[0] = offset;
}

void vm_execute_halt(vm * machine, bytecode * code)
{
    unsigned int strtab_size = 0;
    char ** strtab_array = NULL;

    if (machine->binary_value_ref)
    {
        strtab_size = machine->binary_value_ref->strtab_size;
        strtab_array = machine->binary_value_ref->strtab_array;
    }

    // print result
    unsigned int i = 0; 
    for (i = 0; i < code->halt.size; i++)
    {
        heap_ptr addr = machine->stack[machine->fp + 1 + i].addr;
        gc_print_ref_str(machine->collector,
                         vm_execute_deref(machine, addr),
                         strtab_array, strtab_size);
        printf("%u: ---------------\n", i);
    }
    printf("-----------------\n");

    /* TODO: backtrack on user's wish */
    vm_execute_backtrack(machine);

    // run garbage collector
    // TODO: write a test which will cause memory reclaim
    // gc_run(machine->collector,
    //       machine->stack, machine->sp, machine->trail, machine->tp);
}

void vm_execute_no(vm * machine, bytecode * code)
{
    //vm_execute_print(machine);
    printf("no\n");
    machine->state = VM_STOP;
}

void vm_execute_jump(vm * machine, bytecode * code)
{
    machine->pc = machine->pc + code->jump.offset;
}

void vm_execute_label(vm * machine, bytecode * code)
{
    /* do nothing machine->pc will be incremented on the next bytecode */
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
    if (!vm_execute_check_size(machine, machine->sp, machine->tp + 1))
    {
        return;
    }

    assert(machine->stack[machine->bp - 2].type == STACK_TYPE_HEAP_SIZE);

    if (ref < machine->stack[machine->bp - 2].addr) {
        gc_stack entry  = { 0 };
        entry.type = STACK_TYPE_HEAP_PTR;
        entry.addr = ref;

        machine->tp = machine->tp + 1;
        machine->trail[machine->tp] = entry;
    }
}

void vm_execute_reset(vm * machine, stack_ptr ref_x, stack_ptr ref_y)
{
    stack_ptr ref_u;
    for (ref_u = ref_y; ref_x < ref_u; ref_u--)
    {
        /* NOTE: second version, Lis. p. 130 */
        if (gc_get_object_type(machine->collector, machine->trail[ref_u].addr) == OBJECT_REF)
        {
            gc_reset_ref(machine->collector, machine->trail[ref_u].addr);
        }
    }
}

void vm_execute_backtrack(vm * machine)
{
    machine->fp = machine->bp;

    assert(machine->stack[machine->fp - 2].type == STACK_TYPE_HEAP_SIZE);
    gc_reset_hp(machine->collector, machine->stack[machine->fp - 2].addr);

    assert(machine->stack[machine->fp - 3].type == STACK_TYPE_STACK_PTR);
    vm_execute_reset(machine, machine->stack[machine->fp - 3].saddr, machine->tp);
    machine->tp = machine->stack[machine->fp - 3].saddr;

    assert(machine->stack[machine->fp - 5].type == STACK_TYPE_PC_OFFSET);
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
        gc_get_atom_idx(machine->collector, ref_u) == gc_get_atom_idx(machine->collector, ref_v))
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
            return 1;
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

char vm_execute_check_size(vm * machine, stack_size_t new_stack_size, stack_size_t new_trail_size)
{
    char size_ok = 1;

    if (new_stack_size >= machine->stack_size ||
        new_trail_size >= machine->trail_size)
    {
        size_ok = 0;
        machine->state = VM_ERROR_OUT_OF_MEMORY;
    }

    return size_ok;
}

int vm_execute(vm * machine, gencode_binary * binary_value)
{
    bytecode * bc = NULL;
    machine->binary_value_ref = binary_value;

    printf("------------\n");

    machine->state = VM_RUNNING;
    while (machine->state == VM_RUNNING)
    {
        bc = machine->binary_value_ref->code_array + machine->pc;
        machine->pc++;

        // bytecode_print(bc);
        vm_execute_op[bc->type].execute(machine, bc);
    }

    if (machine->state == VM_ERROR ||
        machine->state == VM_ERROR_OUT_OF_MEMORY)
    {
        // print machine error state
        vm_execute_print(machine);
        return 1;
    }
    else if (machine->state == VM_STOP)
    {
        return 0;
    }

    return 1;
}

const char * vm_state_to_str(vm_state state)
{
    switch (state)
    {
        case VM_RUNNING: return "VM_RUNNING";
        case VM_ERROR: return "VM_ERROR";
        case VM_STOP: return "VM_STOP";
        case VM_ERROR_OUT_OF_MEMORY: return "VM_ERROR_OUT_OF_MEMORY";
    }
    return "VM_UNKNOWN";
}
