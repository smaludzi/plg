/*
 * parser.y
 *
 * Copyright (C) 2017 S�awomir Maludzi�ski
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
%{
#include <stdio.h>
#include <stdarg.h>
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

%type <val.var_val> var
%type <val.vars_val> vars
%type <val.term_val> term
%type <val.terms_val> terms
%type <val.goal_val> goal
%type <val.goals_val> goals;
%type <val.clause_val> clause;
%type <val.clauses_val> clauses;
%type <val.query_val> query;
%type <val.program_val> program;

%destructor { if ($$) free($$); } TOK_ATOM
%destructor { if ($$) free($$); } TOK_VAR
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
         yyerror(NULL, "incorrect variable, found '%s' instead", yylval.val.string_val);
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

term: var
      {
          $$ = term_new_var(TERM_TYPE_VAR, $1);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM
      {
          $$ = term_new(TERM_TYPE_ATOM, $1);
          $$->line_no = $<line_no>1;
      }
    | TOK_ANON
      {
          $$ = term_new(TERM_TYPE_ANON, $1);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM '(' ')'
      {
      	  $$ = term_new(TERM_TYPE_ATOM, $1);
          $$->line_no = $<line_no>1;
      }
    | TOK_ATOM '(' terms ')'
      {
      	  $$ = term_new_list(TERM_TYPE_TERM, $1, $3);
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

%%
