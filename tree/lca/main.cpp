#include <iostream>
#include <cmath>
#include <queue>
#include <stack>
#include <string>
#include <cassert>

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

void
pot(const Node *n, const Node *p, const Node *q, const Node **pp, const Node **qq)
{
	if (*pp != NULL)
		p = NULL;
	if (*qq != NULL)
		q = NULL;

	const Node *_pp = NULL, *_qq = NULL;

	if (n == p) {
		_pp = n;
		p = NULL;
	} else if (n == q) {
		_qq = n;
		q = NULL;
	}

	if (n->left) {
		pot(n->left, p, q, &_pp, &_qq);
	}

	if (n->right) {
		pot(n->right, p, q, &_pp, &_qq);
	}

	if (_pp != NULL && _qq != NULL) {
		if (_pp == _qq) {
			*pp = *qq = _pp;
			return;
		}

		*pp = *qq = n;
		return;
	}

	if (_pp != NULL)
		*pp = n;
	else if (_qq != NULL)
		*qq = n;
}

const Node *
lca(const Node *root, const Node *p, const Node *q) {
	if (root == NULL)
		return NULL;

	if (p == q)
		return p;

	const Node *pp = NULL, *qq = NULL;

	pot(root, p, q, &pp, &qq);

	if (pp != qq)
		return NULL;

	return pp;
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
	const Node *q = root->left->right->right;
	const Node *a = lca(root, p, q);
	if (a == NULL) {
		cout << "LCA of " << p->x << " and " << q->x << " is NULL" << endl;
	} else {
		cout << "LCA of " << p->x << " and " << q->x << " is " << a->x << endl;
	}

	p = root->left->right->left;
	q = root->right->right;
	a = lca(root, p, q);
	if (a == NULL) {
		cout << "LCA of " << p->x << " and " << q->x << " is NULL" << endl;
	} else {
		cout << "LCA of " << p->x << " and " << q->x << " is " << a->x << endl;
	}

	p = root->left->right;
	q = root->left->right;
	a = lca(root, p, q);
	if (a == NULL) {
		cout << "LCA of " << p->x << " and " << q->x << " is NULL" << endl;
	} else {
		cout << "LCA of " << p->x << " and " << q->x << " is " << a->x << endl;
	}

	delete root;

	return 0;
}
