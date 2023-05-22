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
		case TOK_FAIL:
			free(tokp->val.string_val);
		break;
	}
}

const char * token_to_str(token * tokp)
{
	switch (tokp->type)
	{
		case TOK_ANON: return tokp->val.string_val;
		case TOK_VAR: return tokp->val.string_val;
		case TOK_ATOM: return tokp->val.string_val;
	}
	return "unknown";
}

%}

DIGIT     [0-9]
HEX_DIGIT [0-9a-fA-F]
ID	      [a-zA-Z_]
SPECIAL   [.,()=|\[\]+\-*/]

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

    "!" {
		tokp->type = TOK_CUT;
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)yytext;
    	return TOK_CUT;
    }

	"is" {
		tokp->type = TOK_IS;
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)yytext;
    	return TOK_IS;
	}

	"fail" {
		tokp->type = TOK_FAIL;
		tokp->line_no = line_no;
		tokp->val.string_val = (char *)strdup(yytext);
		return TOK_FAIL;
	}

	[A-Z]({ID}|{DIGIT})* {
		tokp->type = TOK_VAR;
		tokp->line_no = line_no;
		tokp->val.string_val = (char *)strdup(yytext);
		return TOK_VAR;
	}
	
    [a-z]({ID}|{DIGIT})* {
		tokp->type = TOK_ATOM;
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)strdup(yytext);
		return TOK_ATOM;
	}

	{DIGIT}+ {
		int result;
		tokp->line_no = line_no;
		sscanf(yytext, "%d", &result);
		tokp->val.int_val = result;
		return TOK_INT;
	}

	0x{HEX_DIGIT}+|0X{HEX_DIGIT}+ {
		int result;
		tokp->line_no = line_no;
		sscanf(yytext, "%x", &result);
		tokp->val.int_val = result;
		return TOK_INT;
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



