//============================================================================
// Name        : uva108-MaximumSum.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <limits>
using namespace std;

int maxsum(const vector<vector<int> >& array) {
	// prefix_sums[i][j] to contain the sum of array[0][j], array[1][j], ..., array[i][j]
	vector<vector<int> > prefix_sums(array.size(), vector<int>(array.size()));
	for (size_t i = 0; i < array[0].size(); ++i) {
		prefix_sums[0][i] = array[0][i];
	}

	for (size_t i = 1; i < array.size(); ++i) {
		for (size_t j = 0; j < array[i].size(); ++j) {
			prefix_sums[i][j] = prefix_sums[i - 1][j] + array[i][j];
		}
	}

	// vertical sum is now O(1), for every combination of rows run Kadane 1D
	int optimal_sum = numeric_limits<int>::min();

	for (size_t i = 0; i < array.size(); ++i) {
		for (size_t j = i + 1; j < array.size(); ++j) {
			// horizontal
			int current_sum = 0;
			for (size_t k = 0; k < array[i].size(); ++k) {
				current_sum += prefix_sums[j][k] - (i > 0 ? prefix_sums[i - 1][k] : 0);
				if (current_sum < 0) {
					current_sum = 0;
				}
				optimal_sum = max(optimal_sum, current_sum);
			}
		}
	}
	return optimal_sum;
}

int main() {
	int n;
	cin >> n;

	vector<vector<int> > array(n, vector<int>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cin >> array[i][j];
		}
	}

	cout << maxsum(array) << endl;
	return 0;
}
