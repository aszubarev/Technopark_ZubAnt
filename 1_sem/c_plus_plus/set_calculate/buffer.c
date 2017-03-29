#include "buffer.h"
#include "string.h"
#include "stdio.h"

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
    printf("BILO %zu; OSTALOS %zu\n", self->Size + 1, self->Size);

    return val;
}

void show_buff(buff_t *self)
{

    printf("SHOW_______________\n");

    printf("Tail = %zu; Head = %zu; Size = %zu; Capasity = %zu\n",
           self->Tail, self->Head, self->Size, self->Capacity);

    for(size_t i = 0; i < self->Capacity; ++i) {
        if (self->Data[i] != NULL) {
            printf("%lld ", *(long long int*)(self->Data[i]));
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









