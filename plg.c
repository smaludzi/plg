/*
 * plg.c
 *
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
#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "program.h"
#include "semcheck.h"
#include "gencode.h"
#include "bytecode.h"
#include "strtab.h"

extern int yyparse(program ** program_value);

void scan()
{
	token tok;
    int tok_type;

	while ((tok_type = lex_scan(&tok)))
	{
		switch (tok_type)
		{
			case '.':
		    case ')':
		    case '(':
		    case ',':
		    case '=':
				printf("%d spec: %c\n", tok.line_no, tok.val.char_val);
				break;
			case TOK_IMPL:
				printf("%d :-\n", tok.line_no);
				break;
			case TOK_QUERY:
				printf("%d ?-\n", tok.line_no);
				break;
			case TOK_VAR:
			    printf("%d var: %s\n", tok.line_no, tok.val.string_val);
			    break;
			case TOK_ATOM:
				printf("%d atom: %s\n", tok.line_no, tok.val.string_val);
				break;
		}
	}
}

int main(int argc, char * argv[])
{
	if (argc > 1)
	{
		yyin = fopen(argv[1], "r");
	}
	else
	{
		yyin = stdin;
	}

	bytecode_print_test();

	program * program_value = NULL;

	yyparse(&program_value);

	if (program_value != NULL)
	{
		semcheck_result sem_res = SEMCHECK_SUCCESS;
		program_semcheck(program_value, &sem_res);
		program_print(program_value);
		if (sem_res == SEMCHECK_SUCCESS)
		{
			gencode * gen = gencode_new();
			gencode_result gen_res = GENCODE_SUCCESS;
			program_gencode(gen, program_value, &gen_res);

			bytecode_list_set_addr(gen->list);

			strtab_print(gen->strtab_value);
			bytecode_list_print(gen->list);

			gencode_delete(gen);
		}
		program_delete(program_value);
	}

	fclose(yyin);

	yylex_destroy();

	return 0;
}

