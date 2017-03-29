#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>

// 2_2. ​Вычислить площадь выпуклого n-угольника, заданного координатами своих вершин. Вначале вводится
// количество вершин, затем последовательно целочисленные координаты всех вершин в порядке обхода
// против часовой стрелки.

template<class Type>
class my_vector
{
public:
    my_vector();
    my_vector(const my_vector<Type> &obj);
    my_vector& operator=(const my_vector<Type> &obj);
    void push_back(Type val);
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

//==================================================================

class Square_n
{
public:
    Square_n(int n);
    void push_back(double x, double y);
    double square();
private:

    my_vector<double> x_vec;
    my_vector<double> y_vec;
    int size;
};


Square_n::Square_n(int n):size(n) {}

void Square_n::push_back(double x, double y)
{
    x_vec.push_back(x);
    y_vec.push_back(y);
}

static double Geron(double a, double b, double c){
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

double Square_n::square()
{

    double x_0 = (x_vec[0] + x_vec[2]) / 2;
    double y_0 = (y_vec[0] + y_vec[2]) / 2;

    double result = 0.0;

    double len_op = sqrt(std::pow(x_vec[0] - x_0, 2) + std::pow(y_vec[0] - y_0, 2));
    double len_first = len_op;

    for(int i = 1; i < size; ++i){

        double len_edge = sqrt(std::pow(x_vec[i] - x_vec[i - 1], 2) +
                               std::pow(y_vec[i] - y_vec[i - 1], 2));
        double len_to_0 = sqrt(std::pow(x_vec[i] - x_0, 2) + std::pow(y_vec[i] - y_0, 2));
        result += Geron(len_op, len_edge, len_to_0);
        len_op = len_to_0;
    }

    double len_edge = sqrt(std::pow(x_vec[size - 1] - x_vec[0], 2) +
                           std::pow(y_vec[size - 1] - y_vec[0], 2));
    result += Geron(len_op, len_edge, len_first);

    return result;
}

int main()
{
    int n = 0;    

    std::cin >> n;
    Square_n obj(n);

    for(int i = 0; i < n; ++i){
        double x = 0.0;
        double y = 0.0;
        std::cin >> x >> y;
        obj.push_back(x ,y);
    }

    std::cout << obj.square() << std::endl;


    return 0;
}

