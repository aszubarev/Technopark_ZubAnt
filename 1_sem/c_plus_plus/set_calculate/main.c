#include <stdio.h>
#include "buffer.h"
#include "vector.h"
#include "string_t.h"
#include "counter.h"
#include "set_operator.h"

// ЗАПИЛИТЬ ПРОВЕРКУ НА ()

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

//        free_buff(buff_vector);

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

void print_buff(buff_t *stack){
    size_t size = stack->Size;
    vector_t *v = NULL;
//    printf("BUFFER: ");
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
                        printf("STACK OPERATOR "); print_buff(stack_operator);
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
                    printf("END STACK OPERATOR "); print_buff(stack_operator);printf("\n");
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



vector_t* get_answer(buff_t *postfix_buff){

    printf("GET ANSWER\n");

    vector_t *answer = NULL;
    buff_t *stack = create_buff();

    while (is_empty_buff(postfix_buff) == false) {


        vector_t *front_buff = pop_front_buff(postfix_buff);
        if (front_buff == NULL)
        {
            printf("LINE = %d; ERROR: %s\n", __LINE__, "front_buff == NULL\n");
            free_buff_vector(stack);
            return NULL;
        }
        if (front_buff->is_vector)
        {
            push_back_buff(stack, front_buff);
            printf("STACK == "); print_buff(stack); printf("\n");
        }
        else
        {
            if (stack->Size < 2)
            {
                printf("LINE = %d; ERROR: %s\n", __LINE__, "stack->Size < 2\n");
                free_buff_vector(stack);
                return NULL;
            }

            vector_t *right_set = pop_back_buff(stack);
            vector_t *left_set = pop_back_buff(stack);

            if(right_set== NULL || left_set == NULL ||
                    right_set->is_vector == false || left_set->is_vector == false)
            {
                printf("LINE = %d; ERROR: %s\n", __LINE__, "right_set== NULL || left_set == NULL || "
                                                           "right_set->is_vector == false || "
                                                           "left_set->is_vector == false\n");
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

            printf("LEFT = "); print_vector_clear(left_set);
            printf("; RIGHT = "); print_vector_clear(right_set);
            printf("; OPERATOR = %c\n", front_buff->symb);
            printf("RESULT = "); print_vector_clear(curr_operation); printf("\n");
            printf("STACK == "); print_buff(stack); printf("\n");


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
                printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "set after set\n");
                break;
            }
            ///@ref " ) [] " == BAD
            else if (cnt->last_is_close_bracket)
            {
                is_valid = false;
                printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "set after \'(\'\n");
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
                    cnt->last_is_set = false;
                }
                ///@ref " ) U " == GOOD
                else if (cnt->last_is_close_bracket == true)
                {
                    drop_flag_counter(cnt);
                    cnt->last_is_operator = true;
                }
                ///@ref " ^ U " == BAD
                else if (cnt->last_is_operator == true)
                {
                    is_valid = false;
                    printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "operator after operator\n");
                    break;
                }
                ///@ref " ( U " == BAD
                else if (cnt->last_is_open_bracket == true)
                {
                    is_valid = false;
                    printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "operator after \'(\'\n");
                    break;
                }
                else if (i == 0)
                {
                    is_valid = false;
                    printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "operator on start\n");
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
                    printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "\'(\' after \')\' == )(\n");
                    break;
                }
                ///@ref " [] ( " == BAD
                else if (cnt->last_is_set == true)
                {
                    is_valid = false;
                    printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "\'(\' after set == [] (\n");
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
                    printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "\')\' after \'(\' == ()\n");
                    break;
                }
                ///@ref " U ) " == BAD
                else if (cnt->last_is_operator == true)
                {
                    is_valid = false;
                    printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "\')\' after operator == U )\n");
                    break;
                }
                ///@ref " start ) " == BAD
                else if (i == 0)
                {
                    is_valid = false;
                    printf("INDEX = %zu; LINE = %d; ERROR: %s\n", i, __LINE__, "\')\' on start\n");
                    break;
                }
            }
        }
    }

    if (is_valid == true && cnt->open_bracket != cnt->close_bracket)
    {
         free_counter(cnt);
         printf("LINE = %d; ERROR: %s\n", __LINE__, "cnt->open_bracket != cnt->close_bracket\n");
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
        free_string(infix_string);
        free_buff_vector(infix_buff);
        return 0;
    }

    print_buff(infix_buff);
    /// ==============================================================
    if (is_valid_infix_buff(infix_buff) == true)
    {
        printf("BUFF IS VALID\n");
    }
    else
    {
        free_string(infix_string);
        free_buff_vector(infix_buff);
        printf("BUFF IS NOT VALID\n");
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

    printf("POSTFIX: ");
    print_buff(postfix_buff);
    printf("\n");
    /// ==============================================================

    vector_t *answer = get_answer(postfix_buff);

    if (answer == NULL) {
        printf("[error]");
        printf("LINE = %d; ERROR: %s\n", __LINE__, "answer == NULL\n");
        free_buff_vector(postfix_buff);
        free_buff_vector(infix_buff);
        free_string(infix_string);
        return 0;
    }

    printf("ANSWER: ");
    print_vector_clear(answer);
    printf("\n");


    free_vector(answer);

    /// ==============================================================

    free_buff_vector(postfix_buff);
    free_buff_vector(infix_buff);
    free_string(infix_string);

    return 0;
}
