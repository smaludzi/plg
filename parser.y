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
%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "scanner.h"
#include "unify_term.h"
#include "program.h"
#include "term.h"

int parse_result;

int yyerror(program ** plg, char * str, ...)
{
  va_list args;
  parse_result = 1;

  va_start(args, str);
  fprintf(stderr, "%d: ", line_no); vfprintf(stderr, str, args); fprintf(stderr, "\n");
  va_end(args);

	return 1;
}

int yylex(token * tokp)
{
  return lex_scan(tokp);
}

%}

%code requires
{
#include "program.h"
}

%define api.pure
%parse-param { program ** plg }

%token <val.string_val> TOK_ARR
%token <val.string_val> TOK_ATOM
%token <val.string_val> TOK_ANON
%token <val.string_val> TOK_VAR
%token <val.string_val> TOK_IMPL
%token <val.string_val> TOK_QUERY
%token <val.string_val> TOK_CUT
%token <val.string_val> TOK_FAIL
%token <val.string_val> TOK_IS
%token <val.int_val> TOK_INT

%type <val.var_val> var
%type <val.vars_val> vars
%type <val.expr_val> expr
%type <val.term_val> term
%type <val.terms_val> terms
%type <val.goal_val> goal
%type <val.goals_val> goals;
%type <val.clause_val> clause;
%type <val.clauses_val> clauses;
%type <val.query_val> query;
%type <val.program_val> program;

%left <val.string_val> '+' '-'
%left <val.string_val> '*' '/'
%right TOK_NOT /* %precedence NEG */
%left <val.string_val> '(' ')'

%destructor { if ($$) free($$); } TOK_ANON
%destructor { if ($$) free($$); } TOK_ATOM
%destructor { if ($$) free($$); } TOK_VAR
%destructor { if ($$) free($$); } TOK_FAIL
%destructor { if ($$) var_delete($$); } var
%destructor { if ($$) var_list_delete($$); } vars
%destructor { if ($$) expr_delete($$); } expr
%destructor { if ($$) term_delete($$); } term
%destructor { if ($$) term_list_delete($$); } terms
%destructor { if ($$) goal_delete($$); } goal
%destructor { if ($$) goal_list_delete($$); } goals
%destructor { if ($$) clause_delete($$); } clause
%destructor { if ($$) clause_list_delete($$); } clauses
%destructor { if ($$) query_delete($$); } query
%destructor { } program

%start program

%%

var: TOK_VAR
     {
         $$ = var_new($1);
         $$->line_no = $<line_no>1;
     }
     | error
     {
         $$ = NULL;
         yyerror(NULL, "incorrect variable");
         //yyerror(NULL, "incorrect variable, found '%s' instead", token_to_str(&yylval));
         //token_delete(&yylval);
         yyclearin;
     }
;

vars: var
     {
          $$ = var_list_new();
          var_list_add_end($$, $1);
     }
     | vars ',' var
     {
          var_list_add_end($1, $3);
          $$ = $1;
     }
;

expr: var
      {
          $$ = expr_new_var($1);
          $$->line_no = $<line_no>1;
      }
     | TOK_INT
      {
          $$ = expr_new_int($1);
          $$->line_no = $<line_no>1;
      }
     | '-' expr %prec TOK_NOT
      {
          $$ = expr_new_neg($2);
          $$->line_no = $<line_no>1;
      }
     | expr '+' expr
      {
          $$ = expr_new_add($1, $3);
          $$->line_no = $<line_no>2;
      }
     | expr '-' expr
      {
          $$ = expr_new_sub($1, $3);
          $$->line_no = $<line_no>2;
      }
     | expr '*' expr
      {
          $$ = expr_new_mul($1, $3);
          $$->line_no = $<line_no>2;
      }
     | expr '/' expr
      {
          $$ = expr_new_div($1, $3);
          $$->line_no = $<line_no>2;
      }
     | '(' expr ')'
      {
          $$ = expr_new_sup($2);
          $$->line_no = $<line_no>1;
      }
;

term: var
      {
          $$ = term_new_var(TERM_TYPE_VAR, $1);
          $$->line_no = $<line_no>1;
      }
    | TOK_INT
      {
          $$ = term_new_int(TERM_TYPE_INT, $1);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM
      {
          $$ = term_new_basic(TERM_TYPE_ATOM, $1);
          $$->line_no = $<line_no>1;
      }
    | TOK_ANON
      {
          $$ = term_new_basic(TERM_TYPE_ANON, $1);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM '(' ')'
      {
      	  $$ = term_new_struct(TERM_TYPE_ATOM, $1, NULL);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM '(' terms ')'
      {
      	  $$ = term_new_struct(TERM_TYPE_STRUCT, $1, $3);
          $$->line_no = $<line_no>1;
      }
    | '[' ']'
      {
          $$ = term_new_basic(TERM_TYPE_ATOM, strdup("[]"));
          $$->line_no = $<line_no>1;
      }
    | '[' terms ']'
      {
          term * tail = term_new_basic(TERM_TYPE_ATOM, strdup("[]"));
          $$ = term_new_list_constructor($2, tail);
          $$->line_no = $<line_no>1;
      }
    | '[' terms '|' term ']'
      {
          $$ = term_new_list_constructor($2, $4);
          $$->line_no = $<line_no>1;
      }
;

terms: term
      {
          $$ = term_list_new();
          term_list_add_end($$, $1);
      }
    | terms ',' term
      {
          term_list_add_end($1, $3);
          $$ = $1;
      }
;

goal: TOK_ATOM
      {
          $$ = goal_new_literal($1, NULL);
          $$->line_no = $<line_no>1;
      }
    |
      TOK_ATOM '(' ')'
      {
          $$ = goal_new_literal($1, NULL);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM '(' terms ')'
      {
          $$ = goal_new_literal($1, $3);
          $$->line_no = $<line_no>1;
      }
    | var '=' term
      {
          $$ = goal_new_unification($1, $3);
          $$->line_no = $<line_no>1;
      }
    | var TOK_IS expr
      {
          $$ = goal_new_is($1, $3);
          $$->line_no = $<line_no>1;
      }
    | TOK_CUT
      {
          $$ = goal_new_cut();
          $$->line_no = $<line_no>1;
      }
    | TOK_FAIL
      {
          $$ = goal_new_fail($1);
          $$->line_no = $<line_no>1;
      }
;

goals: goal
      {
          $$ = goal_list_new();
          goal_list_add_end($$, $1);
      }
    | goals ',' goal
      {
          goal_list_add_end($1, $3);
          $$ = $1;
      }
;

clause:  TOK_ATOM TOK_ARR goals
      {
          $$ = clause_new($1, NULL, $3);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM '(' ')' TOK_ARR goals
      {
          $$ = clause_new($1, NULL, $5);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM '(' vars ')' TOK_ARR goals
      {
          $$ = clause_new($1, $3, $6);
          $$->line_no = $<line_no>1;
      }
;

clauses: clause
      {
          $$ = clause_list_new();
          clause_list_add_end($$, $1);
      }
    | clauses clause
      {
          clause_list_add_end($1, $2);
          $$ = $1;
      }
;

query: TOK_ARR goals
      {
         $$ = query_new($2);
         $$->line_no = $<line_no>1;
      }
;

program: clauses query
      {
         $$ = *plg = program_new($1, $2);
      }
;

program: query
      {
        $$ = *plg = program_new(clause_list_new(), $1);
      }
;

%%
