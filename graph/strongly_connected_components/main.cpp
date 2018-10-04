#include <iostream>
#include <functional>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <vector>

using namespace std;

class Graph {
public:
	Graph() = default;
	Graph(initializer_list<char> l) {
		for (auto c : l)
			addVertex(c);
	}

	struct Vertex {
		Vertex(char xx) : x(xx) { }
		char x;
	};

	void addVertex(char x) {
		if (hasVertex(x))
			return;
		vertices[x] = new Vertex(x);
	}

	void addEdge(char x, char y) {
		addVertex(x);
		addVertex(y);
		auto yv = vertices[y];
		adjMap[x].push_back(yv);
	}

	void printSCC(void) const {
		unordered_set<char> visited;
		vector<const Vertex *> finished;
		for (const auto &v: vertices) {
			if (visited.count(v.first) == 1)
				continue;
			cout << "discovering " << v.first << endl;
			dfs(v.second, visited, finished, adjMap);
		}
		cout << "finished" << endl;
		for (auto v: finished) {
			cout << v->x << " ";
		}
		cout << endl;

		auto rg = revert();
		visited.clear();
		vector<const Vertex *> finished1;
		unsigned int n = 0;
		for (auto v = finished.rbegin(); v != finished.rend(); ++v) {
			if (visited.count((*v)->x) == 1)
				continue;
			dfs(*v, visited, finished1, rg);

			cout << "SCC " << ++n << endl;
			while (!finished1.empty()) {
				cout << finished1.back()->x << " ";
				finished1.pop_back();
			}
		}
	}
private:
	void dfs(const Vertex *v, unordered_set<char> &visited, vector<const Vertex *> &finished, const unordered_map<char, vector<Vertex *>> &m) const {
		visited.insert(v->x);
		auto it = m.find(v->x);

		if (it != m.end())
			for (auto vv: it->second)
				if (visited.count(vv->x) == 0)
					dfs(vv, visited, finished, m);

		finished.push_back(v);
	}

	unordered_map<char, vector<Vertex *>> revert(void) const {
		unordered_map<char, vector<Vertex *>> r;
		for (auto it : adjMap) {
			for (auto v: it.second) {
				r[v->x].push_back(vertices.find(it.first)->second);
			}
		}

		return r;
	}

	unordered_map<char, vector<Vertex *>> adjMap;
	map<char, Vertex *> vertices;

	bool hasVertex(char x) const {
		return vertices.find(x) != vertices.end();
	}
};

int main(int argc, char **argv) {
	Graph g{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

	g.addEdge('a', 'b');
	g.addEdge('b', 'c');
	g.addEdge('c', 'd');
	g.addEdge('d', 'c');
	g.addEdge('d', 'h');
	g.addEdge('h', 'd');
	g.addEdge('h', 'g');
	g.addEdge('c', 'g');
	g.addEdge('f', 'g');
	g.addEdge('g', 'f');
	g.addEdge('b', 'f');
	g.addEdge('b', 'e');
	g.addEdge('e', 'f');
	g.addEdge('e', 'a');

	g.printSCC();

	return 0;
}
