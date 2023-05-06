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
#ifndef __GOAL_H__
#define __GOAL_H__

#include "var.h"
#include "term.h"

typedef struct clause clause;
typedef struct symtab symtab;

typedef enum goal_type { 
    GOAL_TYPE_UNKNOW,
    GOAL_TYPE_LITERAL,
    GOAL_TYPE_UNIFICATION,
    GOAL_TYPE_CUT,
    GOAL_TYPE_FAIL
} goal_type;

typedef struct goal_literal {
    char * name;
    term_list * terms;
    clause * predicate_ref;
} goal_literal;

typedef struct goal_unification {
    var * variable;
    term * term_value;
} goal_unification;

typedef struct goal_cut {
    symtab * symtab_ref;
} goal_cut;

typedef struct goal_fail {
    char * name;
} goal_fail;

typedef struct goal {
    goal_type type;
    union {
        goal_literal literal;
        goal_unification unification;
        goal_cut cut;
        goal_fail fail;
    };
    unsigned int line_no;
    struct goal * next;
} goal;

typedef struct goal_list {
    goal * head;
    goal ** tail;
} goal_list;

goal * goal_new_literal(char * name, term_list * terms);
goal * goal_new_unification(var * variable, term * term_value);
goal * goal_new_cut();
goal * goal_new_fail(char * name);
void goal_delete(goal * value);
char goal_is_last(goal * value);
void goal_print(goal * value);

goal_list * goal_list_new();
void goal_list_delete(goal_list * list);
void goal_list_add_end(goal_list * list, goal * value);

void goal_list_print(goal_list * list);

#endif /* __GOAL_H__ */
