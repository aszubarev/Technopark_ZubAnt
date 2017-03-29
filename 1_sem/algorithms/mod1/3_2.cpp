#include <cassert>
#include <iostream>

#include <stdexcept>
#include <cstdlib>
#include <algorithm>

// 3_2. ​Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m] значения массива строго
// возрастают, а на интервале [m, n-1] строго убывают. Найти m за O(log m).

template<class Type>
class my_vector
{
public:
    my_vector();
    my_vector(const my_vector<Type> &obj);
    my_vector& operator=(const my_vector<Type> &obj);
    void push_back(Type val);
    size_t size();
    bool empty();
    Type pop_back();
    Type operator[](size_t index);
    ~my_vector();

private:
    size_t Size;
    size_t Capacity;
    Type *Data;

    void realloc_data();
};


template<class Type>
my_vector<Type>::my_vector()
{
    Size = 0;
    Capacity = 8;
    Data = new Type[Capacity];
}

template<class Type>
my_vector<Type>::my_vector(const my_vector<Type> &copy):
    Size(copy.Size),
    Capacity(copy.Capacity),
    Data(new Type[Capacity])
{
    std::copy(copy.Data, copy.Data + copy.Size, Data);
}

template<class Type>
my_vector<Type> &my_vector<Type>::operator=(const my_vector<Type> &obj)
{
    ~my_vector();
    Size = obj.Size;
    Capacity = obj.Capacity;
    Data = new Type[Capacity];
    std::copy(obj.Data, obj.Data + obj.Size, Data);
    return *this;
}

template<class Type>
void my_vector<Type>::push_back(Type val)
{
    if(Size == Capacity){ realloc_data(); }
    Data[Size] = val;
    ++Size;
}

template<class Type>
size_t my_vector<Type>::size()
{
    return Size;
}

template<class Type>
bool my_vector<Type>::empty()
{
   return (Size == 0) ? true: false;
}

template<class Type>
Type my_vector<Type>::pop_back()
{
    if (Size == 0) { throw std::out_of_range("Size == 0"); }
    Type val = Data[Size];
    --Size;
    return val;
}

template<class Type>
Type my_vector<Type>::operator[](size_t index)
{
    if(index >= Size)  { throw std::out_of_range("index >= Size"); }
    return Data[index];
}

template<class Type>
my_vector<Type>::~my_vector()
{
    delete Data;
}

template<class Type>
void my_vector<Type>::realloc_data()
{
    size_t new_capacity = 2 * Capacity;
    Type *new_data = new Type[new_capacity];

    std::copy(Data, Data + Capacity, new_data);
    delete[] Data;

    Data = new_data;
    Capacity = new_capacity;
}


//===================================================================

class BiSearsh
{
public:
    BiSearsh();
    void push_back(int x);
    int get_higher_index();
private:
    my_vector<int> data;
};


BiSearsh::BiSearsh()
{}

void BiSearsh::push_back(int x)
{
    data.push_back(x);
}

int BiSearsh::get_higher_index()
{
    assert(!data.empty());

    size_t curr = 1;
    size_t prev = 0;
    while (true) {

        if (curr >= data.size() ){
            curr = data.size() - 1;
            if(curr == prev) { prev = 0; }
            break;
        }
        if (data[curr] < data[curr - 1]) {
            break;
        }

        prev = curr;
        curr *= 2;
    }

    int a = prev;
    int b = curr ;

    int left_board = a;
    int right_board = b;

    int index = 0;
    bool find = false;

    while( (b - a) > 0 ){

        index = a + (b - a) / 2;
        if(index != left_board && index != right_board){

            if(data[index - 1] < data[index] && data[index] > data[index + 1]){

                return index;
            }
            else if(index + 1 == right_board && data[index] < data[right_board]){
                return right_board;
            }
            else if(data[index - 1] < data[index] && data[index] < data[index + 1]){

                a = index;
            }
            else if(data[index - 1] > data[index] && data[index] > data[index + 1]){

                b = index;
            }
        }
        else {

            if(index == left_board && data[index] < data[index + 1]){

                return index + 1;
            }
            return index;
        }
    }

    if(!find){
        return -1;
//        throw std::logic_error("cannot find number in data vector");
    }

    return index;
}


int main()
{
    int n = 0;
    
    BiSearsh obj;
    std::cin >> n;

    for(int i = 0; i < n; ++i){
        int x = 0;
        std::cin >> x;
        obj.push_back(x);
    }

    std::cout << obj.get_higher_index() << std::endl;

    return 0;
}
