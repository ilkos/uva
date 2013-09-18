//============================================================================
// Name        : uva10036-Divisibility.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <set>
using namespace std;

set<pair<int, int> > isNotDivisible;
bool solve(const vector<int>& numbers, const int k,
		const unsigned int curr, const int sum) {
	if (curr == numbers.size()) {
		return sum % k == 0;
	}

	if (isNotDivisible.find(make_pair(curr, sum)) != isNotDivisible.end()) {
		return false;
	}

	if (solve(numbers, k, curr + 1, (sum + numbers[curr]) % k) ||
			solve(numbers, k, curr + 1, (sum - numbers[curr]) % k)) {
		return true;
	}
	else {
		isNotDivisible.insert(make_pair(curr, sum));
		return false;
	}
}

int main() {
	int nCases;
	cin >> nCases;

	while (nCases--) {
		int n, k;
		cin >> n >> k;

		vector<int> numbers(n);
		for (int i = 0; i < n; ++i) {
			cin >> numbers[i];
		}

		isNotDivisible.clear();
		cout << (solve(numbers, k, 1, numbers[0]) ? "Divisible" : "Not divisible") << endl;
	}
	return 0;
}
