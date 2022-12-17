/*
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
#ifndef __SEMCHECK_H__
#define __SEMCHECK_H__

#include "list.h"
#include "term.h"
#include "goal.h"
#include "clause.h"
#include "query.h"
#include "program.h"
#include "symtab.h"

typedef enum semcheck_result {
    SEMCHECK_SUCCESS = 0,
    SEMCHECK_FAILURE = 1
} semcheck_result;

void var_semcheck(var * value, semcheck_result * result);

void var_add_symtab_semcheck(symtab * stab, var * value, semcheck_result * result);
void var_list_add_symtab_semcheck(symtab * stab, List * list, semcheck_result * result);

void term_semcheck(term * value, semcheck_result * result);
void term_is_variable_semcheck(term * value, semcheck_result * result);
void term_list_is_variable_semcheck(List * list, semcheck_result * result);
void term_list_semcheck(List * list, semcheck_result * result);

void goal_literal_semcheck(goal_literal value, semcheck_result * result);
void goal_unification_semcheck(goal_unification value, semcheck_result * result);
void goal_semcheck(goal * value, semcheck_result * result);
void goal_list_semcheck(List * list, semcheck_result * result);

void clause_semcheck(clause * value, semcheck_result * result);
void clause_list_semcheck(List * list, semcheck_result * result);

void query_semcheck(query * value, semcheck_result * result);
void query_list_semcheck(List * list, semcheck_result * result);

void program_semcheck(program * value, semcheck_result * result);

#endif /* __SEMCHECK_H__ */
