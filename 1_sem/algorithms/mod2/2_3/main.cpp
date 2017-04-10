#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <stdexcept>

template<class type>
class Heap{
public:

    void push_heap(type element, bool (*comp)(const type&, const type&));
    type pop_head(bool (*comp)(const type&, const type&));
    type at_head();
    size_t size() { return data.size(); }
    void show();
private:

    std::vector<type> data;
    void heapify(size_t index, bool (*comp)(const type&, const type&));
};


template<class type>
void Heap<type>::push_heap(type element, bool (*comp)(const type&, const type&))
{
    data.push_back(element);

    size_t size = data.size();
    size_t index = size - 1;
    size_t root = (index - 1) / 2;

    while (comp(data[root], data[index]))
    {
        std::swap(data[root], data[index]);
        index = root;
        root = (index - 1) / 2;
    }
}


template<class type>
type Heap<type>::pop_head(bool (*comp)(const type &, const type &))
{
    if (data.size() == 0) { throw std::out_of_range("size = 0"); }

    type head = data[0];   // Запоминаем значение корня.

    data[0] = data.back();  // Переносим последний элемент в корень.
    data.pop_back();

    // Вызываем Heapify для корня.
    if(!data.empty())
    {
        heapify(0, comp);
    }

    return head;
}


template<class type>
type Heap<type>::at_head()
{
    if (data.size() == 0) { throw std::out_of_range("size = 0"); }
    return data[0];
}


template<class type>
void Heap<type>::show()
{
    for(auto &pair: data)
    {
        std::cout << pair[0] << " " << pair[1] << std::endl;
    }
    std::cout  << std::endl;
}


template<class type>
void Heap<type>::heapify(size_t index, bool (*comp)(const type &, const type &))
{

    size_t index_comp = index;
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    // Ищем меньшего сына, если такой есть
    if(left < data.size() && comp(data[index_comp], data[left]))
    {
        index_comp = left;
    }
    if(right < data.size() && comp(data[index_comp], data[right]))
    {
        index_comp = right;
    }
    // Если меньший сын есть, то проталкиваем в него корень
    if(index != index_comp)
    {
        std::swap(data[index], data[index_comp]);
        heapify(index_comp, comp);
    }
}


bool compare_push_heap(const std::array<int, 2> &left, const std::array<int, 2> &right)
{
    return left[1] > right[1];
}

void scan_pair(std::istream &stream, std::vector<std::array<int, 2>> &v_pair, int n)
{
    for (int i = 0; i < n; ++i)
    {
        int arrival = 0;    // прибытие поезда
        int departure = 0;  // убытие поезда
        stream >> arrival >> departure;
        v_pair[i] = {arrival, departure};
    }
}


size_t get_max_deadlock(std::vector<std::array<int, 2>> &v_pair)
{
    Heap<std::array<int, 2>> heap;

    size_t cnt = 0;
    size_t cnt_max = cnt;

    for(auto &pair: v_pair)
    {
        while (heap.size() != 0 &&  heap.at_head()[1] < pair[0])
        {
            heap.pop_head(compare_push_heap);
            --cnt;
        }

        heap.push_heap(pair, compare_push_heap);
        ++cnt;
        if (cnt > cnt_max)
        {
            cnt_max = cnt;
        }
    }

    return cnt_max;
}

int main()
{
    int n = 0;
    std::cin >> n;

    std::vector<std::array<int, 2>> v_pair(n);

    scan_pair(std::cin, v_pair, n);

    size_t max_deadlock = get_max_deadlock(v_pair);

    std::cout << max_deadlock << std:: endl;

    return 0;
}


