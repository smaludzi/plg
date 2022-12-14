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
#include "scanner.h"
#include "unify_term.h"
#include "program.h"

int yyerror(program ** plg, char * str)
{
	fprintf(stderr, "%s\n", str);
	return 0;
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
%token <val.string_val> TOK_VAR
%token <val.string_val> TOK_IMPL
%token <val.string_val> TOK_QUERY

%type <val.term_val> term
%type <val.terms_val> terms
%type <val.goal_val> goal
%type <val.goals_val> goals;
%type <val.clause_val> clause;
%type <val.clauses_val> clauses;
%type <val.query_val> query;
%type <val.queries_val> queries;
%type <val.program_val> program;

%destructor { if ($$) free($$); } TOK_ATOM
%destructor { if ($$) free($$); } TOK_VAR
%destructor { if ($$) term_delete($$); } term
%destructor { if ($$) list_delete($$); } terms
%destructor { if ($$) goal_delete($$); } goal
%destructor { if ($$) list_delete($$); } goals
%destructor { if ($$) clause_delete($$); } clause
%destructor { if ($$) list_delete($$); } clauses
%destructor { if ($$) query_delete($$); } query
%destructor { if ($$) list_delete($$); } queries
%destructor { } program

%start program

%%

term: TOK_VAR
      {
          $$ = term_new(TERM_VAR, $1);
      }
    |
      TOK_ATOM
      {
          $$ = term_new(TERM_ATOM, $1);
      }      
    | TOK_ATOM '(' ')'
      {
      	  $$ = term_new(TERM_TERM, $1);
      }
    | TOK_ATOM '(' terms ')'
      {
      	  $$ = term_new_list(TERM_TERM, $1, $3); 
      }
;

terms: term
      {
      	    $$ = list_new();
      	    list_add_beg_deallocator($$, $1, term_deallocator);
      }
    | terms ',' term
      {
            list_add_beg_deallocator($1, $3, term_deallocator);
            $$ = $1;
      }
;

goal: TOK_ATOM '(' ')'
      {
          $$ = goal_new_literal($1, NULL);
      }
    | TOK_ATOM '(' terms ')'
      {
          $$ = goal_new_literal($1, $3);
      }
    | TOK_VAR '=' term
      {
          $$ = goal_new_unification($1, $3);
      }
;

goals: goal
      {
      	  $$ = list_new();
      	  list_add_beg_deallocator($$, $1, goal_deallocator);
      }
    | goals ',' goal
      {
      	  list_add_beg_deallocator($1, $3, goal_deallocator);
          $$ = $1;
      }
;

clause: TOK_ATOM '(' ')' TOK_ARR goals
      {
          $$ = clause_new($1, NULL, $5);
      }
    | TOK_ATOM '(' terms ')' TOK_ARR goals
      {
          $$ = clause_new($1, $3, $6);
      }
;

clauses: clause
      {
      	  $$ = list_new();
      	  list_add_beg_deallocator($$, $1, clause_deallocator);
      }
    | clauses clause
      {
      	  list_add_beg_deallocator($1, $2, clause_deallocator);
          $$ = $1;
      }
;

query: TOK_ARR goals
      {
         $$ = query_new($2);
      }
;

queries: query
      {
          $$ = list_new();
          list_add_beg_deallocator($$, $1, query_deallocator);
      }
     | queries query
     {
        list_add_beg_deallocator($1, $2, query_deallocator);
        $$ = $1;
     }
;

program: clauses query
      {
         $$ = *plg = program_new($1, $2);
      }
;

%%
