#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "vector.h"


vector_t* create_vector_set()
{
    vector_t* vec = malloc(sizeof(vector_t));
    vec->Set = calloc(sizeof(long long int) * VECTOR_INIT_CAPACITY, sizeof(long long int));
    vec->Size = 0;
    vec->Capacity = VECTOR_INIT_CAPACITY;

    vec->is_vector = true;
    vec->symb = '\0';
    return vec;
}

vector_t* create_vector_char(char _symb)
{
    vector_t* vec = malloc(sizeof(vector_t));

    vec->symb = _symb;

    vec->is_vector = false;
    vec->Set = NULL;
    vec->Size = 0;
    vec->Capacity = 0;

    return vec;
}

bool is_empty_vector(vector_t* self)
{
    if (self->Size != 0) { return false; }
    return true;
}

void push_back_vector(vector_t* self, long long int element)
{
    if(self->Size == self->Capacity){ realloc_vector(self); }
    self->Set[self->Size] = element;
    self->Size++;
}

void realloc_vector(vector_t* self)
{
    long long int *new_set = calloc((sizeof(long long int)) * self->Capacity * VECTOR_RESIZE_FACTOR,
                                     sizeof(long long int));
    memcpy(new_set, self->Set, self->Capacity * sizeof(long long int));
    free(self->Set);
    self->Set = new_set;
    self->Capacity *= VECTOR_RESIZE_FACTOR;
}

void free_vector(vector_t* self)
{
    if(self != NULL)
    {
        if (self->Set != NULL)
        {
            free(self->Set);
            self->Set = NULL;
        }
    }
    free(self);
}

bool set_vector_from_string(vector_t *set, string_t *string, size_t ind_start, size_t ind_stop)
{
    for(size_t i = ind_start; i < ind_stop; ++i){

        if(string->Str[i] >= '0' && string->Str[i] <= '9'){

            string_t *sub_str = create_string();
            size_t k = i;
            for(; k < ind_stop; k++){

                if(string->Str[k] >= '0' && string->Str[k] <= '9'){
                    push_back_string(sub_str, string->Str[k]);
                }
                else { break; }
            }
            long long int val = atoll(sub_str->Str);
            push_back_vector(set, val);
            i = k - 1;
            free_string(sub_str);
        }
    }

    return true;
}

void print_vector(vector_t *self)
{
    if (self->is_vector == false) { return ;}
    printf("Set: [");
    if (is_empty_vector(self) == false) {
        size_t i = 0;
        for (; i < self->Size - 1; ++i){
            printf("%lld, ", self->Set[i]);
        }
        printf("%lld", self->Set[i]);
    }

    printf("]\n");
}

void print_vector_clear(vector_t *self){

    if (self->is_vector == false) { return ;}
    printf("[");
    if (is_empty_vector(self) == false) {
        size_t i = 0;
        for (; i < self->Size - 1; ++i){
            printf("%lld,", self->Set[i]);
        }
        printf("%lld", self->Set[i]);
    }

    printf("]");
}

vector_t *copy_vector(vector_t *self)
{
    if (self == NULL) { return NULL; }

    vector_t* vec = malloc(sizeof(vector_t));
    vec->Set = calloc(sizeof(long long int) * self->Capacity, sizeof(long long int));
    memcpy(vec->Set, self->Set, sizeof(long long int) * self->Capacity);
    vec->Size = self->Size;
    vec->Capacity = self->Capacity;

    vec->is_vector = self->is_vector;
    vec->symb = self->symb;

    return vec;
}
