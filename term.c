/**
 * Copyright 2023 Slawomir Maludzinski
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

term * term_new_list_constructor(term_list * terms, term * tail)
{
	if (terms == NULL || terms->head == NULL)
	{
		return NULL;
	}

	term * node = terms->head;

	term_list * t = term_list_new();
	term_list_add_end(t, node);

	term * first = term_new_struct(TERM_TYPE_LIST, strdup("[|]"), t);

	node = node->next;
	while (node != NULL)
	{
		term_list * ti = term_list_new();
		term_list_add_end(ti, node);

		term * next = term_new_struct(TERM_TYPE_LIST, strdup("[|]"), ti);
		term_list_add_end(t, next);

		t = ti;
		node = node->next;
	}

	term_list_add_end(t, tail);

	return first;
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

void term_list_delete_null(term_list * list)
{
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
