/*
 * term.h
 *
 * Copyright (C) 2007 Slawomir Maludzinski
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
#ifndef __TERM_H__
#define __TERM_H__ 1

#include "var.h"

typedef enum term_type {
    TERM_TYPE_UNKNOWN = 0,
    TERM_TYPE_ANON = 1,
    TERM_TYPE_ATOM = 2,
    TERM_TYPE_VAR = 3,
    TERM_TYPE_TERM = 4
} term_type;

typedef struct term {
    term_type type;
    char * name;
    struct term_list * terms;
    var * var_value;
    unsigned int line_no;
    struct term * next;
} term;

typedef struct term_list {
    term * head;
    term ** tail;
    unsigned int size;
} term_list;

term * term_new(term_type type, char * name);
term * term_new_var(term_type, var * var_value);
term * term_new_list(term_type type, char * name, term_list * terms);
void term_delete(term * t);

void term_deallocator(void * data);

void term_print(term * t);

term_list * term_list_new();
void term_list_delete(term_list * list);
void term_list_add_end(term_list * list, term * value);

void term_list_print(term_list * list);
unsigned int term_list_size(term_list * list);

char * term_type_to_str(term_type value);

/* private */
void term_print_rec(term * t);
void term_list_print_rec(term_list * list);

#endif /* __TERM_H__ */

