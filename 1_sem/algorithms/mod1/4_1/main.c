#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_INIT_CAPACITY 2
#define STACK_RESIZE_FACTOR 2

/*
 *  -->> Tail -->> Head -->>
 */

typedef struct Buffer
{
    void **Data;
    size_t Size;
    size_t Capacity;

    size_t RightBor;
    size_t Tail;
    size_t Head;

} buff_t;


buff_t* create_buff()
{
    buff_t *stack = malloc(sizeof(buff_t));

    stack->Data = calloc(sizeof(void*) * STACK_INIT_CAPACITY, sizeof(void*));
    stack->Size = 0;
    stack->Capacity = STACK_INIT_CAPACITY;

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
//        printf("memcpy RIGHT\n");

        memcpy(new_data + self->Capacity - self->Tail,
               self->Data,
               (self->Head + 1) * sizeof(void*));
//        printf("memcpy LEFT\n");

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

//    printf("FREE BUFFER\n");
    if (self->Size == 0) {
        free(self->Data);
        free(self);
        return;
    }

    if (self->Head < self->Tail) {

        for (size_t i = self->Tail; i <= self->RightBor; ++i) {
            free(self->Data[i]);
//            printf("FREE: 1; LINE = %d\n", __LINE__);
        }

        for (size_t i = 0; i <= self->Head; ++i) {
            free(self->Data[i]);
//            printf("FREE: 1; LINE = %d\n", __LINE__);
        }
    }
    else {

        for (size_t i = self->Tail; i <= self->Head; ++i) {
            free(self->Data[i]);
//            printf("FREE: 1; LINE = %d\n", __LINE__);
        }
    }

//    printf("FREE: 2;  free(self->Data);  free(self);\n");
    free(self->Data);
    free(self);
}

void *back_buff(buff_t *self)
{
    if (self->Size != 0) { return self->Data[self->Size - 1]; }
    return NULL;
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
//    else if (self->Tail == self->RightBor && self->Tail == self->Head) {}
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
//            printf("%zu_", i);
            printf("%d ", *(int*)(self->Data[i]));
        }
        else { printf("0 "); }

    }
    printf("\n");

    printf("END________________\n");
}

int main()
{
    size_t n = 0;
    buff_t *buff = create_buff();
    bool check = true;
    scanf("%zu", &n);

    for(size_t i = 0; i < n; ++i){

        if (check == true){

            int command;
            int *data = (int*) malloc(sizeof(int));

            scanf("%d %d", &command, data);
//            printf("check = true, command = %d; data = %d\n\n", command, *data);
//            show_buff(buff);

            if (command == 3) {

                push_back_buff(buff, data);
//                printf("PUSH_BACK data = %d\n", *data);
            }
            else if (command == 2) {

                int *val = pop_front_buff(buff);
                if (val == NULL){

//                    printf("\nval == NULL; *data = %d\n\n", *data);
                    if (*data != -1) { check = false; }
                }
                else {

//                    printf("\n*val = %d; *data = %d\n\n",*val, *data);
                    if(*val != *data){ check = false; }
                    free(val);
                }

//                printf("FREE: 2 \"else if (command == 2)\"\n");
                free(data);

            }
//            show_buff(buff);
//            printf("=========================================\n\n");
        }
        else {

            int command, data;
            scanf("%d %d", &command, &data);
//            printf("check = false, command = %d; data = %d\n", command, data);
        }
    }

    if (check == true) { printf("YES"); }
    else { printf("NO"); }
    free_buff(buff);
    return 0;
}
