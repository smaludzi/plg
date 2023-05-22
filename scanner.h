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
#ifndef __SCANNER_H__
#define __SCANNER_H__ 1

#include "parser.h"
#include "var.h"
#include "expr.h"
#include "term.h"
#include "goal.h"
#include "clause.h"
#include "query.h"
#include "program.h"

typedef union token_value {
    char char_val;
    int int_val;
    char * string_val;
    var * var_val;
    var_list * vars_val;
    expr * expr_val;
    term * term_val;
    term_list * terms_val;
    goal * goal_val;
    goal_list * goals_val;
    clause * clause_val;
    clause_list * clauses_val;
    query * query_val;
    program * program_val;
} token_value;

typedef struct token {
    int type;
    token_value val; /* union */
	unsigned int line_no;
} token;

extern unsigned int line_no;

extern FILE * yyin;

extern int lex_scan(token * tokp);
extern int yylex_destroy();

extern const char * token_to_str(token * tokp);
extern void token_delete(token * tokp);

#define YYSTYPE token
#define YY_DECL int lex_scan(token * tokp)

#endif /* __SCANNER_H__ */

