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
#ifndef __UNIFY_TERM_H__
#define __UNIFY_TERM_H__ 1

#include "term.h"
#include "unify.h"
#include "hash.h"

char terms_consistent(term * term1, term * term2);
int unify_terms(term * term1, term * term2);

u_system * create_system(term * term1, term * term2, hash * symbol_table);

multi_equation * create_multi_equation(term * term1, term * term2, hash * symbol_table);
multi_equation * create_multi_equation_variable(variable * var);

multi_term * create_multi_term(term * term1, term * term2, hash * symbol_table);
multi_term * create_multi_term_single(term * term1, hash * symbol_table);

temp_mult_eq * create_temp_mult_eq(term * term1, term * term2, hash * symbol_table);
temp_mult_eq * create_temp_mult_eq_single(term * term1, hash * symbol_table);

hash * create_symbol_table(term * term1, term * term2);
void symbol_table_add_variable(hash * symbol_table, term * term1);

#endif /* __UNIFY_TERM_H__ */

