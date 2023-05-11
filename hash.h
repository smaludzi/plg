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
#ifndef __HASH_H__
#define __HASH_H__ 1

typedef struct hash_item {
    const char * key;
    void * value;
} hash_item;

typedef void hash_item_delete(const char * key, void * value);
typedef void hash_item_printer(void * value);

typedef struct hash {
    hash_item_delete * item_delete_func;
    hash_item * items;
    int size;
    int elems;
    int watermark;
} hash;

hash * hash_new();
hash * hash_new_size(int size);
hash * hash_new_delete(hash_item_delete * item_delete_func);
hash * hash_new_delete_size(hash_item_delete * item_delete_func, int size);

void hash_item_null_deallocator(hash_item * item);
void hash_delete(hash * h);

void hash_insert(hash * h, const char * key, void * value);
void * hash_search(hash * h, const char * key);
char hash_contains(hash * h, const char * key);
void hash_remove(hash * h, const char * key);

unsigned long hash_str(const char * str);

/** private **/
int hash_inc_size(hash * h);
void hash_resize(hash * h);
void hash_print(hash * h);
void hash_printer(hash * h, hash_item_printer * printer);

#endif /* __HASH_H__ */

