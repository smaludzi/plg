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
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "scanner.h"
#include "parser.h"
#include "program.h"
#include "semcheck.h"
#include "gencode.h"
#include "bytecode.h"
#include "strtab.h"
#include "vm.h"

extern int parse_result;
extern int yyparse(program ** program_value);

int main(int argc, char * argv[])
{
	if (argc > 1)
	{
		yyin = fopen(argv[1], "r");
		if (yyin == NULL)
		{
			fprintf(stderr, "Cannot open file %s: %s\n", argv[1], strerror(errno));
			return 1;
		}
	}
	else
	{
		yyin = stdin;
	}

	program * program_value = NULL;

	parse_result = 0;
	yyparse(&program_value);

	if (parse_result == 0 && program_value != NULL)
	{
		semcheck_result sem_res = SEMCHECK_SUCCESS;
		program_semcheck(program_value, &sem_res);
		if (sem_res == SEMCHECK_SUCCESS)
		{
			gencode * gen = gencode_new();
			gencode_result gen_res = GENCODE_SUCCESS;
			program_gencode(gen, program_value, &gen_res);
			if (gen_res == GENCODE_SUCCESS)
			{
				gencode_binary * binary_value = gencode_binary_new();
				gencode_binary_generate(binary_value, gen);

				//strtab_array_print(binary_value->strtab_array, binary_value->strtab_size);
				//bytecode_list_print(gen->list);

				vm * vm_value = vm_new(4096, 4096, 4096);
				vm_execute(vm_value, binary_value);
				vm_delete(vm_value);

				gencode_binary_delete(binary_value);
			}
			gencode_delete(gen);
		}
	}
	if (program_value != NULL)
	{
		program_delete(program_value);
	}

	fclose(yyin);

	yylex_destroy();

	return 0;
}

