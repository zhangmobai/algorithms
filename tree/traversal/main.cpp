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

void inorder_recursive(const Node *root) {
	if (!root) {
		return;
	}

	inorder_recursive(root->left);
	cout << root->x << " ";
	inorder_recursive(root->right);
}

void inorder_iterative(const Node *root) {
	stack<const Node *> s;
	const Node *p = root;
	s.push(root);
	do {
		const Node *curr = p;
		while (curr->left != NULL) {
			curr = curr->left;
			s.push(curr);
		}

		const Node *n = s.top();
		while (s.empty() == false && n->right == NULL) {
			cout << n->x << " ";
			s.pop();
			if (s.empty() == false)
				n = s.top();
			else
				return;
		}

		cout << n->x << " ";
		
		if (n->right != NULL) {
			s.pop();
			p = n->right;
			s.push(p);
		}
	} while (!s.empty());
}

void preorder_recursive(const Node *root) {
	if (!root) {
		return;
	}

	cout << root->x << " ";
	preorder_recursive(root->left);
	preorder_recursive(root->right);
}

void preorder_iterative(const Node *root) {
	if (root == NULL) {
		return;
	}
	stack<const Node *> s;
	const Node *p = root;
	s.push(p);
	while (!s.empty()) {
		const Node *n = s.top();
		s.pop();
		if (n == NULL) {
			continue;
		}
		cout << n->x << " ";
		s.push(n->right);
		s.push(n->left);
	}
}

void postorder_recursive(const Node *root) {
	if (!root) {
		return;
	}

	postorder_recursive(root->left);
	postorder_recursive(root->right);
	cout << root->x << " ";
}

void postorder_iterative(const Node *root) {
	if (root == NULL) {
		return;
	}
	stack<const Node *> s;
	const Node *p = root;
	s.push(p);
	while (!s.empty()) {
		const Node *n = s.top();
		if (n->left == NULL && n->right == NULL) {
			do {
				s.pop();
				cout << n->x << " ";
				if (s.empty())
					break;
				const Node *m = s.top();
				if (m->right == n || m->left == n) {
					n = m;
				} else {
					break;
				}
			} while (true);
		} else {
			if (n->right)
				s.push(n->right);
			if (n->left)
				s.push(n->left);
		}
	}
}

void bfs(const Node *root) {
	queue<const Node *> q;
	const Node *p = root;
	q.push(p);
	while (!q.empty()) {
		p = q.front();
		q.pop();
		cout << p->x << " ";
		if (p->left)
			q.push(p->left);
		if (p->right)
			q.push(p->right);
	}
}

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

	cout << "recursive inorder traversal" << endl;
	inorder_recursive(root);
	cout << endl;
	cout << "iterative inorder traversal" << endl;
	inorder_iterative(root);
	cout << endl;

	cout << "\nrecursive preorder traversal" << endl;
	preorder_recursive(root);
	cout << endl;
	cout << "iterative preorder traversal" << endl;
	preorder_iterative(root);
	cout << endl;

	cout << "\nrecursive postorder traversal" << endl;
	postorder_recursive(root);
	cout << endl;
	cout << "iterative postorder traversal" << endl;
	postorder_iterative(root);
	cout << endl;

	cout << "\nbreadth first traversal" << endl;
	bfs(root);
	cout << endl;

	delete root;

	return 0;
}
