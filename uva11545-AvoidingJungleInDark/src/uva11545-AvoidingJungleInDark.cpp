//============================================================================
// Name        : uva11545-AvoidingJungleInDark.cpp
//============================================================================

#include <iostream>
#include <cassert>
#include <queue>
#include <cstring>
using namespace std;

struct State {
	State rest() const {
		State n;
		n.time = time + 8;
		n.pos = pos;
		n.timeSinceLastStop = 0;
		return n;
	}

	State move() const {
		State n;
		n.time = time + 1;
		n.pos = pos + 1;
		n.timeSinceLastStop = timeSinceLastStop + 1;
		return n;
	}

	int time;
	int pos;
	int timeSinceLastStop;
};

int solve(const string& path, int pos) {
	bool isVisited[path.size()][24];
	memset(isVisited, 0, sizeof(isVisited));

	queue<State> q;

	State initial;
	initial.time = 0;
	initial.pos = pos;
	q.push(initial);
	isVisited[pos][0] = true;

	while (!q.empty()) {
		State current = q.front();
		q.pop();

		if (path[current.pos] == 'D') {
			return current.time;
		}

		if (path[current.pos] == '*' &&
				(current.time % 24) >= 12) continue;

		if (current.timeSinceLastStop == 16) {
			q.push(current.rest());
			continue;
		}

		State ms = current.move();
		if (!isVisited[ms.pos][ms.time % 24] && (path[ms.pos] != '*' || (current.time % 24) < 12)) {
			q.push(ms);
			isVisited[ms.pos][ms.time % 24] = true;
		}

		State rs = current.rest();
		if (!isVisited[rs.pos][rs.time % 24]) {
			q.push(rs);
			isVisited[rs.pos][rs.time % 24] = true;
		}
	}

	return -1;
}

int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; ++i) {
		string line;
		cin >> line;

		cout << "Case #" << i + 1 << ": " << solve(line, line.find('S')) << endl;
	}
	return 0;
}
