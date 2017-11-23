#include <iostream>
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

class Iter {
public:
	Iter(const Node *root) : p(NULL) {
		const Node *c = root;
		while (c) {
			s.push(c);
			c = c->left;
		}
	} 

	bool hasNext() const {
		if (s.empty())
			return false;
		return true;
	}

	const Node *next() {
		if (hasNext() == false) {
			return NULL;
		}
		p = s.top();
		s.pop();
		if (p->right) {
			const Node *c = p->right;
			while (c) {
				s.push(c);
				c = c->left;
			}
		}
		return p;
	}
private:
	stack<const Node *> s;
	const Node *p;
};

int main (int argc, char **argv) {
	Node *root = new Node("F");
	root->addLeft("B");
	root->addRight("G");
	root->left->addLeft("A");
	root->left->addRight("D");
	root->left->right->addLeft("C");
	root->left->right->addRight("E");
	root->right->addRight("I");
	root->right->right->addLeft("H");

	Iter it(root);

	while (it.hasNext()) {
		cout << it.next()->x << " ";
	}

	cout << endl;
	delete root;

	return 0;
}
