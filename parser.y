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

int yyerror(char * str)
{
	fprintf(stderr, "%s\n", str);
	
	return 0;
}

int yylex(token * tokp)
{
	return lex_scan(tokp);
}

%}
%pure_parser

%token <val.string_val> TOK_ARR
%token <val.string_val> TOK_ATOM
%token <val.string_val> TOK_VAR
%token <val.string_val> TOK_IMPL
%token <val.string_val> TOK_QUERY

%type <val.term_val> term
%type <val.terms_val> terms

%start program

%%

term: TOK_ATOM
      {
          $$ = term_new(TERM_ATOM, $1);
      }
    | TOK_VAR
      {
          $$ = term_new(TERM_VAR, $1);
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
    | terms ',' term
;

goal: TOK_ATOM '(' ')' 
    | TOK_ATOM '(' terms ')'
    | TOK_VAR '=' term
;

goals: goal
    | goals ',' goal
;

clause: TOK_ATOM '(' ')' TOK_ARR goals
    | TOK_ATOM '(' terms ')' TOK_ARR goals
;

clauses: clause
    | clauses clause
;

query: TOK_ARR goals
;

queries: query
     | queries query
;

program: clauses queries
;

%%

