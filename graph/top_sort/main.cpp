#include <functional>
#include <iostream>
#include <list>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <set>

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
		return true;
	}


	list<const Vertex<T> *> topsort(void) const {
		list<const Vertex<T> *> l;
		set<T> visited;
		for (const auto &v: adj)
			topsortHelper(visited, l, v.first);

		return l;
	}

	template<typename U> friend ostream &operator<<(ostream &os, const Graph<U> &g) noexcept;

private:
	function<bool (const Vertex<T> *l, const Vertex<T> *r)> comp{ [] (const Vertex<T> *l, const Vertex<T> *r) { return l->x < r->x; } };
	using VertexMap = map<Vertex<T> *, list<Vertex<T> *>, decltype(comp)>;

	void topsortHelper(set<T> &visited, list<const Vertex<T> *> &l, const typename VertexMap::key_type &v) const {
		if (visited.count(v->x))
			return;

		typename VertexMap::const_iterator it = adj.find(v);
		if (it == adj.end())
			return;

		visited.insert(v->x);

		for (const auto &e : it->second)
			topsortHelper(visited, l, e);

		l.push_front(v);
	}

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
	Graph<int> g;
	g.addVertex(5);
	g.addVertex(list<int>{7,3,8,11,2,9,10});
	g.addEdge(5,11);
	g.addEdge(7,11);
	g.addEdge(7,8);
	g.addEdge(3,8);
	g.addEdge(3,10);
	g.addEdge(11,2);
	g.addEdge(11,9);
	g.addEdge(11,10);
	g.addEdge(8,9);
	cout << g;

	auto l = g.topsort();
	for (auto v: l)
		cout << v->x << " ";
	cout << endl;
	return 0;
}
