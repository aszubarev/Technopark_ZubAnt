#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFF_INIT_CAPACITY 16
#define STACK_RESIZE_FACTOR 2

#define STRING_INIT_CAPACITY 16
#define STRING_RESIZE_FACTOR 2

#define VECTOR_INIT_CAPACITY 16
#define VECTOR_RESIZE_FACTOR 2


typedef struct Buffer
{
    void **Data;
    size_t Size;
    size_t Capacity;

    size_t RightBor;
    size_t Tail;
    size_t Head;

} buff_t;

typedef struct{

    size_t open_bracket;            // Количество открывающих скобок
    size_t close_bracket;           // Количество закрывающих скобок

    bool last_is_open_bracket;      // Последний прочитанный символ был открывающей скобкой
    bool last_is_close_bracket;     // Последний прочитанный символ был закрывающей скобкой

    bool last_is_operator;          // Последний прочитанный символ был оператором
    bool last_is_set;               // Последний прочитанный символ был множеством

    bool prev_last_is_set;          // Элемент перед последним елемнотом являеться множеством { "set" U "operator" }

} counter_t;


typedef struct String{

    char *Str;
    size_t Capacity;
    size_t Size;
    size_t Beg_ind;

} string_t;


typedef struct Vector{
    long long int *Set;
    size_t Size;
    size_t Capacity;

    bool is_vector;
    char symb;

} vector_t;

///===============================================================================================================

buff_t *create_buff();
bool is_empty_buff(buff_t* self);

void push_back_buff(buff_t* self, void *data);
void push_front_buff(buff_t* self, void *data);

void* pop_front_buff(buff_t* self);
void* pop_back_buff(buff_t* self);

void* back_buff(buff_t* self);
void* front_buff(buff_t* self);
void* at_buff(buff_t* self, size_t ind);    /// НЕ ПРОВЕРИЛ

void realloc_buff(buff_t* self);
void free_buff(buff_t* self);

///===============================================================================================================

counter_t *create_counter();
void drop_flag_counter(counter_t *self);
void free_counter(counter_t *self);

///===============================================================================================================

bool val_in_right(long long int left, vector_t *right);
int comp(const void *left, const void *right);

vector_t * set_operation(vector_t * left, vector_t *right, char set_operator);
vector_t * set_union(vector_t* left, vector_t *right);
vector_t * set_intersection(vector_t* left, vector_t *right);
vector_t * set_diff(vector_t* left, vector_t *right);

///===============================================================================================================

string_t *create_string();
string_t *realoc_string(string_t *string);
void free_string(string_t *string);
bool push_back_string(string_t *string, char c);
size_t get_line_string(FILE* input, string_t *result);
bool is_empty_string(string_t *string);

///===============================================================================================================

bool set_vector_from_string(vector_t *set, string_t *string,
                            size_t ind_start, size_t ind_stop);

bool is_empty_vector(vector_t* self);
void push_back_vector(vector_t* self, long long element);
void* pop_back_vector(vector_t* self);

void realloc_vector(vector_t* self);
void free_vector(vector_t* self);
vector_t *copy_vector(vector_t *self);

void print_vector(vector_t* self);
void print_vector_clear(vector_t *self);

///===============================================================================================================
/*
 *  -->> Tail -->> Head -->>
 */

buff_t* create_buff()
{
    buff_t *stack = malloc(sizeof(buff_t));

    stack->Data = calloc(sizeof(void*) * BUFF_INIT_CAPACITY, sizeof(void*));
    stack->Size = 0;
    stack->Capacity = BUFF_INIT_CAPACITY;

    /// =================================
    stack->RightBor = stack->Capacity - 1;
    stack->Tail = 0;
    stack->Head = 0;
    /// =================================

    return stack;
}

bool is_empty_buff(buff_t* self)
{
    if (self->Size != 0) { return false; }
    return true;
}

void realloc_buff(buff_t *self)
{
    size_t new_capacity = self->Capacity * STACK_RESIZE_FACTOR;
    void **new_data = calloc((sizeof(void*)) * new_capacity, sizeof(void*));

    if (self->Head < self->Tail) {

        memcpy(new_data,
               self->Data + self->Tail,
               (self->Capacity - self->Tail) * sizeof(void*));

        memcpy(new_data + self->Capacity - self->Tail,
               self->Data,
               (self->Head + 1) * sizeof(void*));
    }
    else {
        memcpy(new_data,
               self->Data + self->Tail,
               (self->Head - self->Tail + 1) * sizeof(void*));
    }

    free(self->Data);
    self->Data = new_data;
    self->Capacity = new_capacity;
    self->RightBor = new_capacity - 1;
    self->Tail = 0;
    self->Head = self->Size - 1;

}

void push_back_buff(buff_t* self, void *data)
{
    if (self->Size == self->Capacity) { realloc_buff(self); }

    if (self->Head == self->RightBor && self->Size != 0) { self->Head = 0; }
    else if (self->Head != self->RightBor && self->Size != 0) { self->Head++; }
    else if (self->Size == 0) {
        self->Head = 0;
        self-> Tail = 0;
    }

    self->Data[self->Head] = data;
    self->Size++;

}

void free_buff(buff_t *self)
{

    if (self != NULL) {

        if (self->Size == 0) {
            free(self->Data);
            free(self);
            return;
        }

        if (self->Head < self->Tail) {

            for (size_t i = self->Tail; i <= self->RightBor; ++i) { free(self->Data[i]); }

            for (size_t i = 0; i <= self->Head; ++i) { free(self->Data[i]); }
        }
        else {

            for (size_t i = self->Tail; i <= self->Head; ++i) {
                free(self->Data[i]);
            }
        }

        free(self->Data);
    }

    free(self);
}

void *pop_front_buff(buff_t *self)
{
    if (self->Size == 0) { return NULL; }
    void *val = self->Data[self->Tail];
    self->Data[self->Tail] = NULL;

    if (self->Tail == self->RightBor && self->Head < self->Tail) { self->Tail = 0; }
    else if (self->Tail == self->Head) {
        self->Tail = 0;
        self->Head = 0;
    }
    else { self->Tail++; }
    self->Size--;

    return val;
}

void show_buff(buff_t *self)
{

    printf("SHOW_______________\n");

    printf("Tail = %zu; Head = %zu; Size = %zu; Capasity = %zu\n",
           self->Tail, self->Head, self->Size, self->Capacity);

    for(size_t i = 0; i < self->Capacity; ++i) {
        if (self->Data[i] != NULL) {
            printf("%d ", *(int*)(self->Data[i]));
        }
        else { printf("0 "); }

    }
    printf("\n");

    printf("END________________\n");
}

void push_front_buff(buff_t *self, void *data)
{
    if (self->Size == self->Capacity) { realloc_buff(self); }

    if (self->Tail == 0 && self->Size != 0) { self->Head = self->RightBor; }
    else if (self->Tail != 0 && self->Size != 0) { self->Tail--; }
    else if (self->Size == 0){
        self->Head = 0;
        self-> Tail = 0;
    }

    self->Data[self->Tail] = data;
    self->Size++;

}

void *pop_back_buff(buff_t *self)
{
    if (self->Size == 0) { return NULL; }

    void *val = self->Data[self->Head];
    self->Data[self->Head] = NULL;

    if (self->Head == 0 && self->Head < self->Tail) { self->Head = self->RightBor; }
    else if (self->Tail == self->Head) {
        self->Tail = 0;
        self->Head = 0;
    }
    else { self->Head--; }
    self->Size--;

    return val;
}

void *front_buff(buff_t *self)
{
    if (self->Size != 0) { return self->Data[self->Tail]; }
    return NULL;;
}

void *back_buff(buff_t *self)
{
    if (self->Size != 0) { return self->Data[self->Head]; }
    return NULL;
}

void *at_buff(buff_t *self, size_t ind)
{
    if (ind < self->Size && self->Size != 0) {

        if (self->Tail <= self->Head ) {
            return self->Data[self->Tail + ind];
        }
        else if (self->Tail > self->Head) {

            size_t right_size = self->Capacity - self->Tail;

            if (ind < right_size) {
                return self->Data[self->Tail + ind];
            }
            else{
                return self->Data[0 + ind - right_size];
            }
        }
    }

    return NULL;
}

///===============================================================================================================


counter_t *create_counter(){

    counter_t *cnt = (counter_t *)malloc(sizeof(counter_t));
    if (cnt == NULL) { return NULL; }

    cnt->open_bracket = 0;
    cnt->close_bracket = 0;

    drop_flag_counter(cnt);

    return cnt;
}

void free_counter(counter_t *self){ free(self); }

void drop_flag_counter(counter_t *self)
{
    self->last_is_open_bracket = false;
    self->last_is_close_bracket = false;

    self->last_is_operator = false;
    self->last_is_set = false;

    self->prev_last_is_set = false;
}

///===============================================================================================================


vector_t* create_vector_set()
{
    vector_t* vec = malloc(sizeof(vector_t));
    vec->Set = calloc(sizeof(long long ) * VECTOR_INIT_CAPACITY, sizeof(long long ));
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

void push_back_vector(vector_t* self, long long element)
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
    if (self != NULL) { free(self->Set); }
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

///===============================================================================================================


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

    if (string != NULL) { free(string->Str); }
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


///===============================================================================================================


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

int comp(const void *left, const void *right)
{
    return ( *(long long int*)left - *(long long int*)right );
}

vector_t * set_union(vector_t *left, vector_t *right)
{
    vector_t *new_vector = copy_vector(left);

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

///===============================================================================================================

void scan_infix_string(FILE* input, string_t *result)
{
    get_line_string(input, result);
}

bool is_char_of_set(char c)
{
    if (c >= '0' && c <= '9') { return true; }
    else if(c == ' ' || c == ',') { return true; }
    else if(c == '[' || c == ']') { return true; }

    return false;
}

bool lef_is_bigger_priority(char left, char right){

    if (left == 'U') {
        if (right == '\\') { return false; }
        if (right == '^') { return false; }
    }
    else if (left == '\\') {
        if (right == 'U') { return false; }
        if (right == '^') { return false; }
    }
    else if (left == '^') {
        if (right == 'U') { return true; }
        if (right == '\\') { return true; }
    }
    return false;
}

void free_buff_vector(buff_t *buff_vector){

    if (buff_vector != NULL)
    {
        if (buff_vector->Data != NULL)
        {
            size_t size = buff_vector->Capacity;
            for (size_t i = 0; i < size; ++i)
            {
                free_vector(buff_vector->Data[i]);
                buff_vector->Data[i] = NULL;
            }
            free(buff_vector->Data);
            buff_vector->Data = NULL;
        }
    }

    free(buff_vector);
}

void print_buff(buff_t *stack)
{
    size_t size = stack->Size;
    vector_t *v = NULL;

    for (size_t i = 0; i < size; ++i){
        v = stack->Data[i];
        if(v->is_vector == true) { print_vector_clear(v); }
        else {printf(" %c ", v->symb);}
    }
    printf("\n");
}

buff_t* get_postfix_buff(buff_t *infix_buff){

    if (infix_buff == NULL) { return NULL; }

    size_t size_infix_buff = infix_buff->Size;
    buff_t *postfix_buff = create_buff();
    buff_t *stack_operator = create_buff();

    counter_t *cnt = create_counter();

    for (size_t i = 0; i < size_infix_buff; ++i) {

        /// ===================================

        vector_t *data = infix_buff->Data[i];

        if (data->is_vector == true)
        {
            push_back_buff(postfix_buff, copy_vector(data));
        }
        else
        {
            char symb = data->symb;

            /// ==========================================================================
            if (symb == 'U' || symb == '\\' || symb == '^')
            {
                if (stack_operator->Size != 0)
                {
                    bool is_push_to_stack = false;

                    while (stack_operator->Size != 0)
                    {
                        vector_t *back_stack = back_buff(stack_operator);
                        char back_stack_symb = back_stack->symb;

                        if (back_stack_symb == '(')
                        {
                            push_back_buff(stack_operator, copy_vector(data));
                            is_push_to_stack = true;
                            break;
                        }

                        if (lef_is_bigger_priority(symb, back_stack_symb) == true)
                        {
                            push_back_buff(stack_operator, copy_vector(data));
                            is_push_to_stack = true;
                            break;
                        }
                        else {
                            push_back_buff(postfix_buff, pop_back_buff(stack_operator));
                        }
                    }
                    if (is_push_to_stack == false)
                    {
                        push_back_buff(stack_operator, copy_vector(data));
                    }
                }
                else { push_back_buff(stack_operator, copy_vector(data)); }

            }
            /// ==========================================================================
            else if (symb == '(')
            {
                cnt->open_bracket++;
                push_back_buff(stack_operator, copy_vector(data));
            }
            /// ==========================================================================
            else if (symb == ')')
            {
                cnt->close_bracket++;
                if (cnt->close_bracket > cnt->open_bracket ) {
                    free_counter(cnt);
                    free_buff_vector(postfix_buff);
                    free_buff_vector(stack_operator);
                    return NULL;
                }

                vector_t *back_stack = back_buff(stack_operator);
                char back_stack_symb = back_stack->symb;

                while (back_stack_symb != '(')
                {
                    push_back_buff(postfix_buff, pop_back_buff(stack_operator));
                    back_stack = back_buff(stack_operator);
                    back_stack_symb = back_stack->symb;
                }

                free_vector(pop_back_buff(stack_operator));
            }
            /// ==========================================================================
        }
    }

    while (stack_operator->Size != 0)
    {
        push_back_buff(postfix_buff, pop_back_buff(stack_operator));
    }

    if(cnt->close_bracket != cnt->close_bracket)
    {
        free_counter(cnt);
        free_buff_vector(postfix_buff);
        free_buff_vector(stack_operator);
        return NULL;
    }

    free_counter(cnt);
    free_buff(stack_operator);

    return postfix_buff;
}



vector_t* get_answer(buff_t *postfix_buff)
{
    vector_t *answer = NULL;
    buff_t *stack = create_buff();

    while (is_empty_buff(postfix_buff) == false)
    {
        vector_t *front_buff = pop_front_buff(postfix_buff);
        if (front_buff == NULL)
        {
            free_buff_vector(stack);
            return NULL;
        }
        if (front_buff->is_vector)
        {
            push_back_buff(stack, front_buff);
        }
        else
        {
            if (stack->Size < 2)
            {
                free_buff_vector(stack);
                return NULL;
            }

            vector_t *right_set = pop_back_buff(stack);
            vector_t *left_set = pop_back_buff(stack);

            if(right_set== NULL || left_set == NULL ||
                    right_set->is_vector == false || left_set->is_vector == false)
            {
                free_vector(right_set);
                free_vector(left_set);
                free_buff_vector(stack);
                return NULL;
            }

            vector_t *curr_operation = set_operation(left_set, right_set, front_buff->symb);

            if (is_empty_buff(postfix_buff) == true)
            {
                answer = curr_operation;
                free_vector(front_buff);
                free_vector(right_set);
                free_vector(left_set);
                break;
            }

            push_back_buff(stack, curr_operation);

            free_vector(front_buff);
            free_vector(right_set);
            free_vector(left_set);
        }
    }

    free_buff_vector(stack);

    return answer;
}


buff_t *get_infix_buff(string_t *infix_string){

    if (infix_string == NULL) { return NULL;}

    buff_t *infix_buff = create_buff();
    size_t size_infix_string = infix_string->Size;

    ///=============================================

    for (size_t i = 0; i < size_infix_string; ++i) {

        ///@ref Substing of set
        /// ==========================================================================================
        if (infix_string->Str[i] == '[') {

            size_t k = i;
            for (;; ++k) {

                if(k == size_infix_string){
                    free_buff_vector(infix_buff);
                    return NULL;
                }

                if(is_char_of_set(infix_string->Str[k]) == false) {
                    free_buff_vector(infix_buff);
                    return NULL;
                }
                else if (infix_string->Str[k] == ']') { break; }
            }
            vector_t *set = create_vector_set();

            set_vector_from_string(set, infix_string, i, k);
            push_back_buff(infix_buff, set);

            i = k;
        }
        ///@ref Push back Operator and brackets
        /// ==========================================================================================
        else if (infix_string->Str[i] == 'U') { push_back_buff(infix_buff, create_vector_char('U')); }
        /// ==========================================================================================
        else if (infix_string->Str[i] == '^') { push_back_buff(infix_buff, create_vector_char('^')); }
        /// ==========================================================================================
        else if (infix_string->Str[i] == '\\') { push_back_buff(infix_buff, create_vector_char('\\')); }
        /// ==========================================================================================
        else if (infix_string->Str[i] == '(') { push_back_buff(infix_buff, create_vector_char('(')); }
        /// ==========================================================================================
        else if (infix_string->Str[i] == ')') { push_back_buff(infix_buff, create_vector_char(')')); }
        /// ==========================================================================================
    }

    ///===============================================================================================

    return infix_buff;
}

bool is_valid_infix_buff(buff_t *infix_buff){

    if (infix_buff == NULL) { return false; }
    bool is_valid = true;

    counter_t *cnt = create_counter();
    size_t size = infix_buff->Size;
    buff_t *buff = infix_buff;

    for (size_t i = 0; i < size; ++i)
    {
        vector_t *data = buff->Data[i];

        if (data->is_vector)
        {
            ///@ref " [] [] " == BAD
            if (cnt->last_is_set == true)
            {
                is_valid = false;
                break;
            }
            ///@ref " ) [] " == BAD
            else if (cnt->last_is_close_bracket)
            {
                is_valid = false;
                break;
            }

            drop_flag_counter(cnt);
            cnt->last_is_set = true;
        }
        else
        {
            char symb = data->symb;

            /// ============================================
            if (symb == 'U' || symb == '\\' || symb == '^')
            {
                ///@ref " [] U " == GOOD
                if (cnt->last_is_set == true)
                {
                    drop_flag_counter(cnt);
                    cnt->last_is_operator = true;
                    cnt->prev_last_is_set = true;
                }
                ///@ref " ) U " == GOOD
                else if (cnt->last_is_close_bracket == true)
                {
                    drop_flag_counter(cnt);
                    cnt->last_is_operator = true;
                    cnt->prev_last_is_set = false;
                }
                ///@ref " ^ U " == BAD
                else if (cnt->last_is_operator == true)
                {
                    is_valid = false;
                    break;
                }
                ///@ref " ( U " == BAD
                else if (cnt->last_is_open_bracket == true)
                {
                    is_valid = false;
                    break;
                }
                else if (i == 0)
                {
                    is_valid = false;
                    break;
                }
            }
            /// ============================================
            else if (symb == '(')
            {
                ///@ref " U ( " == GOOD
                if (cnt->last_is_operator == true)
                {
                    drop_flag_counter(cnt);
                    cnt->open_bracket++;
                    cnt->last_is_open_bracket = true;
                }
                ///@ref " ( ( " == GOOD
                else if (cnt->last_is_open_bracket == true)
                {
                    drop_flag_counter(cnt);
                    cnt->open_bracket++;
                    cnt->last_is_open_bracket = true;
                }
                ///@ref " ) ( " == BAD
                else if (cnt->last_is_close_bracket == true)
                {
                    is_valid = false;
                    break;
                }
                ///@ref " [] ( " == BAD
                else if (cnt->last_is_set == true)
                {
                    is_valid = false;
                    break;
                }
                else if (i == 0)
                {
                    drop_flag_counter(cnt);
                    cnt->open_bracket++;
                    cnt->last_is_open_bracket = true;
                }
            }
            /// ============================================
            else if (symb == ')')
            {
                ///@ref " [] ) " == GOOD
                if (cnt->last_is_set == true)
                {
                    drop_flag_counter(cnt);
                    cnt->close_bracket++;
                    cnt->last_is_close_bracket = true;
                }
                ///@ref " ) ) " == GOOD
                else if (cnt->last_is_close_bracket == true)
                {
                    drop_flag_counter(cnt);
                    cnt->close_bracket++;
                    cnt->last_is_close_bracket = true;
                }
                ///@ref " ( ) " == BAD
                else if (cnt->last_is_open_bracket == true)
                {
                    is_valid = false;
                    break;
                }
                ///@ref " U ) " == BAD
                else if (cnt->last_is_operator == true)
                {
                    is_valid = false;
                    break;
                }
                ///@ref " start ) " == BAD
                else if (i == 0)
                {
                    is_valid = false;
                    break;
                }
            }
        }
    }

    if (is_valid == true && cnt->open_bracket != cnt->close_bracket)
    {
         free_counter(cnt);
         return false;
    }

    free_counter(cnt);

    return is_valid;

}

int main()
{
    string_t *infix_string = create_string();

    scan_infix_string(stdin, infix_string);             // write to infix string form stdin

    /// ==============================================================
    buff_t *infix_buff = get_infix_buff(infix_string);  // converrt infix stiring to infix buff with vector_t

    if (infix_buff == NULL) {
        printf("[error]");
        free_string(infix_string);
        free_buff_vector(infix_buff);
        return 0;
    }

    /// ==============================================================
    if (is_valid_infix_buff(infix_buff) == false)
    {
        printf("[error]");
        free_string(infix_string);
        free_buff_vector(infix_buff);
        return 0;
    }
    /// ==============================================================
    buff_t *postfix_buff = get_postfix_buff(infix_buff);

    if (postfix_buff == NULL)
    {
        printf("[error]");
        free_buff_vector(infix_buff);
        free_string(infix_string);
        return 0;
    }

    if (postfix_buff->Size == 1) {

        vector_t *answer = pop_back_buff(postfix_buff);
        if (answer->is_vector == true) { print_vector_clear(answer); }
        else { printf("[error]"); }

        free_vector(answer);
        free_buff_vector(postfix_buff);
        free_buff_vector(infix_buff);
        free_string(infix_string);
        return 0;
    }

    /// ==============================================================

//    printf("ANSWER:\n");
    vector_t *answer = get_answer(postfix_buff);

    if (answer == NULL) {
        printf("[error]");
        free_buff_vector(postfix_buff);
        free_buff_vector(infix_buff);
        free_string(infix_string);
        return 0;
    }

    print_vector_clear(answer);

    free_vector(answer);
    free_buff_vector(postfix_buff);
    free_buff_vector(infix_buff);
    free_string(infix_string);

    return 0;
}
