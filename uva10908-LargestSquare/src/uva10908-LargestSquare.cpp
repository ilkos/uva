//============================================================================
// Name        : uva10908-LargestSquare.cpp
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

template <typename T>
T min3(T one, T two, T three) {
	return min(one, min(two, three));
}

void precalc(const vector<vector<char> >& grid, vector<vector<int> >& mg) {
	// let mg[i][j] be the maximum square with lower right point at [i][j]
	// mg[i][j] would be 1 + min(upper, upper diagonal, left)
	for (size_t i = 0; i < mg.size(); ++i) {
		mg[i][0] = 1;
	}

	for (size_t i = 0; i < mg[0].size(); ++i) {
		mg[0][i] = 1;
	}

	for (size_t i = 1; i < grid.size(); ++i) {
		for (size_t j = 1; j < grid[i].size(); ++j) {
			mg[i][j] = 1 + min3(
					grid[i][j] == grid[i - 1][j] ? mg[i - 1][j] : 0,
					grid[i][j] == grid[i - 1][j - 1] ? mg[i - 1][j - 1] : 0,
					grid[i][j] == grid[i][j - 1] ? mg[i][j - 1] : 0);
		}
	}

#ifdef DEBUG
	for (size_t i = 0; i < mg.size(); ++i) {
		for (size_t j = 0; j < mg[i].size(); ++j) {
			cout << mg[i][j] << ' ';
		}
		cout << endl;
	}
#endif
}

int calc(const vector<vector<int> >& mg, const int r, const int c) {
	int ans = 1;
	for (int i = 1; i < mg[r][c] && r + i < mg.size() && c + i < mg[0].size(); ++i) {
		int next = mg[r + i][c + i];

		if (next < ans + 2) break;
		ans += 2;
	}
	return ans;
}

int main() {
	int t;
	cin >> t;
	while (t--) {
		int m, n, q;
		cin >> m >> n >> q;

		vector<vector<char> > grid(m, vector<char>(n));
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				cin >> grid[i][j];
			}
		}

		vector<pair<int, int> > rq(q);
		for (int i = 0; i < q; ++i) {
			cin >> rq[i].first >> rq[i].second;
		}

		cout << m << ' ' << n << ' ' << q << endl;
		if (!m || !n || !q) continue;

		vector<vector<int> > mg(m, vector<int>(n, -1));
		precalc(grid, mg);
		for (size_t i = 0; i < rq.size(); ++i) {
			cout << calc(mg, rq[i].first, rq[i].second) << endl;
		}
	}
	return 0;
}
