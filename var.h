#ifndef __VAR_H__
#define __VAR_H__

#include "list.h"

typedef enum var_type {
    VAR_TYPE_UNKNOWN = 0,
    VAR_TYPE_BOUND = 1,
    VAR_TYPE_UNBOUND = 2
} var_type;

typedef struct var
{
    var_type type;
    char * name;
    struct var * bound_to;
    unsigned int index;
    unsigned int line_no;
} var;

var * var_new(char * name);
void var_delete(var * value);
void var_deallocator(void * value);
void var_null_deallocator(void * value);

void var_print(var * value);
void var_list_print(List * list);

char * var_type_to_str(var_type type);

#endif /* __VAR_H__ */

