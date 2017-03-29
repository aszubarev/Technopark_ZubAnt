#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <array>

// 7_2. Заявки на переговоры.
// В большой IT-фирме есть только одна переговорная комната. Желающие посовещаться заполняют заявки с
// желаемым временем начала и конца. Ваша задача определить максимальное количество заявок, которое
// может быть удовлетворено.

static bool comp_by_end(int *a, int *b){
    return a[1] < b[1];
}

size_t numb_requests(std::vector<int *> &v_pair)
{
    if(v_pair.empty()){

        throw std::logic_error("empty input vector");
    }
    std::sort(v_pair.begin(), v_pair.end(), comp_by_end);

    int rigth_border = (*v_pair.begin())[1];
    size_t cnt = 1;

    for(auto &it:v_pair){
        if(it[0] >= rigth_border){
            ++cnt;
            rigth_border = it[1];
        }
    }

    return cnt;
}

void delete_input(std::vector <int*> &v_inp){

    for(auto &pair:v_inp){
        delete[] pair;
    }
}

int main(){

    std::vector<int *> v_inp;

    int l, r;
    while (std::cin >> l >> r) {
        int *pair = new int[2];
        pair[0] = l;
        pair[1] = r;
        v_inp.push_back(pair);
    }

    std::cout << numb_requests(v_inp) << std::endl;

    delete_input(v_inp);

    return 0;
}

