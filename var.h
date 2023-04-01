/*
 * var.h
 *
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
#ifndef __VAR_H__
#define __VAR_H__

typedef enum var_type {
    VAR_TYPE_UNKNOWN = 0,
    VAR_TYPE_BOUND = 1,
    VAR_TYPE_UNBOUND = 2
} var_type;

typedef struct var
{
    var_type type;
    char * name;
    struct var * bound_to;
    unsigned int index;
    unsigned int line_no;
} var;

typedef struct var_node
{
    var * value;
    struct var_node * next;
} var_node;

typedef struct var_list
{
    var_node * head;
    var_node ** tail;
    unsigned int size;
} var_list;

var * var_new(char * name);
void var_delete(var * value);

void var_print(var * value);

var_node * var_node_new(var * value);
void var_node_delete(var_node * value);
void var_node_delete_null(var_node * value);

var_list * var_list_new();
void var_list_delete(var_list * list);
void var_list_delete_null(var_list * list);
void var_list_add_end(var_list * list, var * value);
unsigned int var_list_size(var_list * list);
void var_list_print(var_list * list);

char * var_type_to_str(var_type type);

#endif /* __VAR_H__ */

