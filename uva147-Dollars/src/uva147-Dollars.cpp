//============================================================================
// Name        : uva147-Dollars.cpp
//============================================================================

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#define LEN(x) (sizeof((x)) / sizeof((x)[0]))
const int coins[] = { 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000 };

long long solve(const double a) {
	int amount = (double)((a * 100) + 0.5);

	// ways[i] representing the number of ways to reach exactly amount i
	vector<long long> ways(amount + 1, 0);
	ways[0] = 1;

	for (size_t i = 0; i < LEN(coins); ++i) { // introduce coin i
		for (int j = coins[i]; j <= amount; ++j) { // for amount j
			ways[j] += ways[j - coins[i]];
		}
	}

	return ways[amount];
}

int main() {
	double amount;

	while (cin >> amount) {
		if (amount == 0.0) break;

		cout << fixed << setprecision(2) << right << setw(6) << amount << setw(17) << solve(amount) << endl;
	}
	return 0;
}
