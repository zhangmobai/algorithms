#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <cassert>

using namespace std;

template <typename T>
struct Vertex {
	Vertex(const T &xx) : x(xx) { }
	T x;
};

template<typename T>
class Graph {
public:
	Graph(void) : adj(comp), weights(16, h) { }

	void addVertex(const T &x) {
		Vertex<T> v(x);
		if (adj.find(&v) != adj.end())
			return;

		adj.insert(std::make_pair(new Vertex<T>(x), list<Vertex<T> *>()));
	}

	template<template <typename, typename...> class U>
	void addVertex(const U<T> &v) {
		for (auto &element: v)
			addVertex(element);
	}

	bool addEdge(const T &x, const T &y, int weight) {
		Vertex<T> vx(x);
		Vertex<T> vy(y);
		auto it_x = adj.find(&vx);
		auto it_y = adj.find(&vy);

		if (it_x == adj.end() || it_y == adj.end())
			return false;

		it_x->second.push_back(it_y->first);
		it_y->second.push_back(it_x->first);
		weights.insert(make_pair(make_pair(min(x,y), max(x,y)), weight));
		return true;
	}

	int getWeight(const T &x, const T &y) const {
		auto it = weights.find(make_pair(min(x, y), max(x, y)));
		if (it == weights.end())
			return numeric_limits<int>::min();
		return it->second;
	}

	vector<pair<T, T>> mst(void) const;

	template<typename U> friend ostream &operator<<(ostream &os, const Graph<U> &g) noexcept;

private:
	function<bool (const Vertex<T> *l, const Vertex<T> *r)> comp{ [] (const Vertex<T> *l, const Vertex<T> *r) { return l->x < r->x; } };
	using VertexMap = map<Vertex<T> *, list<Vertex<T> *>, decltype(comp)>;
	VertexMap adj;

	function<size_t (const pair<T, T> &) noexcept> h { [] (const pair<T, T> &p) -> size_t { return hash<T>()(p.first) ^ hash<T>()(p.second); } };
	unordered_map<pair<T, T>, int, decltype(h)> weights;

};

template<typename T>
struct UnionNode {
	UnionNode(const T &at) noexcept : t(at) { }
	T t;
	UnionNode *parent{nullptr};
};

template<typename T>
vector<pair<T, T>> Graph<T>::mst(void) const {
	vector<pair<T, T>> r;
	multimap<int, pair<T, T>> edges;
	map<T, UnionNode<T> *> nodes;

	for (auto &e: weights) {
		edges.insert(make_pair(e.second, e.first));
	}

	for (auto &v: adj) {
		nodes.insert(make_pair(v.first->x, new UnionNode<T>(v.first->x)));
	}

	for (auto &e: edges) {
		auto n1 = nodes.find(e.second.first);
		auto n2 = nodes.find(e.second.second);
		assert(n1 != nodes.end());
		assert(n2 != nodes.end());
		UnionNode<T> *un1 = n1->second;
		UnionNode<T> *un2 = n2->second;
		while (un1->parent)
			un1 = un1->parent;
		while (un2->parent)
			un2 = un2->parent;

		if (n1->second != un1)
			n1->second->parent = un1;
		if (n2->second != un2)
			n2->second->parent = un2;
		if (un1 == un2)
			continue;

		un1->parent = un2;
		n1->second->parent = un2;

		r.push_back(make_pair(e.second.first, e.second.second));
	}

	return r;
}

template<typename T>
ostream &operator<<(ostream &os, const Graph<T> &g) noexcept {
	for (auto &v: g.adj) {
		os << "Vertex : " << v.first->x << endl;
		os << "Edges : " << endl;
		for (auto &e: v.second) {
			os << "\t[" << v.first->x << "," << e->x << "], weight " << g.getWeight(v.first->x, e->x) << endl;
		}
	}

	return os;
}

int main(int argc, char **argv) {
	Graph<string> g;
	g.addVertex(vector<string>{"a", "b", "c", "d", "e", "f", "g", "h", "i"});
	g.addEdge("a","b", 4);
	g.addEdge("a","h", 8);
	g.addEdge("b","h", 11);
	g.addEdge("b","c", 8);
	g.addEdge("c","d", 7);
	g.addEdge("c","f", 4);
	g.addEdge("c","i", 2);
	g.addEdge("d","e", 9);
	g.addEdge("d","f", 14);
	g.addEdge("e","f", 10);
	g.addEdge("f","g", 2);
	g.addEdge("g","h", 1);
	g.addEdge("g","i", 6);
	g.addEdge("h","i", 7);
	cout << g;

	auto edges = g.mst();
	int total_weight = 0;
	for (auto &e: edges) {
		cout << "[" << e.first << ", " << e.second << "], " << g.getWeight(e.first, e.second) << "\n";
		total_weight += g.getWeight(e.first, e.second);
	}

	cout << "total weight: " << total_weight << endl;

	return 0;
}
