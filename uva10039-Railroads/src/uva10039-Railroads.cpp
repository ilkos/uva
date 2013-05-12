//============================================================================
// Name        : uva10039-Railroads.cpp
//============================================================================

#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <cassert>
using namespace std;

//#define DEBUG

struct Cities {
	Cities() : currentCount(0) {}

	int addMapping(const string& city) {
		cities[city] = currentCount;
		citiesByNum[currentCount] = city;
		return currentCount++;
	}

	int getMapping(const string& city) const {
		map<string, int>::const_iterator it = cities.find(city);
		assert(it != cities.end());
		return it->second;
	}

	const string& getMapping(const int city) const {
		map<int, string>::const_iterator it = citiesByNum.find(city);
		assert(it != citiesByNum.end());
		return it->second;
	}

	bool hasMapping(const string& city) const {
		return cities.find(city) != cities.end();
	}

	int getNbr() const {
		return currentCount;
	}

private:
	int currentCount;
	map<string, int> cities;
	map<int, string> citiesByNum;
};

struct Vertex {
	Vertex() : isProcessed(false), parent(NULL) {}
	Vertex(const int city, const int time) : city(city), time(time), isProcessed(false), parent(NULL) {}

	void addAdjacent(Vertex& vertex, bool isDirected = false) {
		adjacent.push_back(&vertex);
#ifdef DEBUG
		cout << "Linking from " << city << " " << time << " to " << vertex.city << " " << vertex.time << endl;
#endif
		if (!isDirected) {
			vertex.addAdjacent(*this, true);
		}
	}

	vector<Vertex*> adjacent;

	int city;
	int time;

	bool isProcessed;
	Vertex* parent;
};

struct Graph {
	Graph(int nCities) :
		_graph(new multimap<int, Vertex>[nCities]()) {}

	~Graph() {
		delete[] _graph;
	}

	Vertex& addStop(const int city, const int time) {
		return _graph[city].insert(make_pair(time, Vertex(city, time)))->second;
	}

	multimap<int, Vertex>& getByCity(const int city) {
		return _graph[city];
	}

	bool containsCity(const int city) const {
		return !_graph[city].empty();
	}

	multimap<int, Vertex>* _graph;
};

struct EarlierTime : public binary_function<Vertex*, Vertex*, bool> {
	bool operator() (const Vertex* lhs, const Vertex* rhs) const {
		return lhs->time < rhs->time;
	}
};

Vertex* dfs(Vertex& vertex, const int destination, const int earliest) {
	stack<Vertex*> vertices;
	vertices.push(&vertex);
	vertex.isProcessed = true;

	priority_queue<Vertex*, vector<Vertex*>, EarlierTime> results;

	while (!vertices.empty()) {
		Vertex& current = *vertices.top();
		vertices.pop();

		if (current.city == destination && current.time >= earliest) {
			results.push(&current);
		}

		for (vector<Vertex*>::iterator it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
			if ((*it)->isProcessed) continue;

			(*it)->isProcessed = true;
			(*it)->parent = &current;
			vertices.push(*it);
		}
	}

	return results.empty() ? NULL : results.top();
}

const Vertex& getParent(const Vertex& vertex) {
	if (vertex.parent == NULL)
		return vertex;

	return getParent(*vertex.parent);
}

void solve(const Cities& cities, Graph& graph, int earliest, const string& src, const string& dst) {
	Vertex* srcVertex = NULL;

	if (cities.hasMapping(src) &&
			cities.hasMapping(dst) &&
			graph.containsCity(cities.getMapping(dst))) // if a train can get us there
	{
		multimap<int, Vertex>& dstTrains = graph.getByCity(cities.getMapping(dst));

		int processedTime;
		for (multimap<int, Vertex>::iterator it = dstTrains.begin();
				it != dstTrains.end();
				++it) {

			if (srcVertex && it->first > processedTime)
				break;

			processedTime = it->first;
			Vertex* solutionVertex = dfs(it->second, cities.getMapping(src), earliest);

			if (solutionVertex && (!srcVertex || srcVertex->time < solutionVertex->time))
				srcVertex = solutionVertex;
		}
	}
	if (srcVertex) {
		cout << "Departure " << setfill('0') << setw(4) << srcVertex->time << " " << cities.getMapping(srcVertex->city) << endl;
		const Vertex& lastVertex = getParent(*srcVertex);
		cout << "Arrival   " << setfill('0') << setw(4) << lastVertex.time << " " << cities.getMapping(lastVertex.city) << endl;
	}
	else
		cout << "No connection" << endl;
}

int main() {
	int nCases;
	cin >> nCases;

	for (int ccase = 0; ccase < nCases; ++ccase) {
		int c;
		cin >> c;

		Cities cities;
		{
			string city;
			for (int i = 0; i < c; ++i) { // just consume
				cin >> city;
				cities.addMapping(city);
			}
		}

		int t;
		cin >> t;

		Graph graph(cities.getNbr());
		for (int i = 0; i < t; ++i) {
			// train #i
			int nstops;
			cin >> nstops;

			// all vertices here will be connected
			Vertex* current = NULL;
			Vertex* prev = NULL;
			for (int j = 0; j < nstops; ++j) {
				int time;
				string city;
				cin >> time >> city;

				int cityn = cities.getMapping(city);

				prev = current;
				current = &graph.addStop(cityn, time);

				if (prev)
					current->addAdjacent(*prev, true);

				// same city
				multimap<int, Vertex>& sameCity = graph.getByCity(cityn);

				// links must be *directed* to respect time
				// only keep one as not to explode the size
				multimap<int, Vertex>::iterator smallerIt = sameCity.lower_bound(time);
				if (smallerIt != sameCity.begin()) {
					--smallerIt;

					pair<multimap<int, Vertex>::iterator, multimap<int, Vertex>::iterator > range;
					range = sameCity.equal_range(smallerIt->first);

					for (; range.first != range.second; ++range.first) {
						current->addAdjacent(range.first->second, true);
					}
				}

				multimap<int, Vertex>::iterator greaterIt = sameCity.upper_bound(time);
				if (greaterIt != sameCity.end()) {
					pair<multimap<int, Vertex>::iterator, multimap<int, Vertex>::iterator > range;
					range = sameCity.equal_range(greaterIt->first);

					for (; range.first != range.second; ++range.first) {
						range.first->second.addAdjacent(*current, true);
					}
				}
			}
		}

		// earliest time, src, dst
		int earliest;
		string sourceCity, dstCity;
		cin >> earliest >> sourceCity >> dstCity;

		cout << "Scenario " << ccase + 1 << endl;
		solve(cities, graph, earliest, sourceCity, dstCity);
		cout << endl;
	}
	return 0;
}
