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
#include "object.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

object * object_new_atom(unsigned int idx)
{
    object * value = (object *)malloc(sizeof(object));

    value->type = OBJECT_ATOM;
    value->atom_value.idx = idx;

    return value;
}

object * object_new_anon()
{
    object * value = (object *)malloc(sizeof(object));

    value->type = OBJECT_ANON;
    value->anon_value.ref = 0;

    return value;
}

object * object_new_var()
{
    object * value = (object *)malloc(sizeof(object));

    value->type = OBJECT_VAR;
    value->var_value.ref = 0;

    return value;
}

object * object_new_ref(mem_ptr ptr_value)
{
    object * value = (object *)malloc(sizeof(object));

    value->type = OBJECT_REF;
    value->ref_value.ref = ptr_value;

    return value;
}

object * object_struct(unsigned int size)
{
    mem_ptr * refs = NULL;
    object * value = (object *)malloc(sizeof(object));

    if (size > 0)
    {
        refs = (mem_ptr *)malloc(sizeof(mem_ptr) * size);
    }

    value->type = OBJECT_STRUCT;
    value->struct_value.size = size;
    value->struct_value.refs = refs;

    return value;
}

void object_delete(object * value)
{
    switch (value->type)
    {
        case OBJECT_UNKNOWN:
            assert(0);
        break;
        case OBJECT_ATOM:
        break;
        case OBJECT_ANON:
        break;
        case OBJECT_VAR:
        break;
        case OBJECT_REF:
        break;
        case OBJECT_STRUCT:
        {
            if (value->struct_value.refs != NULL)
            {
                free(value->struct_value.refs);
            }
        }
        break;
    }
    free(value);
}

void object_print(object * value)
{
    switch (value->type)
    {
        case OBJECT_UNKNOWN:
            assert(0);
        break;
        case OBJECT_ATOM:
            printf("%s %u\n", object_type_str(value->type), value->atom_value.idx);
        break;
        case OBJECT_ANON:
            printf("%s %u\n", object_type_str(value->type), value->anon_value.ref);
        break;
        case OBJECT_VAR:
            printf("%s %u\n", object_type_str(value->type), value->var_value.ref);
        break;
        case OBJECT_REF:
            printf("%s %u\n", object_type_str(value->type), value->ref_value.ref);
        break;
        case OBJECT_STRUCT:
        {
            unsigned int i;
            printf("%s %u\n", object_type_str(value->type), value->struct_value.size);
            for (i = 0; i < value->struct_value.size; i++) {
                printf("%u ", value->struct_value.refs[i]);
            }
            if (value->struct_value.size > 0) printf("\n");
        }
        break;
    }
}

const char * object_type_str(object_type type)
{
    switch (type)
    {
        case OBJECT_UNKNOWN: return "OBJECT_UNKNOWN";
        case OBJECT_ATOM: return "OBJECT_ATOM";
        case OBJECT_ANON: return "OBEJCT_ANON";
        case OBJECT_VAR: return "OBJECT_VAR";
        case OBJECT_REF: return "OBJECT_REF";
        case OBJECT_STRUCT: return "OBJECT_STRUCT";
    }
    return "OBJECT_UNKNOWN";
}


