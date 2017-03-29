#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>

// 5_2. Стековые анаграммы.
// Пара слов называется стековой анаграммой, если одно слово можно получить из другого, проведя
// последовательность стековых операций с его буквами (взять очередную букву исходного слова и поместить
// ее в стек; взять букву из стека и добавить ее в конец выходного слова).
// Для заданной пары слов требуется определить, можно ли выполнить последовательность стековых
// операций, переводящую первое слово во второе. Все буквы в слове различные.

template<class Type>
class Stack
{
public:
    Stack();
    ~Stack();
    void push_back(Type val);
    Type pop_back();
    bool is_empty();
    Type get_last();

private:
    size_t Size;
    size_t Capacity;
    Type *Data;
    void realloc_data();
};

template<class Type>
Stack<Type>::Stack():Size(0), Capacity(8), Data(new Type[Capacity])
{}

template<class Type>
Stack<Type>::~Stack()
{
    delete[] Data;
}

template<class Type>
void Stack<Type>::push_back(Type val)
{
   if(Size == Capacity){ realloc_data(); }
   Data[Size] = val;
   ++Size;
}

template<class Type>
Type Stack<Type>::pop_back()
{
    if(Size == 0){ throw std::out_of_range("Size == 0"); }
    Type val = Data[Size];
    --Size;
    return val;
}

template<class Type>
bool Stack<Type>::is_empty()
{
    return (Size == 0) ? true: false;
}

template<class Type>
Type Stack<Type>::get_last()
{
    if(Size == 0){ throw std::out_of_range("Size == 0"); }
    return Data[Size - 1];
}

template<class Type>
void Stack<Type>::realloc_data()
{
    size_t new_capacity = 2 * Capacity;
    Type *new_data = new Type[new_capacity];

    std::copy(Data, Data + Capacity, new_data);
    delete[] Data;

    Data = new_data;
    Capacity = new_capacity;
}

bool is_anagram(std::string &left, std::string &right){

    std::string::iterator lit = left.begin();
    std::string::iterator rit = right.begin();

    std::string::iterator lend = left.end();
    std::string::iterator rend = right.end();

    bool is_lend = false;
    bool is_rend = false;
    bool check = true;

    while (true) {

        if (rit == rend){ is_rend = true; break; }

        while (true) {

            if (lit == lend) { is_lend = true; }

            if (stack.is_empty() != true && check == true) {

                if (stack.get_last() == *rit){
                    stack.pop_back();
                    ++rit;
                    break;
                }
                else if(is_lend == false){
                    stack.push_back(*lit);
                    ++lit;
                }
                else if (is_lend == true) { check = false; }
            }
            else if(is_lend == false){
                stack.push_back(*lit);
                ++lit;
            }
            else if (is_lend == true) { break; check = false; }
        }
        if (is_lend == true && check == false) { break; }
    }

    return is_rend;
}

int main(){

    std::string left, right;
    Stack<char> stack;
    std::cin >> left >> right;

    bool check = is_anagram(left, right);    

    if (check == true) { std::cout << "YES" << std::endl; }
    else { std::cout << "NO" << std::endl; }


    return 0;
}
