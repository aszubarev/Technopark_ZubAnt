/*
 * Группа людей называется современниками если был такой момент, когда они могли собраться
 * вместе. Для этого в этот момент каждому из них должно было уже исполниться 18 лет, но ещё не
 * исполниться 80 лет.
 * Дан список Жизни Великих Людей. Необходимо получить максимальное количество
 * современников. В день 18летия человек уже может принимать участие в собраниях, а в день
 * 80летия и в день смерти уже не может.
 * Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия. В этих случаях
 * принимать участие в собраниях он не мог.
 * */


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Date
{
    Date(bool born, int d = 0, int m = 0, int y = 0);
    bool is_born;
    int day;
    int month;
    int year;
};

Date::Date(bool born, int d, int m, int y): is_born(born), day(d), month(m), year(y)
{}


void print_date(const std::vector<Date> &vec)
{
    std::cout << "Date's" << std::endl;
    for(auto &it:vec)
    {
        std::cout << it.day << " " << it.month << " " << it.year << std::endl;
    }
}


/// В seg не может лежать отрезок жизни меньше или равный 18 лет
/// Старт отрезка жизни увеличиваеться на 18 лет от начала рождения
/// Отсекаеться время жизни после 80 лет
bool convert_seglive_to_contemporary(Date &start, Date &stop)
{
    int delta_year = stop.year - start.year;

    if(delta_year > 18)
    {
        if (delta_year < 80) { /*do nothing*/ }
        else if (delta_year > 80)
        {
            stop.year = start.year + 80;
            stop.month = start.month;
            stop.day = start.day;
        }
        else if(delta_year == 80)
        {
            int delta_month = stop.month - start.month;

            if(delta_month > 0)
            {
                stop.month = start.month;
                stop.day = start.day;
            }
            else if(delta_month == 0)
            {
                int delta_day = stop.day - start.day;
                if(delta_day > 0)
                {
                    stop.day = start.day;
                }
            }
        }
    }
    else if(delta_year == 18)
    {
        int delta_month = stop.month - start.month;

        if(delta_month > 0) { /*do nothing*/ }
        else if(delta_month == 0)
        {
            int delta_day = stop.day - start.day;

            if(delta_day > 0) { /*do nothing*/ }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    start.year += 18;

    return true;
}


vector<Date> scan_data(std::istream &stream)
{
    int n = 0;
    stream >> n;
    std::vector<Date> vec;
    for(int i = 0; i < n; ++i)
    {
        int d_born = 0;
        int m_born = 0;
        int y_born = 0;
        int d_die = 0;
        int m_die = 0;
        int y_die = 0;

        stream >> d_born >> m_born >> y_born
                >> d_die >> m_die >> y_die;

        Date start(true, d_born, m_born, y_born);
        Date stop(false, d_die, m_die, y_die);

        bool check = convert_seglive_to_contemporary(start, stop);

        if(check == true)
        {
            vec.push_back(start);
            vec.push_back(stop);
        }
    }

    return vec;
}

int get_count_max_segment(const std::vector<Date> &contemporary)
{
    int cnt = 0;
    int cnt_max = cnt;
    for(auto &it:contemporary)
    {
        if (it.is_born == true)
        {
            ++cnt;
            if (cnt > cnt_max)
            {
                cnt_max = cnt;
            }
        }
        else
        {
            --cnt;
        }
    }

    return cnt_max;
}


bool comp_contemporary(const Date &left, const Date &right)
{
    if (left.year < right.year)
    {
        return true;
    }
    else if (left.year == right.year)
    {
        if (left.month < right.month)
        {
            return true;
        }
        else if (left.month == right.month)
        {
            if(left.day < right.day)
            {
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}


void Heapify(std::vector<Date> &arr, size_t root, size_t size, bool (*comp)(const Date &left, const Date &right))
{
    size_t comp_root = root;
    size_t left = 2 * root + 1;
    size_t right = 2 * root + 2;

    if (left < size && !comp(arr[left], arr[root]))
    {
        comp_root = left;
    }
    if (right < size && !comp(arr[right], arr[comp_root]))
    {
        comp_root = right;
    }

    if (root != comp_root)
    {
        std::swap(arr[root], arr[comp_root]);
        Heapify(arr, comp_root, size, comp);
    }
}

void BuildHeap(std::vector<Date> &arr, bool (*comp)(const Date &left, const Date &right))
{
    for(int i = arr.size() / 2; i >= 0; --i)
    {
        Heapify(arr, static_cast<size_t>(i), arr.size(), comp);
    }
}

void HeapSort(std::vector<Date> &arr, bool (*comp)(const Date &left, const Date &right))
{
    BuildHeap(arr, comp);
    size_t heapSize = arr.size();

    while (heapSize > 1)
    {
        std::swap(arr[0], arr[heapSize - 1]);
        --heapSize;
        Heapify(arr, 0, heapSize, comp);
    }
}


int main()
{
    vector<Date> contemporary = std::move(scan_data(std::cin));
//    std::cout << contemporary.size() << std::endl;

    HeapSort(contemporary, comp_contemporary);
//    print_date(contemporary);

    int cnt_max = get_count_max_segment(contemporary);
    std::cout << cnt_max << std::endl;

    return 0;
}


