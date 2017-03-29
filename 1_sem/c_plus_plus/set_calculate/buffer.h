#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include <stdbool.h>

#define BUFF_INIT_CAPACITY 16
#define STACK_RESIZE_FACTOR 2

typedef struct Buffer
{
    void **Data;
    size_t Size;
    size_t Capacity;

    size_t RightBor;
    size_t Tail;
    size_t Head;

} buff_t;

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

#endif
