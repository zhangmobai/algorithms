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
	Vertex(const T &xx) : x(xx), pre(nullptr) { }
	T x;
	Vertex *pre;
	int heap_index{-1};
};

template<typename T>
class Graph {
public:
	Graph(void) : weights(16, h) { }

	void addVertex(const T &x) {
		if (adj.find(x) != adj.end())
			return;

		auto r = adj.insert(std::make_pair(x, vector<Vertex<T> *>()));
		r.first->second.push_back(new Vertex<T>(x));
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

		it_x->second.push_back(it_y->second[0]);
		it_y->second.push_back(it_x->second[0]);
		weights.insert(make_pair(make_pair(min(x,y), max(x,y)), weight));
		return true;
	}

	int getWeight(const T &x, const T &y) const {
		auto it = weights.find(make_pair(min(x, y), max(x, y)));
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

	void mst(void) {
		Heap heap(adj.size());
		auto it = adj.begin();
		auto v = it->second[0];
		heap.push(v, 0);

		++it;
		for (; it != adj.end(); ++it) {
			heap.push(it->second[0]);
		}

		unsigned int total_weight = 0;
		while (!heap.empty()) {
			auto u = heap.pop();
			if (u.v->pre) {
				cout << "[" << u.v->pre->x << ", " << u.v->x << "] " << getWeight(u.v->pre->x, u.v->x) << "\n";
				total_weight += getWeight(u.v->pre->x, u.v->x);
			}

			auto it = adj.find(u.v->x)->second.begin();
			++it;
			for (; it < adj.find(u.v->x)->second.end(); ++it) {
				if (heap.contains((*it)->x) && getWeight(u.v->x, (*it)->x) < heap.nodes[(*it)->heap_index].key) {
					heap.decreaseKey((*it)->heap_index, getWeight(u.v->x, (*it)->x));
					(*it)->pre = u.v;
				}
			}
		}

		cout << "total weigth: " << total_weight << endl;
	}

	template<typename U> friend ostream &operator<<(ostream &os, const Graph<U> &g) noexcept;

private:
	using VertexMap = map<T, vector<Vertex<T> *>>;
	VertexMap adj;

	function<size_t (const pair<T, T> &) noexcept> h { [] (const pair<T, T> &p) -> size_t { return hash<T>()(p.first) ^ hash<T>()(p.second); } };
	unordered_map<pair<T, T>, int, decltype(h)> weights;

};

template<typename T>
ostream &operator<<(ostream &os, const Graph<T> &g) noexcept {
	for (auto &v: g.adj) {
		os << "Vertex : " << v.first << endl;
		os << "Edges : " << endl;
		for (auto &e: v.second) {
			if (v.first != e->x)
				os << "\t[" << v.first << "," << e->x << "], weight " << g.getWeight(v.first, e->x) << endl;
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

	g.mst();

	return 0;
}
