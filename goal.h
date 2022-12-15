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

#include "list.h"
#include "term.h"

typedef enum goal_type { 
    GOAL_TYPE_UNKNOW,
    GOAL_TYPE_LITERAL,
    GOAL_TYPE_UNIFICATION
} goal_type;

typedef struct goal_literal {
    char * name;
    List * terms;
} goal_literal;

typedef struct goal_unification {
    char * variable;
    term * term_value;
} goal_unification;

typedef struct goal {
    goal_type type;
    union {
        goal_literal literal;
        goal_unification unification;
    };
    unsigned int line_no;
} goal;

goal * goal_new_literal(char * name, List * terms);
goal * goal_new_unification(char * variable, term * term_value);
void goal_delete(goal * value);
void goal_deallocator(void * value);

void goal_print(goal * value);
void goal_list_print(List * goals);

#endif /* __GOAL_H__ */