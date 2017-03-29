#ifndef SET_OPERATOR_H
#define SET_OPERATOR_H

#include "vector.h"

bool val_in_right(long long int left, vector_t *right);
int comp(const void *left, const void *right);

vector_t * set_operation(vector_t * left, vector_t *right, char set_operator);
vector_t * set_union(vector_t* left, vector_t *right);
vector_t * set_intersection(vector_t* left, vector_t *right);
vector_t * set_diff(vector_t* left, vector_t *right);

#endif // SET_OPERATOR_H
