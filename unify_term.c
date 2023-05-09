/*
 * unify_term.c
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
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "var.h"
#include "unify.h"
#include "unify_term.h"

const char * top_func = "__X";

char terms_consistent(term * term1, term * term2)
{
	if (term1 == NULL && term2 == NULL)
	{
		return 1;
	}
	else if ((term1 == NULL && term2 != NULL) ||
	         (term1 != NULL && term2 == NULL))
	{
		return 0;
	}
	
	if (term1->type == TERM_TYPE_ATOM && term2->type == TERM_TYPE_ATOM)
	{
		if (strcmp(term1->t_basic.name, term2->t_basic.name) != 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}	
	}
	else if ((term1->type == TERM_TYPE_VAR  && term2->type == TERM_TYPE_VAR)  ||
	         (term1->type == TERM_TYPE_ATOM && term2->type == TERM_TYPE_VAR)  ||
	         (term1->type == TERM_TYPE_VAR  && term2->type == TERM_TYPE_ATOM) ||
	         (term1->type == TERM_TYPE_STRUCT && term2->type == TERM_TYPE_VAR)  ||
	         (term1->type == TERM_TYPE_VAR  && term2->type == TERM_TYPE_STRUCT))
	{
		return 1;
	}
	else if (term1->type == TERM_TYPE_STRUCT && term2->type == TERM_TYPE_STRUCT)
	{	
			if (strcmp(term1->t_struct.name, term2->t_struct.name) != 0)
			{
				return 0;
			}
			if (term_list_size(term1->t_struct.terms) != term_list_size(term2->t_struct.terms))
			{
				return 0;
			}
			
			term * iter1 = term1->t_struct.terms->head;
			term * iter2 = term2->t_struct.terms->head;
			while (!iter1 && !iter2)
			{
				if (terms_consistent(iter1, iter2) == 0)
				{
					return 0;
				}
			
				iter1 = iter1->next;
				iter2 = iter2->next;
			}
			
			return 1;
	}
	else
	{
		return 0;
	}
}

int unify_terms(term * term1, term * term2)
{
	int ret;
	u_system * R;
	hash * symbol_table;

	symbol_table = create_symbol_table(term1, term2);
	R = create_system(term1, term2, symbol_table);

	u_system_print(R);
	printf("\n\n");

	ret = unify(R);

	u_system_print(R);
	printf("\n\n");

	u_system_delete(R);

	hash_delete(symbol_table);
	
	return ret;
}

hash * create_symbol_table(term * term1, term * term2)
{
	hash * symbol_table;

	symbol_table = hash_new_delete(variable_hash_delete);

	symbol_table_add_variable(symbol_table, term1);
	symbol_table_add_variable(symbol_table, term2);

	/* hash_printer(symbol_table, variable_hash_print); */

	return symbol_table;	
}

u_system * create_system(term * term1, term * term2, hash * symbol_table)
{
	int i;
	u_system * R;
	multi_equation * multi;

	if (symbol_table == NULL)
	{
		return NULL;
	}
	
	R = u_system_new();

	for (i = 0; i < symbol_table->size; i++)
	{
		if (symbol_table->items[i].key != 0)
		{
			variable * var = (variable *)symbol_table->items[i].value;
		
			multi = create_multi_equation_variable(var);
			var->M = multi;
			
			R->U.equations = multi_equation_node_new(multi, R->U.equations);
			R->U.multi_eq_number++;
		}
	}

	multi = create_multi_equation(term1, term2, symbol_table);
	
	R->U.equations = multi_equation_node_new(multi, R->U.equations);
	R->U.multi_eq_number++;
	R->U.zero_counter_multi_equation = multi_equation_node_new(multi, R->U.zero_counter_multi_equation);

	return R;
}

multi_equation * create_multi_equation(term * term1, term * term2, hash * symbol_table)
{
	multi_equation * multi;
	multi_term * mult;
	
	if ((term1->type == TERM_TYPE_VAR && term2->type == TERM_TYPE_VAR) ||
	    (term1->type == TERM_TYPE_VAR && term2->type == TERM_TYPE_STRUCT) ||
	    (term1->type == TERM_TYPE_STRUCT && term2->type == TERM_TYPE_VAR) ||
	    (term1->type == TERM_TYPE_VAR && term2->type == TERM_TYPE_ATOM) ||
	    (term1->type == TERM_TYPE_ATOM && term2->type == TERM_TYPE_VAR))
	{
		temp_mult_eq * multt;
		
		multt = create_temp_mult_eq(term1, term2, symbol_table);

		mult = multi_term_new("__f", NULL);
		mult->args = temp_mult_eq_node_new(multt, mult->args); 
	}
	else
	{
		mult = create_multi_term(term1, term2, symbol_table);
	}
	
	multi = multi_equation_new(mult);
	multi->S = NULL;
	multi->var_number = 1;
	
	return multi;
}

multi_equation * create_multi_equation_variable(variable * var)
{
	multi_equation * multi;
	
	multi = multi_equation_new(NULL);
	multi->S = variable_node_new(var, multi->S);
	multi->var_number = 1;
	
	return multi;
}

multi_term * create_multi_term(term * term1, term * term2, hash * symbol_table)
{
	multi_term * multi;
	term * iter1;
	term * iter2;

	if (!(term1->type == TERM_TYPE_STRUCT ||
	      term1->type == TERM_TYPE_ATOM ||
	      term2->type == TERM_TYPE_ATOM))
	{
		return NULL;
	}

	if (term1->type == TERM_TYPE_STRUCT)
	{
		multi = multi_term_new(term1->t_struct.name, NULL);
	}
	else if (term1->type == TERM_TYPE_ATOM)
	{
		multi = multi_term_new(term1->t_basic.name, NULL);
	}
	
	if (term1->type == TERM_TYPE_STRUCT &&
	    term2->type == TERM_TYPE_STRUCT)
	{
		iter1 = term1->t_struct.terms->head;
		iter2 = term2->t_struct.terms->head;
		while (!iter1 && !iter2)
		{
			temp_mult_eq * mult;
		
			mult = create_temp_mult_eq(iter1, iter2, symbol_table);
				
			multi->args = temp_mult_eq_node_new(mult, multi->args);
		
			iter1 = iter1->next;
			iter2 = iter2->next;
		}
	}

	return multi;
}

multi_term * create_multi_term_single(term * term1, hash * symbol_table)
{
	multi_term * multi;
	term * iter1;

	if (term1->type != TERM_TYPE_STRUCT && term1->type != TERM_TYPE_ATOM)
	{
		return NULL;
	}

	multi = multi_term_new(term1->t_struct.name, NULL);
	
	iter1 = term1->t_struct.terms->head;
	while (!iter1)
	{
		temp_mult_eq * mult;
	
		mult = create_temp_mult_eq_single(iter1, symbol_table);
		multi->args = temp_mult_eq_node_new(mult, multi->args);
	
		iter1 = iter1->next;
	}

	return multi;
}

temp_mult_eq * create_temp_mult_eq(term * term1, term * term2, hash * symbol_table)
{
	temp_mult_eq * mult;
	variable_queue * S = NULL;
	multi_term * M = NULL;
	
	if (term1->type == TERM_TYPE_VAR && term2->type == TERM_TYPE_VAR)
	{
		S = variable_queue_new();
		
		if (strcmp(term1->t_var.value->name, term2->t_var.value->name) == 0)
		{
			variable * var;
			
			var = hash_search(symbol_table, term1->t_var.value->name);
			variable_inc_mult_eq_counter(var, 1);
		
			variable_queue_add_var(S, var);
		}
		else
		{
			variable * var;
			
			var = hash_search(symbol_table, term1->t_var.value->name);
			variable_inc_mult_eq_counter(var, 1);
		
			variable_queue_add_var(S, var);
			
			var = hash_search(symbol_table, term2->t_var.value->name);
			variable_inc_mult_eq_counter(var, 1);
			
			variable_queue_add_var(S, var);
		}
	}
	else if ((term1->type == TERM_TYPE_VAR && term2->type == TERM_TYPE_STRUCT) ||
	         (term1->type == TERM_TYPE_VAR && term2->type == TERM_TYPE_ATOM))
	{
		variable * var;		
		S = variable_queue_new();
		
		var = hash_search(symbol_table, term1->t_var.value->name);
		variable_inc_mult_eq_counter(var, 1);
		
		variable_queue_add_var(S, var);
		
		M = create_multi_term_single(term2, symbol_table);
	}
	else if ((term1->type == TERM_TYPE_STRUCT && term2->type == TERM_TYPE_VAR) ||
	         (term1->type == TERM_TYPE_ATOM && term2->type == TERM_TYPE_VAR))
	{
		variable * var;
		S = variable_queue_new();
		
		var = hash_search(symbol_table, term2->t_var.value->name);
		variable_inc_mult_eq_counter(var, 1);
		
		variable_queue_add_var(S, var);
		
		M = create_multi_term_single(term1, symbol_table);
	}
	else if ((term1->type == TERM_TYPE_STRUCT && term2->type == TERM_TYPE_STRUCT) ||
	         (term1->type == TERM_TYPE_ATOM && term2->type == TERM_TYPE_ATOM))
	{
		M = create_multi_term(term1, term2, symbol_table);
	}
	
	mult = temp_mult_eq_new(S, M);

	return mult;
}

temp_mult_eq * create_temp_mult_eq_single(term * term1, hash * symbol_table)
{
	temp_mult_eq * mult;
	variable_queue * S = NULL;
	multi_term * M = NULL;
	
	if (term1->type == TERM_TYPE_VAR)
	{
		variable * var;
		S = variable_queue_new();
		
		var = hash_search(symbol_table, term1->t_var.value->name);
		variable_inc_mult_eq_counter(var, 1);
		
		variable_queue_add_var(S, var);
	}
	else if (term1->type == TERM_TYPE_STRUCT ||
	         term1->type == TERM_TYPE_ATOM)
	{
		M = create_multi_term_single(term1, symbol_table);
	}
	
	mult = temp_mult_eq_new(S, M);

	return mult;
}

void symbol_table_add_variable(hash * symbol_table, term * term1)
{
	if (term1->type == TERM_TYPE_ATOM)
	{
		/* do nothing */
	}
	else if (term1->type == TERM_TYPE_VAR)
	{
		char c;
		
		c = hash_contains(symbol_table, term1->t_var.value->name);
		if (c == 0)
		{
			variable * var = variable_new(term1->t_var.value->name);
		
			hash_insert(symbol_table, term1->t_var.value->name, var);
		}
	}
	else if (term1->type == TERM_TYPE_STRUCT)
	{
		term * iter;
		
		iter = term1->t_struct.terms->head;
		while (iter != NULL)
		{
			symbol_table_add_variable(symbol_table, iter);
			iter = iter->next;
		}
	}
}


