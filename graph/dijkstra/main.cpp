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
	Vertex(const T &xx) : x{xx}, pre{nullptr}, d{numeric_limits<int>::max()} { }
	T x;
	Vertex *pre;
	int d;
	vector<Vertex *> adjVertices;
	int heap_index{-1};
};

template<typename T>
class Graph {
public:
	Graph(void) : weights(16, h) { }

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

	bool addEdge(const T &x, const T &y, int weight) {
		auto it_x = adj.find(x);
		auto it_y = adj.find(y);

		if (it_x == adj.end() || it_y == adj.end())
			return false;

		it_x->second->adjVertices.push_back(it_y->second);
		weights.insert(make_pair(make_pair(x,y), weight));
		return true;
	}

	int getWeight(const T &x, const T &y) const {
		auto it = weights.find(make_pair(x, y));
		if (it == weights.end())
			return numeric_limits<int>::min();
		return it->second;
	}

	struct HeapNode {
		HeapNode(Vertex<T> *av = nullptr, int w = numeric_limits<int>::max()) : v(av), key(w) { }
		Vertex<T> *v;
		int key;
		bool operator<(const HeapNode &r) const noexcept {
			return key < r.key;
		}
		bool operator<=(const HeapNode &r) const noexcept {
			return key <= r.key;
		}
	};

	struct Heap {
		Heap(size_t s) : size(0), nodes(s) { }

		void push(Vertex<T> *v, int w = numeric_limits<int>::max()) {
			nodes[size++] = HeapNode(v, w);
			int index = size - 1;
			nodes[index].v->heap_index = index;
			v_set.insert(v->x);

			while (index) {
				int parent = index / 2;
				if (nodes[parent] <= nodes[index])
					break;

				swap(nodes[parent], nodes[index]);
				nodes[parent].v->heap_index = parent;
				nodes[index].v->heap_index = index;
				index = parent;
			}
		}

		HeapNode pop(void) {
			HeapNode r = nodes[0];
			v_set.erase(r.v->x);
			r.v->heap_index = -1;
			nodes[0] = nodes[--size];
			nodes[0].v->heap_index = 0;
			int index = 0;
			int lchild = index * 2 + 1;
			int rchild = index * 2 + 2;

			while (index < size / 2) {
				if (nodes[lchild] < nodes[index]) {
					swap(nodes[lchild], nodes[index]);
					nodes[lchild].v->heap_index = lchild;
					nodes[index].v->heap_index = index;
					index = lchild;
				} else if (rchild < size && nodes[rchild] < nodes[index]) {
					swap(nodes[rchild], nodes[index]);
					nodes[rchild].v->heap_index = rchild;
					nodes[index].v->heap_index = index;
					index = rchild;
				} else 
					break;
			}

			return r;
		}

		void decreaseKey(int index, int k) {
			if (index >= size)
				return;

			nodes[index].key = k;

			while (index) {
				int parent = index / 2;
				if (nodes[parent] <= nodes[index])
					break;

				swap(nodes[parent], nodes[index]);
				nodes[parent].v->heap_index = parent;
				nodes[index].v->heap_index = index;
				index = parent;
			}
		}

		bool empty(void) const noexcept {
			return size == 0;
		}

		bool contains(const T &t) const {
			return v_set.find(t) != v_set.end();
		}

		unsigned int size;
		vector<HeapNode> nodes;
		unordered_set<T> v_set;
	};

	void dijkstra(const T &t) {
		auto it = adj.find(t);
		if (it == adj.end())
			return;

		it->second->d = 0;

		Heap heap(adj.size());
		for (auto &v : adj) {
			if (v.first == t)
				heap.push(v.second, 0);
			else 
				heap.push(v.second);
		}

		while (!heap.empty()) {
			auto u = heap.pop();
			for (auto &v : u.v->adjVertices) {
				if (v->d > u.v->d + getWeight(u.v->x, v->x)) {
					v->d = u.v->d + getWeight(u.v->x, v->x);
					v->pre = u.v;
					heap.decreaseKey(v->heap_index, v->d);
				}
			}
		}
	}

	bool print_shortest_path(const T &s) const {
		auto v = adj.find(s);
		if (v->second->pre == nullptr) {
			cout << "No path to " << s << endl;
			return false;
		}

		print_shortest_path(v->second->pre->x);
		cout << s << " " << endl;
		return true;
	}

	template<typename U> friend ostream &operator<<(ostream &os, const Graph<U> &g) noexcept;

private:
	using VertexMap = unordered_map<T, Vertex<T> *>;
	VertexMap adj;

	function<size_t (const pair<T, T> &) noexcept> h { [] (const pair<T, T> &p) -> size_t { return hash<T>()(p.first) ^ hash<T>()(p.second); } };
	unordered_map<pair<T, T>, int, decltype(h)> weights;

};

template<typename T>
ostream &operator<<(ostream &os, const Graph<T> &g) noexcept {
	for (auto &v: g.adj) {
		os << "Vertex : " << v.first << endl;
		os << "Edges : " << endl;
		for (auto &e: v.second->adjVertices) {
			os << "\t[" << v.first << "," << e->x << "], weight " << g.getWeight(v.first, e->x) << endl;
		}
	}

	return os;
}

int main(int argc, char **argv) {
	Graph<string> g;
	g.addVertex(vector<string>{"s", "t", "x", "y", "z"});
	g.addEdge("s","t", 3);
	g.addEdge("s","y", 5);
	g.addEdge("t","y", 2);
	g.addEdge("y","t", 1);
	g.addEdge("t","x", 6);
	g.addEdge("y","x", 4);
	g.addEdge("y","z", 6);
	g.addEdge("x","z", 2);
	g.addEdge("z","x", 7);
	g.addEdge("z","s", 3);
	cout << g;

	g.dijkstra("s");
	g.print_shortest_path("z");

	return 0;
}
