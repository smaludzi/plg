#include "var.h"

#include <stdlib.h>
#include <stdio.h>

var * var_new(char * name)
{
    var * value = (var *)malloc(sizeof(var));

    value->type = VAR_TYPE_UNKNOWN;
    value->name = name;
    value->bound_to = NULL;
    value->index = 0;
    value->line_no = 0;

    return value;
}
void var_delete(var * value)
{
    if (value == NULL)
    {
        return;
    }
    free(value->name);
    free(value);
}

void var_deallocator(void * value)
{
    var_delete((var *)value);
}

void var_null_deallocator(void * value)
{
    return;
}

void var_print(var * value)
{
    printf("var name: %s\n", value->name);
    printf("var bound: %s\n", var_type_to_str(value->type));
    if (value->bound_to != NULL) {
        printf("var bound index: %u\n", value->bound_to->index);
    }
}

void var_list_print(List * list)
{
    ListIterator iter = list_iterator_first(list);
    while (!list_iterator_is_last(iter))
    {
        var_print((var *)list_iterator_data(iter));
        list_iterator_next(&iter);
    }   
}

char * var_type_to_str(var_type type)
{
    switch (type)
    {
        case VAR_TYPE_UNKNOWN: return "VAR_TYPE_UNKNOWN";
        case VAR_TYPE_BOUND: return "VAR_TYPE_BOUND";
        case VAR_TYPE_UNBOUND: return "VAR_TYPE_UNBOUND";
    }
    return "VAR_TYPE_UNKNOWN";
}
