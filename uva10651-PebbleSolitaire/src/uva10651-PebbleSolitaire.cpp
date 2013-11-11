//============================================================================
// Name        : uva10651-PebbleSolitaire.cpp
//============================================================================

#include <iostream>
#include <cassert>
#include <bitset>
#include <map>
using namespace std;

map<unsigned long, int> cache;
int minimum(const bitset<12> board) {
	map<unsigned long, int>::const_iterator it = cache.find(board.to_ulong());
	if (it != cache.end()) {
		return it->second;
	}
	// for all available moves
	int res = board.count();
	for (int i = 0; i < 10; ++i) {
		if (!board[i] && board[i + 1] && board[i + 2]) {
			bitset<12> nb = board;
			nb[i] = true;
			nb[i + 1] = false;
			nb[i + 2] = false;
			res = min(res, minimum(nb));
		}
		else if (board[i] && board[i + 1] && !board[i + 2]) {
			bitset<12> nb = board;
			nb[i] = false;
			nb[i + 1] = false;
			nb[i + 2] = true;
			res = min(res, minimum(nb));
		}
	}

	return cache[board.to_ulong()] = res;
}

int main() {
	int n;
	cin >> n;

	while (n--) {
		string board;
		cin >> board;

		assert(board.size() == 12);
		bitset<12> b;
		for (size_t i = 0; i < board.size(); ++i) {
			b[i] = board[i] == 'o';
		}

		cache.clear();
		cout << minimum(b) << endl;
	}
	return 0;
}
