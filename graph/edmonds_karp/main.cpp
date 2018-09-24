#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include <unordered_set>
#include <queue>
#include <cassert>

using namespace std;

template <typename T>
struct Vertex {
	Vertex(const T &xx) : x{xx}, pre(nullptr) { }

	struct Edge {
		Edge(Vertex<T> *u, unsigned int c) : v(u), capacity(c) { }
		Vertex<T> *v;
		unsigned int capacity;
	};

	unsigned int getCapacity(const T &x) const {
		auto it = adjVertices.find(x);
		if (it == adjVertices.end())
			return 0;

		return it->second.capacity;
	}

	void addEdge(Vertex<T> *u, unsigned int c) {
		adjVertices.insert(make_pair(u->x, Edge(u, c)));
	}

	void reduceCapacity(const T &t, unsigned int d) {
		auto e = adjVertices.find(t);
		if (e == adjVertices.end()) 
			return;

		e->second.capacity -= d;
	}

	void incrementCapacity(const T &t, unsigned int d) {
		auto e = adjVertices.find(t);
		if (e == adjVertices.end()) 
			return;

		e->second.capacity += d;
	}

	T x;
	Vertex<T> *pre;
	unordered_map<T, Edge> adjVertices;
};

template<typename T>
class Graph {
public:
	Graph(void) { }

	void addVertex(const T &x) {
		if (adj.find(x) != adj.end())
			return;

		auto v = new Vertex<T>(x);
		adj.insert(std::make_pair(x, v));
	}

	template<template <typename, typename...> class U>
	void addVertex(const U<T> &v) {
		for (auto &element: v)
			addVertex(element);
	}

	bool addEdge(const T &x, const T &y, unsigned int capacity) {
		auto it_x = adj.find(x);
		auto it_y = adj.find(y);

		if (it_x == adj.end() || it_y == adj.end())
			return false;

		it_x->second->addEdge(it_y->second, capacity);
		return true;
	}

	unsigned int getCapacity(const T &x, const T &y) const {
		auto it = adj.find(x);
		if (it == adj.end())
			return 0;

		return it->second->getCapacity(y);
	}

	unsigned int edmonds_karp(void) {
		Graph<T> rgraph;
		for (auto &v : adj)
			rgraph.addVertex(v.first);

		for (auto &v : adj) {
			auto rv = rgraph.adj.find(v.first)->second;
			for (auto &e : v.second->adjVertices) {
				auto u = rgraph.adj.find(e.second.v->x)->second;
				rv->addEdge(u, e.second.capacity);
				u->addEdge(rv, 0);
			}
		}

		unsigned int max_flow = 0;
		Vertex<T> *s = rgraph.adj["s"];
		Vertex<T> *t = rgraph.adj["t"];

		while (true) {
			unordered_set<T> visited;
			queue<Vertex<T> *> q;
			for (auto &v : rgraph.adj)
				v.second->pre = nullptr;

			q.push(s);
			visited.insert(s->x);
			bool done = false;
			while (!q.empty() && !done) {
				auto v = q.front();
				q.pop();
				for (auto &e  : v->adjVertices) {
					if (visited.count(e.second.v->x) ||
					    e.second.capacity == 0)
						continue;

					e.second.v->pre = v;
					if (e.second.v == t) {
						done = true;
						break;
					}

					visited.insert(e.first);
					q.push(e.second.v);
				}
			}

			if (t->pre == nullptr)
				break;

			unsigned int min_flow = numeric_limits<unsigned int>::max();
			auto parent = t->pre;
			auto child = t;
			while (parent) {
				min_flow = min(min_flow, parent->getCapacity(child->x));
				child = parent;
				parent = parent->pre;
			}

			parent = t->pre;
			child = t;
			while (parent) {
				parent->reduceCapacity(child->x, min_flow);
				child->incrementCapacity(parent->x, min_flow);
				child = parent;
				parent = parent->pre;
			}

			max_flow += min_flow;
		}

		return max_flow;
	}

	template<typename U> friend ostream &operator<<(ostream &os, const Graph<U> &g) noexcept;

private:
	using VertexMap = unordered_map<T, Vertex<T> *>;
	VertexMap adj;
};

template<typename T>
ostream &operator<<(ostream &os, const Graph<T> &g) noexcept {
	for (auto &v: g.adj) {
		os << "Vertex : " << v.first << endl;
		os << "Edges : " << endl;
		for (auto &e: v.second->adjVertices) {
			os << "\t[" << v.first << "," << e.second.v->x << "], weight " << g.getCapacity(v.first, e.second.v->x) << endl;
		}
	}

	return os;
}

int main(int argc, char **argv) {
	Graph<string> g;

	g.addVertex(vector<string>{"s", "t", "v1", "v2", "v3", "v4"});
	g.addEdge("s", "v1", 16);
	g.addEdge("s", "v2", 13);
	g.addEdge("v2", "v1", 4);
	g.addEdge("v1", "v3", 12);
	g.addEdge("v2", "v4", 14);
	g.addEdge("v3", "v2", 9);
	g.addEdge("v4", "v3", 7);
	g.addEdge("v3", "t", 20);
	g.addEdge("v4", "t", 4);
	cout << g;

	cout << "max flow: " << g.edmonds_karp() << endl;

	return 0;
}
