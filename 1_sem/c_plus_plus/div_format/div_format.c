#include "stdlib.h"
#include "stdbool.h"
#include <stdio.h>
#include <string.h>

#define CAPACITY 8
#define RESIZE_FACTOR 2

typedef struct {

    char **Str;
    size_t Capacity;
    size_t Size;

} vector_string_t;

typedef struct {

    char *Str;
    size_t Capacity;
    size_t Size;
    size_t Beg_ind;

} string_t;

typedef struct{

    size_t level;           // Счетчик табуляций/сдвигов
    size_t offset;          // Размер одной табуляции/сдвига
    size_t open_tag;        // Количество открывающих тэгов
    size_t close_tag;       // Количество закрывающих тэгов

} counter_t;


vector_string_t *create_vector(){

    vector_string_t *vector = (vector_string_t *)malloc(sizeof(vector_string_t));
    if (vector == NULL) { return NULL; }

    vector->Str = (char **) calloc(CAPACITY * sizeof(char*), sizeof(char*));
    if (vector->Str == NULL) {
        free(vector);
        return NULL;
    }
    vector->Capacity = CAPACITY;
    vector->Size = 0;

    return vector;
}


string_t *create_string(){

    string_t *string = (string_t *)malloc(sizeof(string_t));
    if (string == NULL) { return NULL; }

    string->Str = (char *) calloc(CAPACITY * sizeof(char), sizeof(char));
    if (string->Str == NULL) {
        free(string);
        return NULL;
    }

    string->Capacity = CAPACITY;
    string->Size = 0;
    string->Beg_ind = 0;

    return string;
}


counter_t *create_counter(){

    counter_t *cnt = (counter_t *)malloc(sizeof(counter_t));
    if (cnt == NULL) { return NULL; }

    cnt->level = 0;
    cnt->offset = 4;
    cnt->open_tag = 0;
    cnt->close_tag = 0;
    return cnt;
}

size_t get_numb_space(const counter_t *cnt){

    return cnt->level * cnt->offset;    // number_space = level * offset
}


vector_string_t *realoc_vector(vector_string_t *vector){

    size_t new_Capacity = (vector->Capacity * RESIZE_FACTOR);

    char **new_Str = calloc(new_Capacity, sizeof(char*));
    if (new_Str == NULL) { return NULL; }

    memcpy(new_Str, vector->Str, vector->Capacity * sizeof(char*));
    free(vector->Str);

    vector->Str = new_Str;
    vector->Capacity = new_Capacity;

    return vector;
}


string_t *realoc_string(string_t *strign){

    size_t new_Capacity = (strign->Capacity * RESIZE_FACTOR);

    char *new_Str = calloc(new_Capacity, sizeof(char));
    if (new_Str == NULL) { return NULL; }

    memcpy(new_Str, strign->Str, strign->Capacity * sizeof(char));
    free(strign->Str);

    strign->Str = new_Str;
    strign->Capacity = new_Capacity;

    return strign;
}


void free_vector(vector_string_t *vector){
    for (size_t i = 0; i < vector->Size; ++i) {
        free(vector->Str[i]);
    }
    free(vector->Str);
    free(vector);
}


void free_string(string_t *string){

    free(string->Str);
    free(string);
}


void free_counter(counter_t *cnt){
    free(cnt);
}


void free_all(vector_string_t *vector, string_t *string, counter_t *cnt){
    free_vector(vector);
    free_string(string);
    free_counter(cnt);
}


bool push_back_vector(vector_string_t *vector, char* str){

    if (vector == NULL) { return 0; }
    if (vector->Size >= vector->Capacity)
    {
        vector_string_t *new_vector = realoc_vector(vector);
        if (new_vector == NULL) {
            free_vector(vector);
            return false;
        }
    }

    if (str == NULL) {
        str = (char*) malloc(sizeof(char));
        if (str == NULL) {
            return false;
        }
        *str = '\0';
    }

    vector->Str[vector->Size++] = str;
    return true;
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

size_t my_getline(FILE* input, char** result) {

    if (input == NULL || result == NULL) {
        return 0;
    }

    size_t capacity = 1;
    size_t size = 0;

    char* str = (char*) calloc(capacity, sizeof(char));
    if (str == NULL) {
        *result = NULL;
        return 0;
    }

    char c = '\0';

    while (fscanf(input, "%c", &c) == 1 && c != '\n') {
        if (++size == capacity) {
            char* new_str = (char*) realloc(str, sizeof(char) * ((capacity *= RESIZE_FACTOR) + 1));
            if (new_str == NULL) {
                free(str);
                *result = NULL;
                return 0;
            }
            str = new_str;
        }
        str[size - 1] = c;
    }

    str[size] = '\0';
    *result = str;
    return size;;
}


vector_string_t* scan_data(FILE* in){

    vector_string_t* data = create_vector();

    if (data != NULL) {

        while (!feof(in)) {

            char* str;
            my_getline(in, &str);
            push_back_vector(data, str);
        }
    }
    return data;
}


void print_vector(FILE* out, vector_string_t *vector) {
    if (out == NULL || vector == NULL) return;

    for (size_t i = 0; i < vector->Size; ++i) {
        fprintf(out, "%s\n", vector->Str[i]);
    }
}


bool cmp_str_begin(const char* a, const char* b) {

    size_t a_len = strlen(a);
    size_t b_len = strlen(b);

    if (a_len < b_len) { return false; }

    for (size_t i = 0; i < b_len; ++i) {

        if (a[i] != b[i]) { return false; }
    }
    return true;
}


bool is_open_tag(const char* s) {

    return cmp_str_begin(s, "<div>");
}


bool is_close_tag(const char* s) {

    return cmp_str_begin(s, "</div>");
}


bool push_format_str(vector_string_t *vector, counter_t *cnt, const char *str){

    size_t numb_space = get_numb_space(cnt);
    size_t len_tag = strlen(str);
    size_t size_st = numb_space + len_tag + 1;

    char *new_str = (char*) calloc(size_st * sizeof(char), sizeof(char));
    if(str == NULL) { return false; }

    memset(new_str, ' ', numb_space);
    memcpy(new_str + numb_space, str, len_tag);

    if (push_back_vector(vector, new_str) == false) { return false; }

    return true;
}


char *skip_space(string_t *data, size_t curr){

    if(curr == 0) { return NULL; }
    size_t beg = data->Beg_ind;
    size_t end = curr - 1;

    if (beg > end) { return NULL; }

    for (; beg <  end && (data->Str[beg] == ' ' || data->Str[beg] == '\0'); ++beg) {}
    for (; end >= beg && (data->Str[beg] == ' ' || data->Str[beg] == '\0'); --end) {}

    if(beg > end) { return NULL; }

    size_t size_str = end - beg + 1;

    char *str = (char*)calloc(size_str * sizeof(char) + 1, sizeof(char));
    if(str == NULL) { return NULL; }

    memcpy(str, data->Str + beg, size_str);

    return str;
}


vector_string_t* div_format(vector_string_t *input) {

    vector_string_t *new_vector = create_vector();
    counter_t *cnt = create_counter();

    for (size_t i = 0; i < input->Size; ++i) {

        char *str = input->Str[i];
        size_t size = strlen(str);
        string_t *new_str = create_string();

        //--------------------------------------------------------------------------

        for (size_t k = 0; k < size; ++k){

            if (is_open_tag(str + k)) {

                char *left_str = skip_space(new_str, k);
                if (left_str != NULL) {
                    if (push_format_str(new_vector, cnt, left_str) == false) {
                        free(left_str);
                        free_all(new_vector, new_str, cnt);
                        return NULL;
                    }
                    free(left_str);
                }

                if (push_format_str(new_vector, cnt, "<div>") == false) {
                    free_all(new_vector, new_str, cnt);
                    return NULL;
                }
                new_str->Beg_ind = k + strlen("<div>");
                cnt->level++;
                cnt->open_tag++;
            }
            else if (is_close_tag(str + k)) {

                if (cnt->close_tag == cnt->open_tag) {
                    free_all(new_vector, new_str, cnt);
                    return NULL;
                }
                char *left_str = skip_space(new_str, k);
                if(left_str != NULL) {
                    if (push_format_str(new_vector, cnt, left_str) == false) {
                        free(left_str);
                        free_all(new_vector, new_str, cnt);
                        return NULL;
                    }
                    free(left_str);
                }

                cnt->level--;
                if (push_format_str(new_vector, cnt, "</div>") == false) {
                    free_all(new_vector, new_str, cnt);
                    return NULL;
                }
                new_str->Beg_ind = k + strlen("</div>");
                cnt->close_tag++;
            }

            if(push_back_string(new_str, str[k]) == false){
                free_all(new_vector, new_str, cnt);
                return NULL;
            }
        }

        char *left_str = skip_space(new_str, size);
        if(left_str != NULL) {
            if (push_format_str(new_vector, cnt, left_str) == false) {
                free(left_str);
                free_all(new_vector, new_str, cnt);
                return NULL;
            }
            free(left_str);
        }

        //--------------------------------------------------------------------------

        free_string(new_str);
    }

    if(cnt->open_tag != cnt->close_tag){
        free_counter(cnt);
        free_vector(new_vector);
        return NULL;
    }

    free_counter(cnt);

    return new_vector;
}

int main()
{
    vector_string_t *input = scan_data(stdin);
    vector_string_t *output = div_format(input);

    if(output == NULL) {
        printf("[error]");
        free_vector(input);
        return 0;
    }
    else { print_vector(stdout, output); }

    free_vector(output);
    free_vector(input);

    return 0;
}
