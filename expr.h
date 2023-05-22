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
#ifndef __EXPR_H__
#define __EXPR_H__

typedef struct var var;

typedef enum expr_type
{
    EXPR_INT,
    EXPR_VAR,
    EXPR_NEG,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_SUP
} expr_type;

typedef struct expr
{
    expr_type type;
    union {
        struct {
            int value;
        } int_t;
        struct {
            var * value;
        } var_t;
        struct {
            struct expr * expr_value;
        } neg;
        struct {
            struct expr * left_value;
            struct expr * right_value;
        } add;
        struct {
            struct expr * left_value;
            struct expr * right_value;
        } sub;
        struct {
            struct expr * left_value;
            struct expr * right_value;
        } mul;
        struct {
            struct expr * left_value;
            struct expr * right_value;
        } div;
        struct {
            struct expr * expr_value;
        } sup;
    };
    unsigned int line_no;
} expr;

expr * expr_new_int(int int_value);
expr * expr_new_var(var * var_value);
expr * expr_new_neg(expr * expr_value);
expr * expr_new_add(expr * left_value, expr * right_value);
expr * expr_new_sub(expr * left_value, expr * right_value);
expr * expr_new_mul(expr * left_value, expr * right_value);
expr * expr_new_div(expr * left_value, expr * right_value);
expr * expr_new_sup(expr * expr_value);

void expr_delete(expr * expr_value);

void expr_print(expr * expr_value);
const char * expr_type_to_str(expr_type type);

#endif /* __EXPR_H__ */
