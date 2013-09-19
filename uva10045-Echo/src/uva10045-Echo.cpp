//============================================================================
// Name        : uva10045-Echo.cpp
//============================================================================

#include <iostream>
#include <deque>
#include <map>
using namespace std;

string toString(const deque<char>& q) {
	string result(q.size(), '0');
	std::copy(q.begin(), q.end(), result.begin());
	return result;
}

enum Solution { INVALID = 1, UNKNOWN = 2, VALID = 4 };
map<pair<string, int>, int> evaluated;

// start by assuming the solution invalid
// upgrade to unknown / valid
int solve(const string& str, deque<char>& q, const unsigned int curr) {
	if (curr == str.size()) {
		return q.empty() ? VALID : UNKNOWN;
	}

	map<pair<string, int>, int>::const_iterator eIt = evaluated.find(make_pair(toString(q), curr));
	if (eIt != evaluated.end()) {
		return eIt->second;
	}

	int result = INVALID;

	// we have two options
	// consume (interpret as echo)
	if (!q.empty() && str[curr] == q.front()) {
		const char c = q.front();
		q.pop_front();

		int s = solve(str, q, curr + 1);
		if (s == VALID) {
			return s;
		}

		result |= s;
		q.push_front(c);
	}

	// interpret as buffer
	if (q.size() < 10) {
		q.push_back(str[curr]);

		int s = solve(str, q, curr + 1);
		if (s == VALID) {
			return s;
		}

		result |= s;
		q.pop_back();
	}

	return evaluated[make_pair(toString(q), curr)] = result;
}

int main() {
	int nCases;
	cin >> nCases;

	string line;
	while (nCases--) {
		cin >> line;

		evaluated.clear();

		deque<char> q;
		q.push_back(line[0]);
		int s = solve(line, q, 1);

		if (s & VALID) {
			cout << "An echo string with buffer size ten" << endl;
		}
		else if (s & UNKNOWN) {
			cout << "Not an echo string, but still consistent with the theory" << endl;
		}
		else {
			cout << "Not consistent with the theory" << endl;
		}
	}
	return 0;
}
