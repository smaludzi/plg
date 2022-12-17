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
                             int type, const char * id, void * object_value)                             
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_str(id) % size;
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
    entries[index].object_value = object_value;
}

symtab_entry * symtab_entry_lookup_object(symtab_entry * entries,
                                          unsigned int size, const char * id)
{
    unsigned int times = 0;
    unsigned int index = 0;
    
    index = hash_str(id) % size;
    while (entries[index].type != 0)
    {
        if (strcmp(entries[index].id, id) == 0)
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
                                    entries[i].id, entries[i].object_value);
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
        case SYMTAB_VAR: return "var";
    }
    return "unknown";
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
    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_VAR, var_value->name,
                            var_value);
    tab->count++;
    symtab_resize(tab);
}

symtab_entry * symtab_lookup(symtab * tab, const char * id, symtab_lookup_op lookup)
{
    symtab_entry * entry = NULL;

    entry = symtab_entry_lookup_object(tab->entries, tab->size, id);
    if ((lookup == SYMTAB_LOOKUP_GLOBAL) &&
        (entry == NULL) && 
        (tab->parent != NULL))
    {
        return symtab_lookup(tab->parent, id, lookup);
    }
    else
    {
        return entry;
    }
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
