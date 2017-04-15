#include <iostream>
#include <cstdlib>

struct date
{
	int day;
	int month;
	int year;
	bool is_born;
	bool is_dead;
	date( );
	date( int day, int month, int year );
	void input_date_of_start( );
	void input_date_of_end( date& date_of_birth );
	bool operator>( date other );
	bool operator>=( date other );
	bool operator<( date other );
	bool operator<=( date other );
};

date unit( );

date::date( )
{
	day = 0;
	month = 0;
	year = 0;
	is_born = false;
}

date unit( )
{
	date unit_date;
	unit_date.day = rand( ) % 31;
	unit_date.month = rand( ) % 13;
	unit_date.year = 1900 + rand( ) % 150;
	return unit_date;
}

date::date( int day, int month, int year ): day( day ), month( month ), year( year )
{
	is_born = false;
}

void date::input_date_of_start( )
{
	// std::cin >> day;
	// std::cin >> month;
	// std::cin >> year;
	date un = unit( );
	day = un.day;
	month = un.month;
	year = un.year;
	is_born = false;
	is_dead = false;
}

void date::input_date_of_end( date& date_of_birth )
{
	int temp_dead_day = 0;
	int temp_dead_month = 0;
	int temp_dead_year = 0;
	date un = unit( );
	temp_dead_day = un.day;
	temp_dead_month = un.month;
	temp_dead_year = un.year;
	// std::cin >> temp_dead_day;
	// std::cin >> temp_dead_month;
	// std::cin >> temp_dead_year;

	date temp_dead( temp_dead_day, temp_dead_month, temp_dead_year );
	
	date date18( date_of_birth.day, date_of_birth.month, date_of_birth.year + 18 );
	date date80( date_of_birth.day, date_of_birth.month, date_of_birth.year + 80 );

	if( date18 < temp_dead ){	
		date_of_birth.is_born = true;
		date_of_birth.is_dead = false;
		date_of_birth.year += 18;
	}

	if( date80 < temp_dead && date_of_birth.is_born == true ){
		is_dead = true;
		is_born = false;
		day = date80.day;
		month = date80.month;
		year = date80.year;
	}
	else if( date80 >= temp_dead && date_of_birth.is_born == true ){
		is_dead = true;
		is_born = false;
		day = temp_dead.day;
		month = temp_dead.month;
		year = temp_dead.year;
	}
}

bool date::operator>( date other )
{
	if( year > other.year ){
		return true;
	}
	else if( year == other.year ){
		if( month > other.month ){
			return true;
		}
		else if( month == other.month ){
			if( day > other.day ){
				return true;
			}
			else if( day <= other.day ){
				return false;
			}
		}
		else if( month < other.month ){
			return false;
		}
	}
	else if( year < other.year ){
		return false;
	}
}

bool date::operator<( date other )
{
	if( year < other.year ){
		return true;
	}
	else if( year == other.year ){
		if( month < other.month ){
			return true;
		}
		else if( month == other.month ){
			if( day < other.day ){
				return true;
			}
			else if( day >= other.day ){
				return false;
			}
		}
		else if( month > other.month ){
			return false;
		}
	}
	else if( year > other.year ){
		return false;
	}
}

bool date::operator<=( date other )
{
	if( year < other.year ){
		return true;
	}
	else if( year == other.year ){
		if( month < other.month ){
			return true;
		}
		else if( month == other.month ){
			if( day <= other.day ){
				return true;
			}
			else if( day > other.day ){
				return false;
			}
		}
		else if( month > other.month ){
			return false;
		}
	}
	else if( year > other.year ){
		return false;
	}
}

bool date::operator>=( date other )
{
	if( this->year > other.year ){
		return true;
	}
	else if( year == other.year ){
		if( month > other.month ){
			return true;
		}
		else if( month == other.month ){
			if( day >= other.day ){
				return true;
			}
			else if( day < other.day ){
				return false;
			}
		}
		else if( month < other.month ){
			return false;
		}
	}
	else if( year < other.year ){
		return false;
	}
}

void BuildHeap( date* a, int heap_size );
void HeapSort( date* a, int n );
void SiftDown( date* a, int i, int heap_size );
void print( date* a, int n );

void BuildHeap( date* a, int heap_size )
{
	int parent = ( heap_size - 2 ) / 2;
	for( int i = parent; i >= 0; --i ){
		SiftDown( a, i, heap_size);
	}
}

void HeapSort( date* a, int n )
{
	int heap_size = n;
	BuildHeap( a, heap_size );

	while( heap_size > 1 ){	
		std::swap( a[0], a[heap_size - 1] );
		--heap_size;
		SiftDown( a, 0, heap_size );
	}
}

void SiftDown( date* a, int i, int heap_size )
{
	int smallest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	if( a[smallest] < a[left] && left < heap_size ){
		smallest = left;
	}
	if( a[smallest] < a[right] && right < heap_size ){
		smallest = right;
	}
	if( smallest != i ){
		std::swap( a[smallest], a[i] );
		SiftDown( a, smallest, heap_size );
	}
}

void print( date* a, int n )
{
	for( int j = 0; j < n; ++j ){
		if( a[j].is_born == true && a[j].is_dead == false ){
			std::cout << a[j].day<< ' ';
			std::cout << a[j].month<< ' ';
			std::cout << a[j].year<< ' ';
			std::cout << "is_born: " << a[j].is_born; 
			std::cout << "is_dead: " << a[j].is_dead; 
			std::cout << std::endl;
		}
		if( a[j].is_born == false && a[j].is_dead == true ){
			std::cout << a[j].day<< ' ';
			std::cout << a[j].month<< ' ';
			std::cout << a[j].year<< ' ';
			std::cout << "is_born: " << a[j].is_born; 
			std::cout << "is_dead: " << a[j].is_dead; 
			std::cout << std::endl;
		}
	}
}

int max_modernists( date* a, int size )
{
	int current_max = 0;
	int max = current_max;
	for( int i = 0; i < size; ++i ){
		if( a[i].is_born == true && a[i].is_dead == false ){
			++current_max;
		}
		else if( a[i].is_born == false && a[i].is_dead == true ){
			--current_max;
		}
		if( current_max > max ){
			max = current_max;
		}
	}
	return max;
}

int main()
{
	int n = 0;
	std::cin >> n;
	int size = 2 * n;
	date* all_dates = new date[size];
	for( int i = 0; i < size; ++i ){
		all_dates[i].input_date_of_start( );
		if( i < size ){
			++i;
			all_dates[i].input_date_of_end( all_dates[i - 1] );
		}
	}

	HeapSort( all_dates, size );

	print( all_dates, size );
	std::cout << max_modernists( all_dates, size );

	
	delete[] all_dates;
	return 0;
}