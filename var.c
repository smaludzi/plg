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
    value->next = NULL;

    return value;
}
void var_delete(var * value)
{
    if (value->name)
    {
        free(value->name);
    }
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

var_list * var_list_new()
{
    var_list * list = (var_list *)malloc(sizeof(var_list));

    list->head = NULL;
    list->tail = &list->head;
    list->size = 0;

    return list;
}

void var_list_delete(var_list * list)
{
    var * node = list->head;
    while (node != NULL)
    {
        var * next = node->next;
        var_delete(node);
        node = next;
    }
    free(list);
}

void var_list_delete_null(var_list * list)
{
    free(list);
}

void var_list_add_end(var_list * list, var * value)
{
    *list->tail = value;
    list->tail = &value->next;
    list->size++;
}

unsigned int var_list_size(var_list * list)
{
    return list->size;
}

void var_list_print(var_list * list)
{
    var * node = list->head;
    while (node != NULL)
    {
        var_print(node);
        node = node->next;
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
