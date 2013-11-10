//============================================================================
// Name        : uva11137-IngenuousCubrency.cpp
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

void calculate(int amount, vector<long long>& output) {
	output.resize(amount);
	output[0] = 1;

	for (int i = 1; i <= 21; ++i) {
		int cube = i * i * i; // introduce this

		for (int j = cube; j < amount; ++j) {
			output[j] += output[j - cube];
		}
	}
}

int main() {
	vector<long long> ways;
	calculate(10001, ways);
	int amount;
	while (cin >> amount) {
		cout << ways[amount] << endl;
	}
	return 0;
}
