/** 
 * Copyright 2021 Slawomir Maludzinski
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
#include <stdlib.h>
#include <stdio.h>
#include "expr.h"
#include "var.h"

expr * expr_new_int(int int_value)
{
    expr * value = (expr *)malloc(sizeof(expr));

    value->type = EXPR_INT;
    value->int_t.value = int_value;
    value->line_no = 0;

    return value;
}

expr * expr_new_var(var * var_value)
{
    expr * value = (expr *)malloc(sizeof(expr));

    value->type = EXPR_VAR;
    value->var_t.value = var_value;
    value->line_no = 0;

    return value;
}

expr * expr_new_neg(expr * expr_value)
{
    expr * value = (expr *)malloc(sizeof(expr));

    value->type = EXPR_NEG;
    value->neg.expr_value = expr_value;
    value->line_no = 0;

    return value;
}

expr * expr_new_add(expr * left_value, expr * right_value)
{
    expr * value = (expr *)malloc(sizeof(expr));

    value->type = EXPR_ADD;
    value->add.left_value = left_value;
    value->add.right_value = right_value;
    value->line_no = 0;

    return value;
}

expr * expr_new_sub(expr * left_value, expr * right_value)
{
    expr * value = (expr *)malloc(sizeof(expr));

    value->type = EXPR_SUB;
    value->sub.left_value = left_value;
    value->sub.right_value = right_value;
    value->line_no = 0;

    return value;
}

expr * expr_new_mul(expr * left_value, expr * right_value)
{
    expr * value = (expr *)malloc(sizeof(expr));

    value->type = EXPR_MUL;
    value->mul.left_value = left_value;
    value->mul.right_value = right_value;
    value->line_no = 0;

    return value;
}

expr * expr_new_div(expr * left_value, expr * right_value)
{
    expr * value = (expr *)malloc(sizeof(expr));

    value->type = EXPR_DIV;
    value->div.left_value = left_value;
    value->div.right_value = right_value;
    value->line_no = 0;

    return value;
}

expr * expr_new_sup(expr * expr_value)
{
    expr * value = (expr *)malloc(sizeof(expr));

    value->type = EXPR_SUP;
    value->sup.expr_value = expr_value;
    value->line_no = 0;

    return value;
}

void expr_delete(expr * value)
{
    switch (value->type)
    {
      case EXPR_INT:
      break;
      case EXPR_VAR:
        if (value->var_t.value)
        {
            var_delete(value->var_t.value);
        }
      break;
      case EXPR_NEG:
      {
        if (value->neg.expr_value)
        {
            expr_delete(value->neg.expr_value);
        }
      }
      break;
      case EXPR_ADD:
      {
         if (value->add.left_value)
         {
            expr_delete(value->add.left_value);
         }
         if (value->add.right_value)
         {
            expr_delete(value->add.right_value);
         }
      }
      break;
      case EXPR_SUB:
      {
         if (value->sub.left_value)
         {
            expr_delete(value->sub.left_value);
         }
         if (value->sub.right_value)
         {
            expr_delete(value->sub.right_value);
         }
      }
      break;
      case EXPR_MUL:
      {
         if (value->mul.left_value)
         {
            expr_delete(value->mul.left_value);
         }
         if (value->mul.right_value)
         {
            expr_delete(value->mul.right_value);
         }
      }
      break;
      case EXPR_DIV:
      {
         if (value->div.left_value)
         {
            expr_delete(value->div.left_value);
         }
         if (value->div.right_value)
         {
            expr_delete(value->div.right_value);
         }
      }
      break;
      case EXPR_SUP:
      {
         if (value->sup.expr_value)
         {
            expr_delete(value->sup.expr_value);
         }
      }
      break;
    }
    free(value);
}

void expr_print(expr * value)
{
    switch (value->type)
    {
      case EXPR_INT:
        printf("%d", value->int_t.value);
      break;
      case EXPR_VAR:
        if (value->var_t.value)
        {
            var_print(value->var_t.value);
        }
      break;
      case EXPR_NEG:
      {
        printf("-");
        if (value->neg.expr_value)
        {
            expr_print(value->neg.expr_value);
        }
      }
      break;
      case EXPR_ADD:
      {
         if (value->add.left_value)
         {
            expr_print(value->add.left_value);
         }
         printf("+");
         if (value->add.right_value)
         {
            expr_print(value->add.right_value);
         }
      }
      break;
      case EXPR_SUB:
      {
         if (value->sub.left_value)
         {
            expr_print(value->sub.left_value);
         }
         printf("-");
         if (value->sub.right_value)
         {
            expr_print(value->sub.right_value);
         }
      }
      break;
      case EXPR_MUL:
      {
         if (value->mul.left_value)
         {
            expr_print(value->mul.left_value);
         }
         printf("*");
         if (value->mul.right_value)
         {
            expr_print(value->mul.right_value);
         }
      }
      break;
      case EXPR_DIV:
      {
         if (value->div.left_value)
         {
            expr_print(value->div.left_value);
         }
         printf("/");
         if (value->div.right_value)
         {
            expr_print(value->div.right_value);
         }
      }
      break;
      case EXPR_SUP:
      {
         if (value->sup.expr_value)
         {
            printf("(");
            expr_print(value->sup.expr_value);
            printf(")");
         }
      }
      break;
    }
    printf("\n");
}

const char * expr_type_to_str(expr_type type)
{
    switch (type)
    {
        case EXPR_INT: return "EXPR_INT";
        case EXPR_VAR: return "EXPR_VAR";
        case EXPR_NEG: return "EXPR_NEG";
        case EXPR_ADD: return "EXPR_ADD";
        case EXPR_SUB: return "EXPR_SUB";
        case EXPR_MUL: return "EXPR_MUL";
        case EXPR_DIV: return "EXPR_DIV";
        case EXPR_SUP: return "EXPR_SUP";
    }
    return "EXPR_UNKNOWN";
}
