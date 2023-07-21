#include "vector.h"
#include <assert.h>

vector vector_new(vector_item_free_callback free_cb)
{
    vector vec = (vector)malloc(sizeof(vector_t));

    vec->ptr = (void **)malloc(sizeof(void *));
    vec->len = 0;
    vec->cap = 1;
    vec->free_cb = free_cb;

    return vec;
}

void vector_push(vector vec, void *item)
{
    if (vec->len == vec->cap)
    {
        size_t new_cap = vec->cap * 2;
        vec->ptr = (void **)realloc(vec->ptr, new_cap * sizeof(void *));
        vec->cap = new_cap;
    }

    vec->ptr[vec->len] = item;
    vec->len++;
}

void vector_free(vector vec)
{
    for (size_t i = 0; i < vec->len; i++)
    {
        if (vec->free_cb == NULL)
        {
            free(vec->ptr[i]);
        }
        else
        {
            vec->free_cb(vec->ptr[i]);
        }
    }
    free(vec);
}

void* vector_get(vector vec, size_t idx) {
    assert(idx < vec->len);
    return vec->ptr[idx];
}