//============================================================================
// Name        : uva10717-Mint.cpp
//============================================================================

#include <iostream>
#include <cassert>
#include <cmath>
#include <map>
#include <vector>
#include <limits>
using namespace std;

int gcd(int a, int b) {
	if (!b) return a;
	return gcd(b, a % b);
}

inline int lcm(int a, int b) {
	return a * b / gcd(a, b);
}

void st(const int height, const vector<int>& coins) {
	int rmax = numeric_limits<int>::max();
	int rmin = 0;

	for (size_t one = 0; one < coins.size(); ++one) {
		for (size_t two = one + 1; two < coins.size(); ++two) {
			for (size_t three = two + 1; three < coins.size(); ++three) {
				for (size_t four = three + 1; four < coins.size(); ++four) {
					int tlcm = lcm(lcm(coins[one], coins[two]), lcm(coins[three], coins[four]));

					rmin = max(rmin, height - height % tlcm);
					rmax = min(rmax, tlcm * (int)(ceil(height / (double)tlcm)));
				}
			}
		}
	}

	cout << rmin << ' ' << rmax << endl;
}

void solve(const vector<int>& coins, const vector<int>& tables) {
	for (size_t i = 0; i < tables.size(); ++i) {
		st(tables[i], coins);
	}
}

int main() {
	int n, t;
	while (cin >> n >> t) {
		if (!n && !t) break;

		vector<int> coins(n);
		for (int i = 0; i < n; ++i) {
			cin >> coins[i];
		}

		vector<int> tables(t);
		for (int i = 0; i < t; ++i) {
			cin >> tables[i];
		}

		solve(coins, tables);
	}
	return 0;
}
