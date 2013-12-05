//============================================================================
// Name        : uva11110-Equidivisions.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>
using namespace std;

#define LEN(x) (sizeof((x)) / sizeof((x)[0]))
pair<int, int> DIRECTIONS[] = { make_pair(1, 0), make_pair(-1, 0), make_pair(0, 1), make_pair(0, -1) };

void visit(const vector<vector<int> >& square, const int i, const int j,
		vector<vector<bool> >& isVisited) {
	if (isVisited[i][j]) return;

	isVisited[i][j] = true;
	for (size_t d = 0; d < LEN(DIRECTIONS); ++d) {
		int ni = i + DIRECTIONS[d].first;
		int nj = j + DIRECTIONS[d].second;

		if (ni < 0 || ni >= (int)square.size() || nj < 0 || nj >= (int)square.size()) {
			continue;
		}

		if (square[i][j] == square[ni][nj]) {
			visit(square, ni, nj, isVisited);
		}
	}
}

bool isGood(const vector<vector<int> >& square) {
	vector<vector<bool> > isVisited(square.size(), vector<bool>(square.size()));
	vector<bool> isSeen(square.size());

	for (size_t i = 0; i < square.size(); ++i) {
		for (size_t j = 0; j < square.size(); ++j) {
			if (isSeen[square[i][j]] && !isVisited[i][j]) return false;

			visit(square, i, j, isVisited);
			isSeen[square[i][j]] = true;
		}
	}

	return true;
}

int main() {
	int n;

	while (cin >> n >> ws) {
		if (!n) break;

		vector<vector<int> > square(n, vector<int>(n));
		string line;
		for (int i = 1; i < n; ++i) {
			getline(cin, line);
			istringstream is(line);
			int x, y;
			while (is >> x >> y) {
				--x; --y;
				square[x][y] = i;
			}
		}

		cout << (isGood(square) ? "good" : "wrong") << endl;
	}
	return 0;
}
