#include "counter.h"

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
