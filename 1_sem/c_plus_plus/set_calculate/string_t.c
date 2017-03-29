#include "string_t.h"
#include "string.h"

string_t *create_string(){

    string_t *string = (string_t *) malloc(sizeof(string_t));
    if (string == NULL) { return NULL; }

    string->Str = (char *) calloc(STRING_INIT_CAPACITY * sizeof(char) + 1,
                                  sizeof(char));
    if (string->Str == NULL) {
        free(string);
        return NULL;
    }

    string->Capacity = STRING_INIT_CAPACITY;
    string->Size = 0;
    string->Beg_ind = 0;

    return string;
}

string_t *realoc_string(string_t *string){

    size_t new_Capacity = (string->Capacity * STRING_RESIZE_FACTOR + 1);

    char *new_Str = calloc(new_Capacity, sizeof(char));
    if (new_Str == NULL) { return NULL; }

    memcpy(new_Str, string->Str, string->Capacity * sizeof(char));
    free(string->Str);

    string->Str = new_Str;
    string->Capacity = new_Capacity;

    return string;
}

bool push_back_string(string_t *string, char c){

    if (string == NULL) { return 0; }
    if (string->Size >= string->Capacity)
    {
        string_t *new_data = realoc_string(string);
        if (new_data == NULL) {
            free_string(string);
            return false;
        }
    }

    string->Str[string->Size++] = c;
    return true;
}

void free_string(string_t *string){

    free(string->Str);
    free(string);
}

char *get_cstr(string_t *string)
{
    if (string == NULL) { return 0; }
    return string->Str;
}

size_t get_line_string(FILE* input, string_t *result)
{
    if (input == NULL || result == NULL) { return 0; }

    char c = '\0';

    while (!feof(input)) {
        if (fscanf(input, "%c", &c) == 1 && c != '\n' && c != '\0') {
            push_back_string(result, c);
        }
    }

    return result->Size;
}

bool is_empty_string(string_t *string)
{
    if (string->Size !=0 ) { return false; }
    return true;
}
