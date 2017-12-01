/*
 * This file implements the Aho-Corasick string search algorithm
 * It uses finite automaton to simultaneously search multiple patterns
 * For industrial grade code, see GNU grep source code kwset.c
 */

#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <stack>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>
#include <climits>

using namespace std;

struct State {
	State(int s, unsigned int d) : accept(false), depth(d), fail(NULL), state(s) { }

	~State(void) {
		for (auto v : edges)
			delete v.second;
	}

	bool accept;
	set<string> pats; // accepted patterns
	unsigned int depth;

	State *fail;
	map<char, State *> edges;
	int state; // debugging purpose
};

static void
print(State *trie)
{
	if (!trie)
		return;

	cout << "State: " << trie->state << " fail: " << (trie->fail ? trie->fail->state : -1) << " Depth: "
	     << trie->depth << " Accept: " << trie->accept << " Edges: ";

	for (const auto &it : trie->edges) {
		cout << "(" << it.first << ", " << it.second->state << ") ";
	}

	cout << " Output: ";
	for (const auto &w : trie->pats) {
		cout << w << ",";
	}

	cout << "\n";
	
	for (const auto &it : trie->edges) {
		print(it.second);
	}
}

static State *
build_trie(const vector<string> &pats)
{
	int state = 0;
	State *trie = new State(state++, 0);

	for (const auto &pat : pats) {
		State *t = trie;
		for (const auto c : pat) {
			State *&e = t->edges[c];

			if (!e)
				e = new State(state++, t->depth + 1);

			t = e;
		}

		t->accept = true;
		t->pats.insert(pat);
	}

	queue<State *> q;
	for (auto &v: trie->edges) {
		v.second->fail = trie;
		q.push(v.second);
	}

	while (!q.empty()) {
		State *s = q.front();
		q.pop();

		for (auto &v: s->edges) {
			q.push(v.second);

			State *f = s;
			do {
				f = f->fail;
				auto e = f->edges.find(v.first);
				if (e != f->edges.end()) {
					f = e->second;
					break;
				}
			} while (f->fail);

			v.second->fail = f;
			for (const auto &w : f->pats) {
				v.second->pats.insert(w);
			}
		}
	}
	
	return trie;
}

static void
acfind(const string &str, const vector<string> &pats)
{
	if (str.size() == 0 || pats.size() == 0)
		return;

	State *trie = build_trie(pats);
	print(trie);

	State *s = trie;
	for (const auto c : str) {
		while (true) {
			auto v = s->edges.find(c);

			if (v != s->edges.end()) {
				s = v->second;
				break;
			}

			if (!s->fail) {
				break;
			}

			s = s->fail;
		}

		if (s->accept) {
			for (const auto &w : s->pats) {
				cout << "Matched " << w << endl;
			}
		}
	}

	delete trie;
}

int
main (int argc, char **argv)
{
	vector<string> pat = {
		"he",
		"she",
		"his",
		"hers"
	};

	acfind("ushers", pat);
	//acfind("hishe", pat);

	return 0;
}
