#include <iostream>
#include <chrono>
#include <random>

int my_rand(int a, int b)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen (seed);
    std::uniform_int_distribution<int> dist (a, b);
    return dist(gen);
}

int Partition(int* data, int n)
{
    if( n <= 1 ) { return 0; }

    int rand_ind = my_rand(0, n - 1);

    int pivot_ind = n - 1;
    std::swap(data[rand_ind], data[pivot_ind]);
    const int pivot = data[pivot_ind];

    int i = 0;
    int j = 0;

    while( j < pivot_ind )
    {
        for(; data[j] > pivot && j <= pivot_ind; ++j)
        {}
        for(; data[i] <= pivot && i < j ; ++i)
        {}

        if( j != pivot_ind )
        {
            std::swap( data[i++], data[j++] );
        }
    }

    std::swap(data[i], data[pivot_ind]);

    return i;
}

int get_ord_stat(int *data, int n, int k)
{
    int part = Partition(data, n);
    if(part == k)
    {
        return data[part];
    }
    else if( part > k )
    {
        return get_ord_stat(data, part, k);
    }
    else
    {
        return get_ord_stat(data + part + 1, n - (part + 1), k - (part + 1));
    }
}

void scan(std::istream &stream, int *data, int n)
{
    for(int i = 0; i < n; ++i)
    {
        int val = 0;
        stream >> val;
        data[i] = val;
    }
}

int main( )
{
    int n, k;
    std::cin >> n >> k;

    int *data = new int[n];
    scan(std::cin, data, n);

    std::cout << get_ord_stat(data, n, k) << std::endl;

    delete[] data;

    return 0;
}
