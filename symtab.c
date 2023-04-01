/**
 * Copyright 2018 Slawomir Maludzinski
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
#include "symtab.h"
#include "var.h"
#include "clause.h"
#include "hash.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


symtab_entry * symtab_entry_new(unsigned int size)
{
    symtab_entry * entries =
        (symtab_entry *)malloc(size * sizeof(symtab_entry));
    memset(entries, 0, size * sizeof(symtab_entry));

    return entries;
}

void symtab_entry_delete(symtab_entry * entries) { free(entries); }

void symtab_entry_add_object(symtab_entry * entries, unsigned int size,
                             int type, const char * id, unsigned int arity, void * object_value)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = (hash_str(id) + 32 * arity) % size;
    while (entries[index].type != 0)
    {
        index = (index + 1) % size;
        if (times++ > size)
        {
            assert(0);
            return;
        }
    }
    entries[index].type = type;
    entries[index].id = id;
    entries[index].arity = arity;
    entries[index].object_value = object_value;
}

symtab_entry * symtab_entry_lookup_object(symtab_entry * entries,
                                          unsigned int size,
                                          const char * id,
                                          unsigned int arity)
{
    unsigned int times = 0;
    unsigned int index = 0;
    
    index = (hash_str(id) + 32 * arity) % size;
    while (entries[index].type != 0)
    {
        if (entries[index].arity == arity &&
            strcmp(entries[index].id, id) == 0)
        {
            return &entries[index];
        }

        index = (index + 1) % size;
        if (times++ > size)
        {
            return NULL;
        }
    }

    return NULL;
}

void symtab_entry_resize(symtab_entry * entries, int size,
                         symtab_entry * entries_new, int size_new)
{
    int i = 0;

    for (i = 0; i < size; i++)
    {
        if (entries[i].id != NULL)
        {
            symtab_entry_add_object(entries_new, size_new, entries[i].type,
                                    entries[i].id, entries[i].arity,
                                    entries[i].object_value);
        }
    }
}

void symtab_entry_print(symtab_entry * entry)
{
    if (entry->type == SYMTAB_VAR)
    {
        var_print((var *)entry->var_value);
    }
}

char * symtab_entry_type_str(symtab_entry_type type)
{
    switch (type)
    {
        case SYMTAB_UNKNOWN: return "SYMTAB_UNKNOWN";
        case SYMTAB_VAR: return "SYMTAB_VAR";
        case SYMTAB_CLAUSE: return "SYMTAB_CLAUSE";
    }
    return "SYMTAB_UNKNOWN";
}

symtab * symtab_new(unsigned int size, symtab * parent)
{
    symtab * tab = (symtab *)malloc(sizeof(symtab));
    
    tab->size = size;
    tab->count = 0;
    tab->parent = parent;
    tab->entries = symtab_entry_new(size);

    return tab;
}

void symtab_delete(symtab * tab)
{
    if (tab == NULL)
    {
        return;
    }
    if (tab->entries)
    {
        symtab_entry_delete(tab->entries);
    }
    free(tab);
}

void symtab_resize(symtab * tab)
{
    if (tab->count > tab->size * 3 / 4)
    {
        unsigned int size_new = 2 * tab->size;
        symtab_entry * entries_new = symtab_entry_new(size_new);

        symtab_entry_resize(tab->entries, tab->size, entries_new, size_new);
        symtab_entry_delete(tab->entries);

        tab->size = size_new;
        tab->entries = entries_new;
    }
}

void symtab_add_var(symtab * tab, var * var_value)
{
    if (var_value->name == NULL)
    {
        return;
    }
    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_VAR,
                            var_value->name, 0, var_value);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_predicate(symtab * tab, clause * clause_value)
{
    if (clause_value->name == NULL)
    {
        return;
    }
    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_CLAUSE,
                            clause_value->name, clause_arity(clause_value),
                            clause_value);
    tab->count++;
    symtab_resize(tab);
}

symtab_entry * symtab_lookup(symtab * tab, const char * id, symtab_lookup_op lookup)
{
    return symtab_lookup_arity(tab, id, 0, lookup);
}

symtab_entry * symtab_lookup_arity(symtab * tab, const char * id, unsigned int arity, symtab_lookup_op lookup)
{
    symtab_entry * entry = NULL;

    entry = symtab_entry_lookup_object(tab->entries, tab->size, id, arity);
    if ((lookup == SYMTAB_LOOKUP_GLOBAL) &&
        (entry == NULL) && 
        (tab->parent != NULL))
    {
        return symtab_lookup_arity(tab->parent, id, arity, lookup);
    }
    else
    {
        return entry;
    }
}

unsigned int symtab_size(symtab * tab)
{
    if (tab == NULL)
    {
        return 0;
    }
    return tab->size;
}

unsigned int symtab_size_type(symtab * tab, symtab_entry_type type)
{
    if (tab == NULL)
    {
        return 0;
    }
    unsigned int size = 0;
    unsigned int i;
    for (i = 0; i < tab->size; i++)
    {
        if (tab->entries[i].type == type)
        {
            size++;
        }
    }
    return size;
}

void symtab_print(symtab * tab)
{
    unsigned int i;

    printf("[%p, %p]\n", tab, tab->parent);

    for (i = 0; i < tab->size; i++)
    {
        symtab_entry_print(tab->entries + i);
    }
}
