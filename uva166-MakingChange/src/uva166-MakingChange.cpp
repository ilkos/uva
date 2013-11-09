//============================================================================
// Name        : uva166-MakingChange.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <cassert>
#include <limits>
#include <map>
#include <iomanip>
using namespace std;

#define LEN(x) (sizeof((x)) / sizeof((x)[0]))
static const int coins[] = { 5, 10, 20, 50, 100, 200 };

int getMinimumNumberOfCoins(int amount) {
	int nCoins = 0;
	for (int i = LEN(coins) - 1; i >= 0 && amount > 0; --i) {
		while (amount >= coins[i]) {
			amount -= coins[i];
			++nCoins;
		}
	}
	assert(!amount);
	return nCoins;
}

map<pair<int, vector<int> >, int> A;

int solve(vector<int>& nCoins, int amount, size_t lo = 0) {
	int minCoins = numeric_limits<int>::max() / 2;

	if (amount < -coins[LEN(coins) - 1]) {
		return minCoins;
	}

	map<pair<int, vector<int> >, int>::const_iterator it = A.find(make_pair(amount, nCoins));
	if (it != A.end()) return it->second;

	if (amount <= 0) {
		minCoins = getMinimumNumberOfCoins(-amount);
	}

	for (size_t i = lo; i < nCoins.size(); ++i) {
		if (nCoins[i] > 0) {
			--nCoins[i];
			minCoins = min(minCoins, 1 + solve(nCoins, amount - coins[i], lo));
			++nCoins[i];
		}
	}

	return A[make_pair(amount, nCoins)] = minCoins;
}

int main() {
	while (true) {
		bool stop = true;

		vector<int> nCoins(6);
		for (int i = 0; i < 6; ++i) {
			cin >> nCoins[i];
			if (nCoins[i]) {
				stop = false;
			}
		}
		if (stop) break;

		double amount;
		cin >> amount;

		cout << right << setw(3) << solve(nCoins, (amount * 100) + 0.5) << endl;
	}
	return 0;
}
