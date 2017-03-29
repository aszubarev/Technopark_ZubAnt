#include <iostream>
#include <cstdlib>

/*
 * 6_1.​ Высокая пирамида. ​Каждый вышележащий слой пирамиды должен быть не больше нижележащего.
 * Дано N кубиков. Требуется определить каким количеством способов можно выстроить из этих кубиков
 * пирамиду.
 * Формат входных данных:
 * На вход подается количество кубиков N.
 * Формат выходных данных:
 * Вывести число различных пирамид из N кубиков.
 * */


size_t getnumb_pyr(size_t numb_rem_cub, size_t right_board, size_t **table) // numb_remaining_cubes - количество оставшихся кубиков
{
    size_t cnt = 0;
    if(numb_rem_cub <= 1){
        if (table[numb_rem_cub][right_board] == 0){
            table[numb_rem_cub][right_board] = 1;
        }
        return 1;
    }    

    for(size_t ind = 1; ind <= right_board && ind <= numb_rem_cub; ++ind){

        size_t ostatok = numb_rem_cub - ind;

        if (table[ostatok][ind] == 0){
            table[ostatok][ind] = getnumb_pyr(ostatok, ind, table);
        }

        cnt += table[ostatok][ind];

    }
    return cnt;
}

int main(){
    
    int blocks;
    std::cin >> blocks;

    size_t size_table = blocks + 2;
    size_t **table1 = new size_t*[size_table];

    for (size_t i = 0; i < size_table; ++i) {
        table1[i] = new size_t[size_table];
        std::fill(table1[i], table1[i] + size_table, '\0');
    }

    std::cout << getnumb_pyr(blocks, blocks, table1) << std::endl;

    for (size_t i = 0; i < size_table; ++i) {
        delete[] table1[i];
    }

    delete[] table1;

    return 0;
}
