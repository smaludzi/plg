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

#include "list.h"
#include "symtab.h"

typedef struct clause {
    char * name;
    List * terms;
    List * goals;
    symtab * stab;
    unsigned int line_no;
} clause;

clause * clause_new(char * name, List * terms, List * goals);
void clause_delete(clause * value);
void clause_deallocator(void * value);

void clause_print(clause * value);
void clause_list_print(List * list);

#endif /* __CLAUSE_H__ */
