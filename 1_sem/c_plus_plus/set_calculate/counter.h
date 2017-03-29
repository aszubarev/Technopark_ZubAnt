#ifndef COUNTER_H
#define COUNTER_H

#include "stdlib.h"
#include "stdbool.h"

typedef struct{

    size_t open_bracket;            // Количество открывающих скобок
    size_t close_bracket;           // Количество закрывающих скобок

    bool last_is_open_bracket;      // Последний прочитанный символ был открывающей скобкой
    bool last_is_close_bracket;     // Последний прочитанный символ был закрывающей скобкой

    bool last_is_operator;          // Последний прочитанный символ был оператором
    bool last_is_set;               // Последний прочитанный символ был множеством

    bool prev_last_is_set;          // Элемент перед последним елемнотом являеться множеством { "set" U "operator" }

} counter_t;

counter_t *create_counter();
void drop_flag_counter(counter_t *self);
void free_counter(counter_t *self);

#endif // COUNTER_H
