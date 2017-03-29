#include "set_operator.h"
#include "string.h"
#include "stdlib.h"

int comp(const void *left, const void *right)
{
    return ( *(long long int*)left - *(long long int*)right );
}


vector_t * set_operation(vector_t * left, vector_t *right, char set_operator)
{
    switch (set_operator) {
    case 'U':
        return set_union(left, right);
        break;
    case '^':
        return set_intersection(left, right);
        break;
    case '\\':
        return set_diff(left, right);
        break;

    default:
        break;
    }

    return NULL;
}

vector_t * set_union(vector_t *left, vector_t *right)
{
    vector_t *new_vector = copy_vector(left);
//    vector_t *new_vector = create_vector_set();
//    memcpy(new_vector->Set, left->Set, left->Size * sizeof(long long int));
//    new_vector->Size = left->Size;

    size_t size = right->Size;
    for (size_t i = 0; i < size; ++i) {
        if (val_in_right(right->Set[i], left) == false) {
            push_back_vector(new_vector, right->Set[i]);
        }
    }

    qsort(new_vector->Set, new_vector->Size, sizeof(long long int), comp);

    return new_vector;
}

vector_t * set_intersection(vector_t *left, vector_t *right)
{
    vector_t *new_vector = create_vector_set();
    size_t left_size = left->Size;
    for (size_t i = 0; i < left_size; ++i) {
        if (val_in_right(left->Set[i], right) == true) {
            push_back_vector(new_vector, left->Set[i]);
        }
    }

    return new_vector;
}

vector_t * set_diff(vector_t *left, vector_t *right)
{
    vector_t *new_vector = create_vector_set();
    size_t left_size = left->Size;
    for (size_t i = 0; i < left_size; ++i) {
        if (val_in_right(left->Set[i], right) == false) {
            push_back_vector(new_vector, left->Set[i]);
        }
    }

    return new_vector;
}

bool val_in_right(long long int val, vector_t *right)
{
    size_t size = right->Size;
    bool in = false;
    for (size_t i = 0; i < size; ++i){
        if (val == right->Set[i]) { return true; }
    }

    return in;
}


