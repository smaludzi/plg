/*
 * term.c
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "term.h"
#include "var.h"

term * term_new_basic(term_type type, char * name)
{
	term * value = malloc(sizeof(term));

	value->type = type;
	value->t_basic.name = name;
	value->predicate_ref = NULL;
	value->next = NULL;

	return value;
}

term * term_new_var(term_type type, var * var_value)
{
	term * value = malloc(sizeof(term));

	value->type = type;
	value->t_var.value = var_value;
	value->predicate_ref = NULL;
	value->next = NULL;

	return value;
}

term * term_new_struct(term_type type, char * name, term_list * terms)
{
	term * t;
	
	t = malloc(sizeof(term));
	if (t == NULL)
	{
		fprintf(stderr, "cannot allocate term\n");
	}
	
	t->type = type;
	t->t_struct.name = name;
	t->t_struct.terms = terms;
	t->predicate_ref = NULL;
	t->next = NULL;
	
	return t;
}

term * term_new_int(term_type type, int int_val)
{
	term * value = malloc(sizeof(term));

	value->type = type;
	value->t_int.value = int_val;
	value->predicate_ref = NULL;
	value->next = NULL;

	return value;
}

void term_delete(term * t)
{
	switch (t->type)
	{
		case TERM_TYPE_UNKNOWN:
			assert(0);
		break;
		case TERM_TYPE_ANON:
		case TERM_TYPE_ATOM:
		case TERM_TYPE_LIST_EMPTY:
			if (t->t_basic.name)
			{
				free(t->t_basic.name);
			}
		break;
		case TERM_TYPE_VAR:
			if (t->t_var.value)
			{
				var_delete(t->t_var.value);
			}
		break;
		case TERM_TYPE_LIST:
		case TERM_TYPE_STRUCT:
			if (t->t_struct.name)
			{
				free(t->t_struct.name);
			}
			if (t->t_struct.terms)
			{
				term_list_delete(t->t_struct.terms);
			}
		break;
		case TERM_TYPE_INT:
		break;
	}
	free(t);
}

unsigned int term_arity(term * t)
{
	switch (t->type)
	{
		case TERM_TYPE_UNKNOWN: assert(0);
		case TERM_TYPE_ANON: return 0;
		case TERM_TYPE_ATOM: return 0;
		case TERM_TYPE_VAR: return 0;
		case TERM_TYPE_STRUCT: return term_list_size(t->t_struct.terms);
		case TERM_TYPE_LIST_EMPTY: return 0;
		case TERM_TYPE_LIST: return 2;
		case TERM_TYPE_INT: return 0;
	}
	return 0;
}

void term_print(term * t)
{
	if (t == NULL)
	{
		return;
	}

	term_print_rec(t);
}

void term_print_rec(term * t)
{
	if (t == NULL)
	{
		return;
	}

	switch (t->type)
	{
    case TERM_TYPE_UNKNOWN:
		printf("term unknown\n");
		break;
    case TERM_TYPE_ANON:
		printf("term _ %s\n", t->t_basic.name);
		break;
	case TERM_TYPE_ATOM:
    	printf("term atom %s\n", t->t_basic.name);
		break;
	case TERM_TYPE_VAR:
		var_print(t->t_var.value);
		break;
	case TERM_TYPE_LIST:
	case TERM_TYPE_STRUCT:
		printf("term %s/%u\n", t->t_struct.name, term_list_size(t->t_struct.terms));
		term_list_print_rec(t->t_struct.terms);
		break;
	case TERM_TYPE_LIST_EMPTY:
		printf("term empty list\n");
		break;
	case TERM_TYPE_INT:
		printf("term int %d\n", t->t_int.value);
		break;
	}
}

term_list * term_list_new()
{
	term_list * list = malloc(sizeof(term_list));

	list->head = NULL;
	list->tail = &list->head;
	list->size = 0;

	return list;
}

void term_list_delete(term_list * list)
{
	term * node = list->head;
	while (node != NULL)
	{
		term * next = node->next;
		term_delete(node);
		node = next;
	}
	free(list);
}

void term_list_add_end(term_list * list, term * value)
{
	list->size++;
	*(list->tail) = value;
	list->tail = &value->next;
}

unsigned int term_list_size(term_list * list)
{
	if (list == NULL)
	{
		return 0;
	}
	return list->size;
}

unsigned int term_list_arity(term_list * list)
{
	if (list == NULL)
	{
		return 0;
	}
	return list->size;
}

void term_list_print(term_list * list)
{
	if (list == NULL)
	{
		return;
	}
	term_list_print_rec(list);
}

void term_list_print_rec(term_list * list)
{
	char first = 1;

	term * node = list->head;
	while (node)
	{
		if (first == 0)
		{
			printf(", ");
		}
		term_print_rec(node);
		first = 0;
		node = node->next;
	}
}

char * term_type_to_str(term_type value)
{
	switch (value)
	{
		case TERM_TYPE_UNKNOWN: return "TERM_TYPE_UNKNOWN";
		case TERM_TYPE_ANON: return "TERM_TYPE_ANON";
		case TERM_TYPE_ATOM: return "TERM_TYPE_ATOM";
		case TERM_TYPE_VAR: return "TERM_TYPE_VAR";
		case TERM_TYPE_STRUCT: return "TERM_TYPE_STRUCT";
		case TERM_TYPE_LIST_EMPTY: return "TERM_TYPE_LIST_EMPTY";
		case TERM_TYPE_LIST: return "TERM_TYPE_LIST";
		case TERM_TYPE_INT: return "TERM_TYPE_INT";
	}
	return "TERM_TYPE_UNKNOWN";
}
