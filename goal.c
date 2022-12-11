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
#include <stdlib.h>
#include <assert.h>
#include "goal.h"

goal * goal_new_literal(char * name, List * terms)
{
    goal * value = malloc(sizeof(goal));

    value->type = GOAL_TYPE_LITERAL;
    value->literal.name = name;
    value->literal.terms = terms;

    return value;
}

goal * goal_new_unification(char * variable, term * term_value)
{
    goal * value = malloc(sizeof(goal));

    value->type = GOAL_TYPE_UNIFICATION;
    value->unification.variable = variable;
    value->unification.term_value = term_value;

    return value;
}

void goal_delete(goal * value)
{
    if (value == NULL)
    {
        return;
    }

    switch (value->type)
    {
        case GOAL_TYPE_LITERAL:
            free(value->literal.name);
            list_delete(value->literal.terms);
        break;
        case GOAL_TYPE_UNIFICATION:
            free(value->unification.variable);
            term_delete(value->unification.term_value);
        break;
        case GOAL_TYPE_UNKNOW:
            assert(0);
        break;
    }

    free(value);
}

void goal_deallocator(void * value)
{
    goal_delete((goal *)value);
}
