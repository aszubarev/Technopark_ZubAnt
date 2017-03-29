//============================================================================
// Name        : ads22.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdint.h>

using namespace std;

void fill_table(int64_t **table, int blocks) {
	for (int diag = 0; diag < blocks * 2 + 1; ++diag) {
		for (int m = 1; m <= diag + 1; ++m) {
			int n = diag - m + 1;

			if (n == 0) {
				table[n][m] = 1;
			} else {
				table[n][m] = 0; //n - количество оставшихся блоков, m - размер нижележащего слоя

				for (int j = 1; j <= n; ++j) {
					int i = n - j;
					if (j < m) table[n][m] += table[i][j];
				}
			}
		}
	}
}

int main() {
	int blocks;

	cin >> blocks;

	int64_t **table = new int64_t*[blocks * 2 + 1];

	for (int i = 0; i < blocks * 2 + 1; ++i) {
		table[i] = new int64_t[blocks * 2 + 1];
	}

	fill_table(table, blocks);

	cout << (table[blocks][blocks] + 1) << endl;

	return 0;
}
