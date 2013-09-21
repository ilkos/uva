//============================================================================
// Name        : uva10052-InvitingPoliticians.cpp
//               split graph in 4 independent sets (4-colour)
//============================================================================

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
using namespace std;

struct AdjList {
	AdjList(int n) :
		_size(n),
		_edges(n, vector<int>()) {}

	void addEdge(const int src, const int dst) {
		_edges[src].push_back(dst);
		_edges[dst].push_back(src);
	}

	const vector<int>& getEdges(int node) const {
		return _edges[node];
	}

	int getSize() const {
		return _size;
	}

private:
	int _size;
	vector<vector<int> > _edges;
};

enum Colour { UNKNOWN = 0, RED = 1, GREEN = 2, BLUE = 4, BLACK = 8 };
const int AllColours = (RED | GREEN | BLUE | BLACK);

int getAdjacentColours(const vector<int>& edges, const vector<Colour>& colouring) {
	int colours = 0;

	for (vector<int>::const_iterator edgeIt(edges.begin()), edgeItEnd(edges.end());
			edgeIt != edgeItEnd && colours != AllColours;
			++edgeIt) {
		colours |= colouring[*edgeIt];
	}

	return colours;
}

int countBits(int n) {
	int res = 0;
	while (n) {
		++res;
		n &= n - 1;
	}
	return res;
}

bool colour(int src, const AdjList& graph, vector<Colour>& colouring) {
	const vector<Colour> backup = colouring;

	const vector<int>& edges = graph.getEdges(src);
	const int colourCandidates = AllColours & ~getAdjacentColours(edges, colouring);

	bool hardRefresh = false;

	for (int c = RED; c <= BLACK; c <<= 1) {
		if (!(c & colourCandidates)) continue;

		colouring[src] = (Colour) c;
		bool success = true;

		set<pair<int, int>, greater<pair<int, int> > > toColour;
		for (vector<int>::const_iterator edgeIt(edges.begin()), edgeItEnd(edges.end());
				edgeIt != edgeItEnd;
				++edgeIt) {
			if (colouring[*edgeIt] == UNKNOWN) {
				toColour.insert(make_pair(
						countBits(getAdjacentColours(graph.getEdges(*edgeIt), colouring)),
						*edgeIt));
			}
		}

		for (set<pair<int, int> >::const_iterator toColourIt(toColour.begin()), toColourItEnd(toColour.end());
				toColourIt != toColourItEnd;
				++toColourIt) {
			if (colouring[toColourIt->second] == UNKNOWN) {
				if (!colour(toColourIt->second, graph, colouring)) {
					success = false;
					break;
				}
				else {
					hardRefresh = true; // managed to colour a branch
				}
			}
		}

		if (success) return true;
		colouring[src] = UNKNOWN;

		if (hardRefresh) {
			colouring = backup;
			hardRefresh = false;
		}
	}

	return false;
}

void solve(const AdjList& graph, const vector<string>& names) {
	vector<Colour> colouring(graph.getSize());
	fill(colouring.begin(), colouring.end(), UNKNOWN);

	for (int i = 0; i < graph.getSize(); ++i) {
		if (colouring[i] == UNKNOWN) colour(i, graph, colouring);
	}

	vector<vector<string> > chunks(4, vector<string>());
	for (int i = 0; i < graph.getSize(); ++i) {
		if (colouring[i] == RED) {
			chunks[0].push_back(names[i]);
		}
		else if (colouring[i] == GREEN) {
			chunks[1].push_back(names[i]);
		}
		else if (colouring[i] == BLUE) {
			chunks[2].push_back(names[i]);
		}
		else {
			chunks[3].push_back(names[i]);
		}
	}

	for (int i = 0; i < 4; ++i) {
		cout << chunks[i].size() << endl;
		for (vector<string>::const_iterator it = chunks[i].begin(), itEnd = chunks[i].end();
				it != itEnd;
				++it) {
			if (it != chunks[i].begin()) cout << ' ';
			cout << *it;
		}
		cout << endl;
	}
}

int main() {
	int nCases;
	cin >> nCases;

	for (int cCase = 1; cCase <= nCases; ++cCase) {
		int numPoliticians, numEdges;
		cin >> numPoliticians >> numEdges;

		vector<string> names(numPoliticians);
		map<string, int> nameToNode;
		for (int i = 0; i < numPoliticians; ++i) {
			cin >> names[i];
			nameToNode[names[i]] = i;
		}

		AdjList graph(numPoliticians);
		for (int i = 0; i < numEdges; ++i) {
			string n1, n2;
			cin >> n1 >> n2;

			graph.addEdge(nameToNode[n1], nameToNode[n2]);
		}

		if (cCase != 1) cout << endl;
		cout << "Case #" << cCase << endl;
		solve(graph, names);
	}
	return 0;
}
