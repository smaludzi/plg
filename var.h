#ifndef __VAR_H__
#define __VAR_H__

#include "list.h"

typedef struct var
{
    char * name;
    unsigned int line_no;
} var;

var * var_new(char * name);
void var_delete(var * value);
void var_deallocator(void * value);

void var_print(var * value);
void var_list_print(List * list);

#endif /* __VAR_H__ */

