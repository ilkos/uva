//============================================================================
// Name        : uva10360-RatAttack.cpp
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

struct Population {
	int x, y, sz;
};

#define MAXSZ 1025

void solve(int d, const vector<Population>& pops) {
	vector<vector<int> > A (MAXSZ, vector<int>(MAXSZ));

	for (size_t i = 0; i < pops.size(); ++i) {
		for (int dx = max(pops[i].x - d, 0); dx <= min(pops[i].x + d, MAXSZ - 1); ++dx) {
			for (int dy = max(pops[i].y - d, 0); dy <= min(pops[i].y + d, MAXSZ - 1); ++dy) {
				A[dx][dy] += pops[i].sz;
			}
		}
	}

	int best = 0;
	int best_x = 0;
	int best_y = 0;
	for (int i = 0; i < MAXSZ; ++i) {
		for (int j = 0; j < MAXSZ; ++j) {
			if (A[i][j] > best) {
				best = A[i][j];
				best_x = i;
				best_y = j;
			}
		}
	}

	cout << best_x << ' ' << best_y << ' ' << best << endl;
}

int main() {
	int nCases;
	cin >> nCases;

	while (nCases--) {
		int d, n;
		cin >> d >> n;

		vector<Population> pops(n);
		for (int i = 0; i < n; ++i) {
			cin >> pops[i].x >> pops[i].y >> pops[i].sz;
		}

		solve(d, pops);
	}
	return 0;
}
