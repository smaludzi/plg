/*
 * scanner.l
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
#include <string.h>
#include "scanner.h"

unsigned int line_no = 1;

extern void token_delete(token * tokp)
{
	switch (tokp->type)
	{
		case TOK_ANON:
		case TOK_VAR:
		case TOK_ATOM:
			free(tokp->val.string_val);
		break;
	}
}

const char * token_to_str(token * tokp)
{
	switch (tokp->type)
	{
		case TOK_ANON: return tokp->val.string_val;
		case TOK_ARR: return tokp->val.string_val;
		case TOK_IMPL: return tokp->val.string_val;
		case TOK_QUERY: return tokp->val.string_val;
		case TOK_VAR: return tokp->val.string_val;
		case TOK_ATOM: return tokp->val.string_val;
	}
	return "unknown";
}

%}

DIGIT     [0-9]
ID	      [a-zA-Z_]
SPECIAL   [.,()=]

%option pointer
%option noyylineno
%option noyywrap

%%

<INITIAL>{
    [ \t\r]+	/* remove white space */

    \n {
        line_no++;
    }
    
	"_" {
		tokp->type = TOK_ANON;
		tokp->line_no = line_no;
		tokp->val.string_val = (char *)strdup(yytext);
		return TOK_ANON;
	}

    "<=" {
		tokp->type = TOK_ARR;
		tokp->line_no = line_no;
		tokp->val.string_val = (char *)yytext;
		return TOK_ARR;
	}

    ":-" {
		tokp->type = TOK_IMPL;
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)yytext;
    	return TOK_IMPL;
    }
    
    "?-" {
		tokp->type = TOK_QUERY;
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)yytext;
    	return TOK_QUERY;
    }

	[A-Z]({ID}|{DIGIT})* {
		tokp->type = TOK_VAR;
		tokp->line_no = line_no;
		tokp->val.string_val = (char *)strdup(yytext);
		return TOK_VAR;
	}
	
    [a-z]({ID}|{DIGIT})*	{
		tokp->type = TOK_ATOM;
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)strdup(yytext);
		return TOK_ATOM;
	}
    
    {SPECIAL} {
    	tokp->line_no = line_no;
     	tokp->val.char_val = yytext[0];
        return yytext[0];
    }

	<<EOF>>	{
    	 yyterminate();
    	 return yytext[0];
	}
	
	. {
		printf("%d unknown char %s\n", line_no, yytext);
	}
}

%%



