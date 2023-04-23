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
#ifndef __CLAUSE_H__
#define __CLAUSE_H__

#include "symtab.h"
#include "goal.h"

typedef struct clause {
    char * name;
    var_list * vars;
    goal_list * goals;
    symtab * stab;
    unsigned int line_no;
    char gencode;
    char with_cut;
    unsigned int addr;
} clause;

typedef struct clause_node {
    clause * value;
    struct clause_node * next;
} clause_node;

typedef struct clause_list {
    clause_node * head;
    clause_node ** tail;
    unsigned int size;
} clause_list;

clause * clause_new(char * name, var_list * vars, goal_list * goals);
void clause_delete(clause * value);

unsigned int clause_arity(clause * value);

void clause_print(clause * value);

clause_node * clause_node_new(clause * value);
void clause_node_delete(clause_node * value);
void clause_node_delete_null(clause_node * value);

clause_list * clause_list_new();
void clause_list_delete(clause_list * list);
void clause_list_delete_null(clause_list * list);
void clause_list_add_end(clause_list * list, clause * value);

void clause_list_print(clause_list * list);

#endif /* __CLAUSE_H__ */
