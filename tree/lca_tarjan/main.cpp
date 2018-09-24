#include <iostream>
#include <cmath>
#include <queue>
#include <stack>
#include <string>
#include <cassert>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Node {
	Node (const string &_x) noexcept : x(_x), left(NULL), right(NULL) {}

	~Node (void) noexcept {
		delete left;
		delete right;
	}

	bool addLeft(const string &v) noexcept {
		if (left != NULL) {
			return false;
		}

		left = new Node(v);
		return true;
	}

	bool addRight(const string &v) noexcept {
		if (right != NULL) {
			return false;
		}

		right = new Node(v);
		return true;
	}

	string x;
	Node *left;
	Node *right;
};

struct Set {
	Set(const Node *node) : parent(this), n(node), ancestor(node) {}
	Set *parent;
	const Node *n;
	const Node *ancestor;
};

unordered_map<string, Set *> sets;

Set *makeSet(const Node *n) {
	auto ss = new Set(n);
	sets[n->x] = ss;
	return ss;
}

Set *findSet(const string &s) {
	auto it = sets.find(s);
	if (it == sets.end())
		return nullptr;

	Set *c = it->second;
	while (c->parent != c)
		c = c->parent;
	it->second->parent = c;

	return c;
}

Set *findSet(Set *s) {
	Set *c = s;
	while (c->parent != c)
		c = c->parent;
	s->parent = c;
	return c;
}

void unionSet(const string &l, const string &r) {
	auto ls = findSet(l);
	auto rs = findSet(r);
	ls->parent = rs;
}

unordered_set<string> visited;
bool done = false;

const Node *
lca(const Node *root, const Node *p, const Node *q) {
	Set *u = makeSet(root);
	const Node *r = nullptr;

	if (root->left) {
		r = lca(root->left, p, q);
		if (r)
			return r;
		unionSet(root->x, root->left->x);
		findSet(u)->ancestor = u->n;
	}

	if (root->right) {
		r = lca(root->right, p, q);
		if (r)
			return r;
		unionSet(root->x, root->right->x);
		findSet(u)->ancestor = u->n;
	}

	visited.insert(root->x);
	const Node *v = nullptr;
	if (root == p)
		v = q;
	else if (root == q)
		v = p;
	else 
		return nullptr;

	if (visited.find(v->x) != visited.end()) {
		return findSet(v->x)->ancestor;
	}

	return nullptr;
}

int main (int argc, char **argv) {
	Node *root = new Node("F");
	root->addLeft("B");
	root->addRight("G");
	root->left->addLeft("A");
	root->left->addRight("D");
	root->left->right->addLeft("C");
	root->left->right->addRight("E");
	//root->right->addLeft("J");
	root->right->addRight("I");
	root->right->right->addLeft("H");

	const Node *p = root->left->left;
	const Node *q = root->right->right->left;
	const Node *a = lca(root, p, q);
	
	cout << "LCA of " << p->x << " and " << q->x << " is " << a->x << endl;

#if 0
	p = root->left->right->left;
	q = root->right->right;
	lca(root, p, q);

	p = root->left->right;
	q = root->left->right;
	lca(root, p, q);
#endif

	delete root;

	return 0;
}
