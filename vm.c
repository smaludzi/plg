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

    vm_execute_test();

    return machine;
}

void vm_delete(vm * machine)
{
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

}

void vm_execute_put_ref(vm * machine, bytecode * code)
{

}

void vm_execute_put_var(vm * machine, bytecode * code)
{

}

void vm_execute_u_ref(vm * machine, bytecode * code)
{

}

void vm_execute_u_var(vm * machine, bytecode * code)
{

}

void vm_execute_check(vm * machine, bytecode * code)
{

}

void vm_execute_put_anon(vm * machine, bytecode * code)
{

}

void vm_execute_put_atom(vm * machine, bytecode * code)
{

}

void vm_execute_put_struct(vm * machine, bytecode * code)
{

}

void vm_execute_put_struct_addr(vm * machine, bytecode * code)
{

}

void vm_execute_u_atom(vm * machine, bytecode * code)
{

}

void vm_execute_u_struct(vm * machine, bytecode * code)
{

}

void vm_execute_u_struct_addr(vm * machine, bytecode * code)
{

}

void vm_execute_up(vm * machine, bytecode * code)
{

}

void vm_execute_bind(vm * machine, bytecode * code)
{

}

void vm_execute_son(vm * machine, bytecode * code)
{

}

void vm_execute_mark(vm * machine, bytecode * code)
{

}

void vm_execute_call(vm * machine, bytecode * code)
{

}

void vm_execute_call_addr(vm * machine, bytecode * code)
{

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

}

void vm_execute_jump(vm * machine, bytecode * code)
{

}

void vm_execute_label(vm * machine, bytecode * code)
{

}

void vm_execute(vm * machine, gencode_binary * binary_value)
{
    machine->binary_value_ref = binary_value;
}
