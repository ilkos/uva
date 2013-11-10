//============================================================================
// Name        : uva562-DividingCoins.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
using namespace std;

int subset_sum(const vector<int>& elements, int desired) {
	// A[i][t] to indicate whether we can attain amount t using elements [0, i)
	bool A[elements.size() + 1][desired + 1];
	memset(A, 0, sizeof(A));
	for (int i = 0; i < (int) elements.size() + 1; ++i) {
		A[i][0] = true;
	}

	int closest = 0;
	for (int i = 1; i < (int) elements.size() + 1; ++i) { // introduce element i - 1
		for (int j = 0; j < desired + 1; ++j) {
			A[i][j] = A[i - 1][j] || (j >= elements[i - 1] ? A[i - 1][j - elements[i - 1]] : false);
			if (A[i][j] && j > closest) {
				closest = j;
			}
		}
	}

	return closest;
}

int main() {
	int nProblems;
	cin >> nProblems;

	while (nProblems--) {
		int m;
		cin >> m;

		vector<int> coins(m);
		int total = 0;
		for (int i = 0; i < m; ++i) {
			cin >> coins[i];
			total += coins[i];
		}

		int closest = subset_sum(coins, total / 2);
		cout << abs(2 * closest - total) << endl;
	}
	return 0;
}
