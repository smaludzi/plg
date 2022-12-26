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

#include "list.h"
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
    List * terms;
    var * var_value;
    unsigned int line_no;
} term;

term * term_new(term_type type, char * name);
term * term_new_var(term_type, var * var_value);
term * term_new_list(term_type type, char * name, List * terms);
void term_delete(term * t);

void term_deallocator(void * data);

void term_print(term * t);
void term_list_print(List * l);

char * term_type_to_str(term_type value);

/* private */
void term_print_rec(term * t);
void term_list_print_rec(List * l);

#endif /* __TERM_H__ */

