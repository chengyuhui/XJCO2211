#pragma once
#include <stdlib.h>
#include <stdint.h>

typedef void (*vector_item_free_callback)(void *);

typedef struct
{
    void **ptr;
    size_t len;
    size_t cap;
    vector_item_free_callback free_cb;
} vector_t;

typedef vector_t* vector;

vector vector_new(vector_item_free_callback free_cb);
void vector_push(vector vec, void *item);
void vector_free(vector vec);
void *vector_get(vector vec, size_t idx);