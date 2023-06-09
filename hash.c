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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define HASH_INIT_SIZE 5
#define HASH_MAGIC     5381
#define HASH_W_MARK    0.8

unsigned long hash_str(const char * str)
{
    int c;
    unsigned long hash = HASH_MAGIC;
    
    while ((c = *str++) != 0)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

int hash_get_watermark(hash * h)
{
	return 1.0 * h->size * HASH_W_MARK;
}

hash * hash_new()
{
	return hash_new_delete_size(NULL, HASH_INIT_SIZE);
}

hash * hash_new_size(int size)
{
	return hash_new_delete_size(NULL, size);
}

hash * hash_new_delete(hash_item_delete * item_delete_func)
{
	return hash_new_delete_size(item_delete_func, HASH_INIT_SIZE);
}

hash * hash_new_delete_size(hash_item_delete * item_delete_func, int size)
{
	hash * ret = NULL;
	
	ret = malloc(sizeof(hash));
	if (ret == NULL)
	{
		fprintf(stderr, "cannot allocate hash\n");
	}
	
    ret->item_delete_func = item_delete_func;
    ret->elems = 0;
    ret->size = size;
    ret->watermark = hash_get_watermark(ret);
    ret->items = calloc(ret->size, sizeof(hash_item));
    if (ret->items == NULL)
    {
    	fprintf(stderr, "cannot allocate hash items table\n");
    }
	
	return ret;
}

void hash_item_null_deallocator(hash_item * item)
{	
}

void hash_delete(hash * h)
{
	int i;

	if (h == NULL)
	{
		return;
	}
	
	if (h->items != NULL && h->item_delete_func != NULL)
	{
		for (i = 0; i < h->size; i++)
		{
			h->item_delete_func(h->items[i].key, h->items[i].value);
		}
	}
	
	if (h->items != NULL)
	{
		free(h->items);
	}
	free(h);
}

char hash_is_prime(int number)
{
	int d;
	
	for (d = 2; d * d <= number; d++)
	{
		if ((number % d) == 0)
		{
			return 0;
		}
	}
	
	return 1;
}

int hash_next_prime(int start)
{
	int s;
	
	for (s = start; ;s++)
	{
		if (hash_is_prime(s))
		{
			return s;
		}
	}
	
	return start;
}

int hash_inc_size(hash * h)
{
	return hash_next_prime(h->size * 2);
}

void hash_resize(hash * h)
{
	int i;
	int h_ns = hash_inc_size(h);
	
	hash_item * h_i = calloc(h_ns, sizeof(hash_item));
	
	for (i = 0; i < h->size; i++)
	{
		if (h->items[i].key != 0)
		{
			unsigned long hs = hash_str(h->items[i].key);
			while (h_i[hs % h_ns].key != 0)
			{
				hs++;
			}
			
			h_i[hs % h_ns].key = h->items[i].key;
			h_i[hs % h_ns].value = h->items[i].value;
		}
	}

	h->size = h_ns;
	h->watermark = hash_get_watermark(h);

	free(h->items);
	h->items = h_i;
}

void hash_insert(hash * h, const char * key, void * value)
{
	if (h->elems + 1 > h->watermark)
	{
		hash_resize(h);
	}
	
	unsigned long hs = hash_str(key);
	while (h->items[hs % h->size].key != 0)
	{
		hs++;
	}
	
	h->elems++;
	h->items[hs % h->size].key = key;
	h->items[hs % h->size].value = value;
}

void * hash_search(hash * h, const char * key)
{
	if (h == NULL || h->items == NULL)
	{
		return NULL;
	}
	
	unsigned long hs = hash_str(key);
	while (h->items[hs % h->size].key != 0)
	{
		if (strcmp(h->items[hs % h->size].key, key) == 0)
		{
			return h->items[hs % h->size].value;
		}
		hs++;
	}
	
	return NULL;
}

char hash_contains(hash * h, const char * key)
{
	if (h == NULL || h->items == NULL)
	{
		return 0;
	}
	
	unsigned long hs = hash_str(key);
	while (h->items[hs % h->size].key != 0)
	{
		if (strcmp(h->items[hs % h->size].key, key) == 0)
		{
			return 1;
		}
		hs++;
	}
	
	return 0;
}

void hash_remove(hash * h, const char * key)
{
	if (h == NULL || h->items == NULL)
	{
		return;
	}
	
	unsigned long hs = hash_str(key);
	while (h->items[hs & h->size].key != 0)
	{
		if (strcmp(h->items[hs % h->size].key, key) == 0)
		{
			if (h->item_delete_func != NULL)
			{
				h->item_delete_func(h->items[hs % h->size].key,
				                    h->items[hs % h->size].value);
			}
			
			h->items[hs % h->size].key = 0;
			h->items[hs % h->size].value = 0;
			h->elems--;
		
			return;
		}
	}
}

void hash_item_null_printer(void * value)
{
	printf("(data)");
}

void hash_print(hash * h)
{
	hash_printer(h, hash_item_null_printer);
}

void hash_printer(hash * h, hash_item_printer * printer)
{
	int i;
	int c = 0;
	
	printf("{ ");
	
	for (i = 0; i < h->size; i++)
	{
		if (h->items[i].key != 0)
		{
			if (c)
			{
				printf(", ");
			}
	
			c = 1;
		
			printf("%s", h->items[i].key); printf(" : "); printer(h->items[i].value);
		}
	}
	
	printf(" }\n");
}

