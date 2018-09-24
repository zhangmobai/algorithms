#include <functional>
#include <iostream>
#include <list>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <queue>

using namespace std;

template <typename T>
struct Vertex {
	Vertex(const T &xx) : x(xx) { }
	T x;
};

template<typename T>
class Graph {
public:
	Graph(void) : adj(comp) { }

	void addVertex(const T &x) {
		Vertex<T> v(x);
		if (adj.find(&v) != adj.end())
			return;

		adj.insert(std::make_pair(new Vertex<T>(x), list<Vertex<T> *>()));
	}

	template<template <typename ...> class U>
	void addVertex(const U<T> &v) {
		for (auto &element: v)
			addVertex(element);
	}

	bool addEdge(const T &x, const T &y) {
		Vertex<T> vx(x);
		Vertex<T> vy(y);
		auto it_x = adj.find(&vx);
		auto it_y = adj.find(&vy);

		if (it_x == adj.end() || it_y == adj.end())
			return false;

		it_x->second.push_back(it_y->first);
		it_y->second.push_back(it_x->first);
		return true;
	}

	void bfs(const T &t) const {
		Vertex<T> v(t);
		auto it = adj.find(&v);
		if (it == adj.end())
			return;
		queue<Vertex<T> *> q;
		q.push(it->first);

		set<T> visited;
		while (!q.empty()) {
			auto c = q.front();
			q.pop();
			if (visited.count(c->x))
				continue;

			visited.insert(c->x);
			cout << c->x << " ";
			for (auto edge: adj.find(c)->second) {
				if (visited.find(edge->x) == visited.end())
					q.push(edge);
			}
		}
	}

	template<typename U> friend ostream &operator<<(ostream &os, const Graph<U> &g) noexcept;

private:
	function<bool (const Vertex<T> *l, const Vertex<T> *r)> comp{ [] (const Vertex<T> *l, const Vertex<T> *r) { return l->x < r->x; } };
	using VertexMap = map<Vertex<T> *, list<Vertex<T> *>, decltype(comp)>;

	VertexMap adj;
};

template<typename T>
ostream &operator<<(ostream &os, const Graph<T> &g) noexcept {
	for (auto &v: g.adj) {
		os << "Vertex : " << v.first->x << endl;
		os << "Edges : " << endl;
		for (auto &e: v.second) {
			os << "\t[" << v.first->x << "," << e->x << "]" << endl;
		}
	}

	return os;
}

int main(int argc, char **argv) {
	Graph<string> g;
	g.addVertex(list<string>{"r", "s", "t", "u", "v", "w", "x", "y"});
	g.addEdge("r","s");
	g.addEdge("r","v");
	g.addEdge("s","w");
	g.addEdge("w","t");
	g.addEdge("w","x");
	g.addEdge("t","x");
	g.addEdge("t","u");
	g.addEdge("u","x");
	g.addEdge("u","y");
	g.addEdge("x","y");
	cout << g;

	g.bfs("s");

	return 0;
}
