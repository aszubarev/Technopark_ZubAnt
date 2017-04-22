#include <iostream>
#include <vector>
#include <chrono>
#include <random>

int rand(int a, int b)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen (seed);
    std::uniform_int_distribution<int> dist (a, b);
    return dist(gen);
}

std::vector<int> scan(std::istream &stream, int n)
{
    std::vector<int> input;
    for(int i = 0; i < n; ++i)
    {
        int val = 0;
        stream >> val;
        input.push_back(val);
    }

    return input;
}

void print_data(std::vector<int> &v)
{
    std::cout << "Data: " << std::endl;
    for(auto &it:v)
    {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}

int Partition(std::vector<int> &data, int ind_start, int n)
{
    if( n <= 1 )
    {
//        std::cout << "n <= 1" << std::endl;
        return ind_start;
    }
    int rand_ind = rand(ind_start, ind_start + n - 1);

    int pivot_ind = ind_start + n - 1;
//    std::cout << "ind_start = " << ind_start << "; n = " << n
//              << "; rand_ind = " << rand_ind
//              << "; pivot_ind = " << pivot_ind << std::endl;




//    print_data(data);
    std::swap(data[rand_ind], data[pivot_ind]);
//    print_data(data);
    int pivot = data[pivot_ind];
//    std::cout << "pivot = " << pivot << std::endl;

    int i = 0;
    int j = 0;

    while( ind_start + j < pivot_ind )
    {
        for( ; data[ind_start + j] > pivot && j <= pivot_ind; ++j ){}
        for( ; data[ind_start + i] <= pivot && i < j ; ++i ){}


//        std::cout << "i = " << i << "; j = " << j <<std::endl;

        if( ind_start +j != pivot_ind )
        {
            std::swap(data[ind_start + i++], data[ind_start + j++]);
        }
    }

    std::swap(data[ind_start + i], data[pivot_ind]);
    return ind_start + i;
}

int get_k_statistics(std::vector<int> &data, int ind_start, int n, int k)
{
    int part = Partition(data, ind_start, n);
//    std::cout << "part = " << part << "; k =  " << k << std::endl;
//    print_data(data);
//    std::cout << std::endl;
    int ret_data = 0;
    if( part == k )
    {
        return data[part];
    }
    else if( part > k )
    {
//        std::cout << "part > k" << std::endl;
        ret_data = get_k_statistics(data, ind_start, part, k);
    }
    else
    {
//        std::cout << "part < k" << std::endl;
        ret_data = get_k_statistics(data, part + 1, n - (part + 1), k );
    }
    return ret_data;
}

int main()
{
    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<int> data = std::move(scan(std::cin, n));
    int ordval = get_k_statistics(data, 0, data.size(), k);
    std::cout << ordval << std::endl;

    return 0;
}
