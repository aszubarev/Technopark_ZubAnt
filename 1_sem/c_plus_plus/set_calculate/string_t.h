#ifndef STRING_T_H
#define STRING_T_H

#define STRING_INIT_CAPACITY 16
#define STRING_RESIZE_FACTOR 2

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

typedef struct {

    char *Str;
    size_t Capacity;
    size_t Size;
    size_t Beg_ind;

} string_t;

string_t *create_string();
string_t *realoc_string(string_t *string);
void free_string(string_t *string);
bool push_back_string(string_t *string, char c);
size_t get_line_string(FILE* input, string_t *result);
bool is_empty_string(string_t *string);


#endif // STRING_T_H
