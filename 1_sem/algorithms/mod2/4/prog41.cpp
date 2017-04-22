#include <iostream>

int Partition( long* a, int n )
{
    if( n <= 1 ){
        return 0;
    }
    long* first = &a[0];
    long* last = &a[n - 1];
    long* medium = &a[n / 2];
    long* mediana = first;

    if( ( *first <= *last && *first >= *medium ) || ( *first <= *medium && *first>= *last ) ){
        mediana = first;
    }
    else if( ( *medium <= *last && *medium >= *first ) || ( *medium <= *first && *medium>= *last ) ){
        mediana = medium;
    }
    else{
        mediana = last;
    }

    int pivot_position = n - 1;
    std::swap( *mediana, a[pivot_position]);
    const long& pivot = a[pivot_position];
    int i = 0;
    int j = 0;

    while( j < pivot_position ){

        for( ; a[j] > pivot && j <= pivot_position; ++j ){}
        for( ; a[i] <= pivot && i < j ; ++i ){}


        if( j != pivot_position ){
            std::swap( a[i++], a[j++] );
        }
    }

    std::swap( a[i], a[pivot_position]);

    return i;
}

int get_ord_stat( long* a, int n, int k )
{
    int part = Partition( a, n );
    if( part == k ){
        return a[part];
    }
    if( part > k ){
        get_ord_stat( a, part, k );
    }
    if( part < k ){
        get_ord_stat( a + part + 1, n - (part + 1), k - ( part + 1 ) );
    }
}

int main( )
{
    int n = 0;
    int k = 0;
    std::cin >> n;
    std::cin >> k;

    long* mass = new long[n];
    int j = 0;
    j = n;
    for( int i = 0; i < n; ++i ){
        std::cin >> mass[i];
    }
    std::cout << get_ord_stat( mass, n, k );

    delete[] mass;

    return 0;
}
