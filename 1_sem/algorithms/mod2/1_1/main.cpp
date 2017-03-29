#include <iostream>
#include <vector>

/*
 * 1_1. Ящики.
 * На склад привезли много пустых ящиков. Все ящики пронумерованы по порядку поступления от 0.
 * Известно, что их все можно сложить один в один (то есть так, что каждый следующий помещается
 * в предыдущий). Один ящик можно вложить в другой, если его можно перевернуть так, что размеры
 * одного ящика по всем осям станут строго меньше размеров другого ящика по соответствующим
 * осям. Требуется определить, в какой последовательности они будут вложены друг в друга.
 * Вывести номера ящиков.
 * */

struct Box{
    int x;
    int y;
    int z;
    int index;
};


template<class type>
void insert_sort(std::vector<type>& arr, bool( *comparer )( const type&, const type& ))
{
    size_t size = arr.size();
    for(size_t i = 1; i < size; ++i)
    {
        type x = arr[i];
        size_t j = i;
        while (j > 0 && comparer(x, arr[j - 1]))
        {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = x;
    }
}

void scan_data(std::istream &stream, std::vector<Box> &vec, int n)
{
    for(int i = 0; i < n; ++i)
    {
        int x, y, z;
        stream >> x >> y >> z;
        vec[i].x = x;
        vec[i].y = y;
        vec[i].z = z;
        vec[i].index = i;
    }
}

void show_index(const std::vector<Box> &vec)
{
    for (size_t i = 0; i < vec.size() - 1; ++i)
    {
        std::cout << vec[i].index << " ";
    }
    std::cout << vec[vec.size() - 1].index << std::endl;
}

bool compare_box(const Box &left, const Box &right)
{
    if (left.x > right.x) return false;

    if (left.y > right.y) return false;

    if (left.z > right.z) return false;

    return true;
}

void sort_by_box(std::vector<Box> &vec)
{
    insert_sort(vec, compare_box);
}


bool compare_param(const int &a, const int &b)
{
    return a < b;
}

void sorted_py_param(std::vector<Box> &vec)
{
    for (auto &it: vec)
    {
        std::vector<int> arr = {it.x, it.y, it.z};
        insert_sort(arr, compare_param);
        it.x = arr[0];
        it.y = arr[1];
        it.z = arr[2];
    }
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<Box> vec(n);

    scan_data(std::cin, vec, n);

    sorted_py_param(vec);

    sort_by_box(vec);

    show_index(vec);

    return 0;
}







