#include "var.h"

#include <stdlib.h>
#include <stdio.h>

var * var_new(char * name)
{
    var * value = (var *)malloc(sizeof(var));

    value->type = VAR_TYPE_UNKNOWN;
    value->name = name;
    value->bound_to = value;
    value->index = 0;
    value->line_no = 0;

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

var_node * var_node_new(var * value)
{
    var_node * node = (var_node *)malloc(sizeof(var_node));

    node->value = value;
    node->next = NULL;

    return node;
}

void var_node_delete(var_node * value)
{
    if (value->value)
    {
        var_delete(value->value);
    }
    free(value);
}

void var_node_delete_null(var_node * value)
{
    free(value);
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
    var_node * node = list->head;
    while (node != NULL)
    {
        var_node * next = node->next;
        var_node_delete(node);
        node = next;
    }
    free(list);
}

void var_list_delete_null(var_list * list)
{
    var_node * node = list->head;
    while (node != NULL)
    {
        var_node * next = node->next;
        var_node_delete_null(node);
        node = next;
    }
    free(list);
}

void var_list_add_end(var_list * list, var * value)
{
    var_node * node = var_node_new(value);
    *list->tail = node;
    list->tail = &node->next;
    list->size++;
}

unsigned int var_list_size(var_list * list)
{
    return list->size;
}

void var_list_print(var_list * list)
{
    var_node * node = list->head;
    while (node != NULL)
    {
        if (node->value)
        {
            var_print(node->value);
        }
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
