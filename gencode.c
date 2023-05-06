/** 
 * Copyright 2021 Slawomir Maludzinski
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
#include "gencode.h"
#include "bytecode.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

gencode * gencode_new()
{
    gencode * value = (gencode *)malloc(sizeof(gencode));

    value->strtab_value = strtab_new(32);
    value->current_addr = 0;
    value->list = bytecode_list_new();

    return value;
}

void gencode_delete(gencode * value)
{
    if (value->strtab_value)
    {
        strtab_delete(value->strtab_value);
    }
    if (value->list)
    {
        bytecode_list_delete(value->list);
    }
    free(value);
}

gencode_binary * gencode_binary_new()
{
    gencode_binary * value = (gencode_binary *)malloc(sizeof(gencode_binary));

    value->strtab_array = NULL;
    value->strtab_size = 0;
    value->code_array = NULL;
    value->code_size = 0;

    return value;
}

void gencode_binary_delete(gencode_binary * value)
{
    if (value->strtab_array != NULL)
    {
        strtab_array_delete(value->strtab_array, value->strtab_size);
    }
    if (value->code_array != NULL)
    {
        bytecode_array_delete(value->code_array);
    }
    free(value);
}

void gencode_binary_generate(gencode_binary * value, gencode * gen)
{
    bytecode_list_set_addr(gen->list);
    strtab_to_array(gen->strtab_value, &value->strtab_array, &value->strtab_size);
    bytecode_to_array(gen->list, &value->code_array, &value->code_size);
}

bytecode * gencode_add_bytecode(gencode * value, bytecode * code)
{
    bytecode * ret = NULL;

    code->addr = value->current_addr++;
    ret = bytecode_list_add_end(value->list, code);

    return ret;
}

void var_gencode(gencode * gen, var * value, gencode_result * result)
{
    switch (value->type)
    {
        case VAR_TYPE_BOUND:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_PUT_REF;
            bc.put_ref.index = value->bound_to->index;
            gencode_add_bytecode(gen, &bc);
            /* printf("PUT_REF name %s index %d\n", value->name, value->bound_to->index); */
        }
        break;
        case VAR_TYPE_UNBOUND:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_PUT_VAR;
            bc.put_ref.index = value->index;
            gencode_add_bytecode(gen, &bc);
            /* printf("PUT_VAR name %s index %d\n", value->name, value->index); */
        }
        break;
        case VAR_TYPE_UNKNOWN:
            /* printf("PUT_VAR_UNKNOWN!!! %d: %s\n", value->line_no, value->name); */
            assert(0);
        break;
    }
}

void var_unify_gencode(gencode * gen, var * value, gencode_result * result)
{
    switch (value->type)
    {
        case VAR_TYPE_BOUND:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_U_REF;
            bc.u_ref.index = value->bound_to->index;
            gencode_add_bytecode(gen, &bc);
            /* printf("U_REF name %s index %d\n", value->name, value->bound_to->index); */
        }
        break;
        case VAR_TYPE_UNBOUND:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_U_VAR;
            bc.u_var.index = value->index;
            gencode_add_bytecode(gen, &bc);
            /* printf("U_VAR name %s index %d\n", value->name, value->index); */
        }
        break;
        case VAR_TYPE_UNKNOWN:
            /* printf("PUT_VAR_UNKNOWN!!! %d: %s\n", value->line_no, value->name); */
            assert(0);
        break;
    }
}

void var_check_gencode(gencode * gen, var * value, gencode_result * result)
{
    bytecode bc = { 0 };
    bc.type = BYTECODE_CHECK;
    bc.check.index = value->bound_to->index;
    gencode_add_bytecode(gen, &bc);
    /* printf("CHECK name %s index %d\n", value->name, value->bound_to->index); */
}

void var_get_bound_vars_gencode(gencode * gen, var * value, var_list * bound_vars, gencode_result * result)
{
    switch (value->type)
    {
        case VAR_TYPE_BOUND:
            var_list_add_end(bound_vars, value);
        break;
        case VAR_TYPE_UNBOUND:
        break;
        case VAR_TYPE_UNKNOWN:
            assert(0);
        break;
    }
}

void var_list_check_gencode(gencode * gen, var_list * bound_vars, gencode_result * result)
{
    var_node * node = bound_vars->head;
    while (node != NULL)
    {
        var * var_value = node->value;
        if (var_value)
        {
            var_check_gencode(gen, var_value, result);
        }
        node = node->next;
    }
}

void term_gencode(gencode * gen, term * value, gencode_result * result)
{
    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
            assert(0);
        break;
        case TERM_TYPE_ANON:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_PUT_ANON;
            gencode_add_bytecode(gen, &bc);
            /* printf("PUT_ANON _\n"); */
        }
        break;
        case TERM_TYPE_ATOM:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_PUT_ATOM;
            bc.put_atom.idx = strtab_add_string(gen->strtab_value, value->name);
            gencode_add_bytecode(gen, &bc);
            /* printf("PUT_ATOM %s\n", value->name); */
        }
        break;
        case TERM_TYPE_VAR:
        {
            var_gencode(gen, value->var_value, result);
        }
        break;
        case TERM_TYPE_TERM:
        {
            term_list_gencode(gen, value->terms, result);

            bytecode bc = { 0 };
            bc.type = BYTECODE_PUT_STRUCT;
            bc.put_struct.n = value->terms->size;
            bc.put_struct.predicate_ref = value->predicate_ref;
            gencode_add_bytecode(gen, &bc);
            /* printf("PUT_STRUCT %s/%d\n", value->name, term_list_size(value->terms)); */
        }
        break;
    }
}

void term_unify_gencode(gencode * gen, term * value, gencode_result * result)
{
    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
            assert(0);
        break;
        case TERM_TYPE_ANON:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_POP;
            gencode_add_bytecode(gen, &bc);
            /* printf("POP\n"); */
        }
        break;
        case TERM_TYPE_ATOM:
        {
            bytecode bc = { 0 };
            bc.type = BYTECODE_U_ATOM;
            bc.u_atom.idx = strtab_add_string(gen->strtab_value, value->name);
            gencode_add_bytecode(gen, &bc);
            /* printf("UATOM %s\n", value->name); */
        }
        break;
        case TERM_TYPE_VAR:
            var_unify_gencode(gen, value->var_value, result);
        break;
        case TERM_TYPE_TERM:
        {
            bytecode bc_u_struct = { 0 };
            bytecode * bc_u_struct_ptr;
            bc_u_struct.type = BYTECODE_U_STRUCT;
            bc_u_struct.u_struct.offset = 0;
            bc_u_struct.u_struct.n = term_list_size(value->terms);
            bc_u_struct.u_struct.predicate_ref = value->predicate_ref;
            bc_u_struct_ptr = gencode_add_bytecode(gen, &bc_u_struct);
            /* printf("USTRUCT %s/%d A\n", value->name, term_list_size(value->terms)); */

            term_list_unify_gencode(gen, value->terms, result);

            bytecode bc_up = { 0 };
            bytecode * bc_up_ptr;
            bc_up.type = BYTECODE_UP;
            bc_up.up.offset = 0;
            bc_up_ptr = gencode_add_bytecode(gen, &bc_up);
            /* printf("UP B\n"); */

            bytecode bc_label_a = { 0 };
            bytecode * bc_label_a_ptr;
            bc_label_a.type = BYTECODE_LABEL;
            bc_label_a_ptr = gencode_add_bytecode(gen, &bc_label_a);
            bc_u_struct_ptr->u_struct.offset = bc_label_a_ptr->addr;
            /* printf("A:\n"); */

            var_list * bound_vars = var_list_new();
            term_list_get_bound_vars_gencode(gen, value->terms, bound_vars, result);
            var_list_check_gencode(gen, bound_vars, result);
            var_list_delete_null(bound_vars);
            term_gencode(gen, value, result);

            bytecode bc_bind = { 0 };
            bc_bind.type = BYTECODE_BIND;
            gencode_add_bytecode(gen, &bc_bind);
            /* printf("BIND\n"); */

            bytecode bc_label_b = { 0 };
            bytecode * bc_label_b_ptr;
            bc_label_b.type = BYTECODE_LABEL;
            bc_label_b_ptr = gencode_add_bytecode(gen, &bc_label_b);
            bc_up_ptr->up.offset = bc_label_b_ptr->addr;
            /* printf("B:\n"); */
        }
        break;
    }
}

void term_get_bound_vars_gencode(gencode * gen, term * value, var_list * bound_vars, gencode_result * result)
{
    switch (value->type)
    {
        case TERM_TYPE_UNKNOWN:
            assert(0);
        break;
        case TERM_TYPE_ANON:
        break;
        case TERM_TYPE_ATOM:
        break;
        case TERM_TYPE_VAR:
            var_get_bound_vars_gencode(gen, value->var_value, bound_vars, result);
        break;
        case TERM_TYPE_TERM:
            term_list_get_bound_vars_gencode(gen, value->terms, bound_vars, result);
        break;
    }
}

void term_list_gencode(gencode * gen, term_list * list, gencode_result * result)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_gencode(gen, node, result);
        node = node->next;
    }
}

void term_list_unify_gencode(gencode * gen, term_list * list, gencode_result * result)
{
    unsigned int son_number = 0;

    term * node = list->head;
    while (node != NULL)
    {
        bytecode bc = { 0 };
        bc.type = BYTECODE_SON;
        bc.son.number = son_number++;
        gencode_add_bytecode(gen, &bc);
        /* printf("SON %u\n", bc.son.number); */

        term_unify_gencode(gen, node, result);
        node = node->next;
    }
}

void term_list_get_bound_vars_gencode(gencode * gen, term_list * list, var_list * bound_vars, gencode_result * result)
{
    term * node = list->head;
    while (node != NULL)
    {
        term_get_bound_vars_gencode(gen, node, bound_vars, result);
        node = node->next;
    }
}

void goal_literal_gencode(gencode * gen, goal_literal * value, gencode_result * result)
{
    bytecode bc = { 0 };
    bytecode * bc_ptr;
    bc.type = BYTECODE_MARK;
    bc.mark.offset = 0;
    bc_ptr = gencode_add_bytecode(gen, &bc);
    /* printf("MARK B\n"); */

    if (value->terms != NULL)
    {
        term_list_gencode(gen, value->terms, result);
    }

    bytecode bc_call = { 0 };
    bc_call.type = BYTECODE_CALL;
    bc_call.call.n = term_list_size(value->terms);
    bc_call.call.predicate_ref = value->predicate_ref;
    gencode_add_bytecode(gen, &bc_call);
    /* printf("CALL %s/%u\n", value.name, term_list_size(value.terms)); */

    bytecode bc_label = { 0 };
    bc_label.type = BYTECODE_LABEL;
    gencode_add_bytecode(gen, &bc_label);
    bc_ptr->mark.offset = bc_label.addr;
    /* printf("B: ...\n"); */
}

char goal_is_last_literal_opt_gencode(clause * clause_value, goal * goal_value)
{
    /* TODO: check if comparing
             clause_value->predicate_ref == goal_value->literal.predicate_ref
             is sufficient */
    return (clause_value != NULL &&
            goal_value != NULL &&
            goal_is_last(goal_value) &&
            strcmp(clause_value->name, goal_value->literal.name) == 0 &&
            var_list_size(clause_value->vars) == term_list_size(goal_value->literal.terms));
}

void goal_last_literal_gencode(gencode * gen, clause * clause_value, goal_literal * value, gencode_result * result)
{
    bytecode bc_lm = { 0 };
    bc_lm.type = BYTECODE_LAST_MARK;
    gencode_add_bytecode(gen, &bc_lm);

    if (value->terms != NULL)
    {
        term_list_gencode(gen, value->terms, result);
    }

    bytecode bc_lc = { 0 };
    bc_lc.type = BYTECODE_LAST_CALL;
    bc_lc.last_call.size = var_list_size(clause_value->local_vars);
    bc_lc.last_call.n = term_list_size(value->terms);
    bc_lc.last_call.predicate_ref = value->predicate_ref;
    gencode_add_bytecode(gen, &bc_lc);
}

void goal_unification_gencode(gencode * gen, goal_unification * value, gencode_result * result)
{
    switch (value->variable->type)
    {
    case VAR_TYPE_UNBOUND:
    {
        var_gencode(gen, value->variable, result);
        term_gencode(gen, value->term_value, result);

        bytecode bc = { 0 };
        bc.type = BYTECODE_BIND;
        gencode_add_bytecode(gen, &bc);
        /* printf("BIND\n"); */
        break;
    }
    case VAR_TYPE_BOUND:
    {
        var_gencode(gen, value->variable, result);
        term_unify_gencode(gen, value->term_value, result);
        break;
    }
    default:
        var_gencode(gen, value->variable, result);
        term_gencode(gen, value->term_value, result);
        assert(0);
    }
}

void goal_cut_gencode(gencode * gen, unsigned int local_vars, goal_cut * value, gencode_result * result)
{
    bytecode prune_bc = { 0 };
    prune_bc.type = BYTECODE_PRUNE;
    gencode_add_bytecode(gen, &prune_bc);

    bytecode push_env_bc = { 0 };
    push_env_bc.type = BYTECODE_PUSH_ENV;
    push_env_bc.push_env.size = local_vars;
    gencode_add_bytecode(gen, &push_env_bc);
}

void goal_fail_gencode(gencode * gen, goal * goal, gencode_result * result)
{
    bytecode bc = { 0 };
    bc.type = BYTECODE_FAIL;
    gencode_add_bytecode(gen, &bc);
}

void goal_gencode(gencode * gen, clause * clause_value, unsigned int local_vars, goal * value, gencode_result * result)
{
    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
        {
            if (goal_is_last_literal_opt_gencode(clause_value, value))
            {
                goal_last_literal_gencode(gen, clause_value, &value->literal, result);
            }
            else
            {
                goal_literal_gencode(gen, &value->literal, result);
            }
            break;
        }
        case GOAL_TYPE_UNIFICATION:
        {
            goal_unification_gencode(gen, &value->unification, result);
            break;
        }
        case GOAL_TYPE_CUT:
        {
            goal_cut_gencode(gen, local_vars, &value->cut, result);
            break;
        }
        case GOAL_TYPE_FAIL:
        {
            goal_fail_gencode(gen, value, result);
            break;
        }
        case GOAL_TYPE_UNKNOW:
        {
            assert(0);
        }
    }
}

void goal_list_gencode(gencode * gen, clause * clause_value, unsigned int local_vars, goal_list * list, gencode_result * result)
{
    goal * node = list->head;
    while (node != NULL)
    {
        goal_gencode(gen, clause_value, local_vars, node, result);
        node = node->next;
    }
}

void clause_head_get_local_vars_gencode(gencode * gen, var_list * vars, var_list * local_vars)
{
    var_node * node = vars->head;
    while (node != NULL)
    {
        if (node->value != NULL)
        {
            var_list_add_end(local_vars, node->value);
        }
        node = node->next;
    }
}

void clause_gencode(gencode * gen, clause * value, gencode_result * result)
{
    unsigned int local_vars = var_list_size(value->local_vars);

    bytecode bc_push_env = { 0 };
    bc_push_env.type = BYTECODE_PUSH_ENV;
    bc_push_env.push_env.size = local_vars;
    gencode_add_bytecode(gen, &bc_push_env);
    /* printf("PUSHENV %u\n", local_vars->size); */

    goal_list_gencode(gen, value, local_vars, value->goals, result);

    bytecode bc_pop_env = { 0 };
    bc_pop_env.type = BYTECODE_POP_ENV;
    gencode_add_bytecode(gen, &bc_pop_env);
    /* printf("POPENV\n"); */
}

void predicate_0_gencode(gencode * gen, clause * value, gencode_result * result)
{
    bytecode bc = { 0 };
    bc.type = BYTECODE_LABEL;
    gencode_add_bytecode(gen, &bc);
    value->addr = bc.addr;

    if (value->with_cut)
    {
        bytecode bc = { 0 };
        bc.type = BYTECODE_SET_CUT;
        gencode_add_bytecode(gen, &bc);
    }

    clause_gencode(gen, value, result);
}

void predicate_N_gencode(gencode * gen, clause_list * list, gencode_result * result)
{
    bytecode bc_addr = { 0 };
    bc_addr.type = BYTECODE_LABEL;
    gencode_add_bytecode(gen, &bc_addr);

    bytecode bc_set_btp = { 0 };
    bc_set_btp.type = BYTECODE_SET_BTP;
    gencode_add_bytecode(gen, &bc_set_btp);
    /* printf("SETBTP\n"); */

    // allocate one more so it is easy to calculate clause indexes from 1
    bytecode ** try_arr = malloc(sizeof(bytecode *) * (list->size + 1));

    unsigned int clause_nbr = 1;
    clause_node * node = list->head;
    assert(node);
    node->value->addr = bc_addr.addr;

    node = node->next; /* omit one clause */
    while (node != NULL)
    {
        if (node->value != NULL)
        {
            bytecode bc_try = { 0 };
            bc_try.type = BYTECODE_TRY;
            bc_try.try.offset = 0;
            try_arr[clause_nbr] = gencode_add_bytecode(gen, &bc_try);
            /* printf("TRY A%u\n", clause_nbr); */

            clause_nbr++;
        }
        node = node->next;
    }

    bytecode bc_del_btp = { 0 };
    bc_del_btp.type = BYTECODE_DEL_BTP;
    gencode_add_bytecode(gen, &bc_del_btp);
    /* printf("DELBTP\n"); */

    bytecode bc_jump = { 0 };
    bytecode * bc_jump_ptr;
    bytecode * bc_last_clause_ptr;
    bc_jump.type = BYTECODE_JUMP;
    bc_jump_ptr = gencode_add_bytecode(gen, &bc_jump);
    // cheating so it is easy to generate jumps below
    try_arr[clause_nbr] = bc_jump_ptr;
    /* printf("JUMP A%u\n", clause_nbr); */

    clause_nbr = 1;
    node = list->head;
    while (node != NULL)
    {
        if (node->value != NULL)
        {
            bytecode bc_label = { 0 };
            bytecode * bc_label_ptr;
            bc_label.type = BYTECODE_LABEL;
            bc_last_clause_ptr = bc_label_ptr = gencode_add_bytecode(gen, &bc_label);
            try_arr[clause_nbr]->try.offset = bc_label_ptr->addr;
            //try_arr[clause_nbr]->try.offset = bc_label_ptr->addr; - try_arr[clause_nbr]->addr;

            /* printf("A%u:\n", clause_nbr); */
            clause_nbr++;

            clause_gencode(gen, node->value, result);
        }
        node = node->next;
    }
    bc_jump_ptr->jump.offset = bc_last_clause_ptr->addr - bc_jump_ptr->addr;

    free(try_arr);
}

void predicate_gencode(gencode * gen, clause_list * list, gencode_result * result)
{
    if (list->size == 1)
    {
        clause_node * node = list->head;
        if (node && node->value != NULL)
        {
            predicate_0_gencode(gen, node->value, result);
        }
    }
    else
    {
        predicate_N_gencode(gen, list, result);
    }
}

void predicate_gather_gencode(gencode * gen, clause_node * first, gencode_result * result)
{
    char first_clause = 0;
    char * predicate_name = NULL;
    unsigned int predicate_arity = 0;
    clause_list * predicates = clause_list_new();

    clause_node * node = first;
    while (node != NULL)
    {
        clause * value = node->value;
        if (value && value->gencode == 0)
        {
            if (first_clause == 0)
            {
                first_clause = 1;
                value->gencode = 1;
                predicate_name = value->name;
                predicate_arity = clause_arity(value);
                clause_list_add_end(predicates, value);
            }
            else
            {
                if (strcmp(value->name, predicate_name) == 0 &&
                    predicate_arity == clause_arity(value))
                {
                    value->gencode = 1;
                    clause_list_add_end(predicates, value);
                }
            }
        }
        node = node->next;
    }

    if (predicates->size > 0)
    {
        predicate_gencode(gen, predicates, result);
    }

    clause_list_delete_null(predicates);
}

void clause_list_gencode(gencode * gen, clause_list * list, gencode_result * result)
{
    clause_node * node = list->head;
    while (node != NULL)
    {
        predicate_gather_gencode(gen, node, result);
        node = node->next;
    }
}

void query_gencode(gencode * gen, query * value, gencode_result * result)
{
    bytecode bc_init = { 0 };
    bytecode * bc_init_ptr;
    bc_init.type = BYTECODE_INIT;
    bc_init.init.offset = 0;
    bc_init_ptr = gencode_add_bytecode(gen, &bc_init);
    /* printf("INIT A\n"); */

    unsigned int local_vars = symtab_size_type(value->stab, SYMTAB_VAR);

    bytecode bc_push_env = { 0 };
    bc_push_env.type = BYTECODE_PUSH_ENV;
    bc_push_env.push_env.size = local_vars;
    gencode_add_bytecode(gen, &bc_push_env);
    /* printf("PUSHENV %u\n", symtab_size_type(value->stab, SYMTAB_VAR)); */

    if (value->with_cut)
    {
        bytecode bc_set_cut = { 0 };
        bc_set_cut.type = BYTECODE_SET_CUT;
        gencode_add_bytecode(gen, &bc_set_cut);
        /* printf("SET_CUT\n"); */
    }

    goal_list_gencode(gen, NULL, local_vars, value->goals, result);

    bytecode bc_halt = { 0 };
    bc_halt.type = BYTECODE_HALT;
    bc_halt.halt.size = local_vars;
    gencode_add_bytecode(gen, &bc_halt);
    /* printf("HALT %u\n", symtab_size_type(value->stab, SYMTAB_VAR)); */

    bytecode bc_label = { 0 };
    bytecode * bc_label_ptr;
    bc_label.type = BYTECODE_LABEL;
    bc_label_ptr = gencode_add_bytecode(gen, &bc_label);
    bc_init_ptr->init.offset = bc_label_ptr->addr;
    /* printf("A:\n"); */

    bytecode bc_no = { 0 };
    bc_no.type = BYTECODE_NO;
    gencode_add_bytecode(gen, &bc_no);
    /* printf("NO\n"); */
}

void program_gencode(gencode * gen, program * value, gencode_result * result)
{
    query_gencode(gen, value->query_value, result);
    clause_list_gencode(gen, value->clausies, result);
}

