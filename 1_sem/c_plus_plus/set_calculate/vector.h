#ifndef VECTOR_H
#define VECTOR_H

#ifndef VECTOR_INIT_CAPACITY
#define VECTOR_INIT_CAPACITY 16
#endif

#define VECTOR_RESIZE_FACTOR 2


#include <stdlib.h>
#include <stdbool.h>
#include "string_t.h"

typedef struct Vector
{
    long long int *Set;
    size_t Size;
    size_t Capacity;

    bool is_vector;
    char symb;

} vector_t;

vector_t* create_vector_set();
vector_t* create_vector_char(char _symb);

bool set_vector_from_string(vector_t *set, string_t *string,
                            size_t ind_start, size_t ind_stop);

bool is_empty_vector(vector_t* self);
void push_back_vector(vector_t* self, long long int element);
void* pop_back_vector(vector_t* self);

void realloc_vector(vector_t* self);
void free_vector(vector_t* self);
vector_t *copy_vector(vector_t *self);

void print_vector(vector_t* self);
void print_vector_clear(vector_t *self);

#endif
