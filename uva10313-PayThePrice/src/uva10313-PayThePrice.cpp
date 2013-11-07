//============================================================================
// Name        : uva10313-PayThePrice.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

void countDistinct(int n, int numcoins, vector<vector<long long> >& output) {
	output = vector<vector<long long> >(n + 1, vector<long long>(numcoins + 1, 0));

	// let A[i][j] denote the number of ways we can get amount i in exactly j coins
	vector<vector<long long> > A (n + 1, vector<long long>(numcoins + 1, 0));

	A[0][0] = 1;

	// using coins [1, i] - introduce coin i
	// this must be the outer loop to avoid calculating duplicates
	// [e.g. (1, 2), (2, 1) -> two ways for getting 3 in 2 coins]
	for (int i = 1; i <= n; ++i) {
		for (int j = i; j <= n; ++j) { // for amount j
			for (int nCoins = 1; nCoins <= numcoins; ++nCoins) { // using nCoins coins
				A[j][nCoins] += A[j - i][nCoins - 1];
			}
		}
	}

	output[0][0] = A[0][0];
	for (int j = 0; j <= n; ++j) { // attain j
		for (int i = 1; i <= numcoins; ++i) { // using up to i coins
			output[j][i] = output[j][i - 1] + A[j][i];
		}
	}
}

int main() {
	string s;
	vector<vector<long long> > calc;
	countDistinct(300, 300, calc);

	while (getline(cin, s)) {
		istringstream is(s);

		vector<int> v;
		int tmp;
		while (is >> tmp) {
			v.push_back(tmp);
		}

		if (v.size() == 1) {
			cout << calc[v[0]][v[0]] << endl;
		}
		else if (v.size() == 2) {
			cout << calc[v[0]][min(v[1], v[0])] << endl;
		}
		else if (v.size() == 3) {
			cout << calc[v[0]][min(v[2], v[0])] - (v[1] > 0 ? calc[v[0]][v[1] - 1] : 0) << endl;
		}
	}

	return 0;
}
