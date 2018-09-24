#include <iostream>
#include <queue>
#include <stack>
#include <string>

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

void inorder_recursive(const Node &root) {
	if (root.left)
		inorder_recursive(*root.left);
	cout << root.x << " ";
	if (root.right)
		inorder_recursive(*root.right);
}

void inorder_iterative(const Node &root) {
	stack<const Node *> st;
	st.push(&root);
	auto p = st.top();

	while (!st.empty()) {
		while (p->left) {
			st.push(p->left);
			p = p->left;
		}

		do {
			p = st.top();
			cout << p->x << " ";
			st.pop();
		} while (!st.empty() && p->right == nullptr);

		if (p->right) {
			p = p->right;
			st.push(p);
		}
	}
}

void preorder_recursive(const Node &root) {
	cout << root.x << " ";
	if (root.left)
		preorder_recursive(*root.left);
	if (root.right)
		preorder_recursive(*root.right);
}

void preorder_iterative(const Node &root) {
	stack<const Node *> st;
	st.push(&root);
	while (!st.empty()) {
		auto p = st.top();
		cout << p->x << " ";
		st.pop();
		if (p->right)
			st.push(p->right);
		if (p->left)
			st.push(p->left);
	}
}

void postorder_recursive(const Node &root) {
	if (root.left)
		postorder_recursive(*root.left);
	if (root.right)
		postorder_recursive(*root.right);
	cout << root.x << " ";
}

void postorder_iterative(const Node &root) {
	stack<const Node *> st;
	st.push(&root);
	auto p = st.top();
	
	while (!st.empty()) {
		while (p->left) {
			p = p->left;
			st.push(p);
		}

		const Node *q = nullptr;
		while (!st.empty()) {
			p = st.top();
			if (p->right && p->right != q)
				break;
			cout << p->x << " ";
			q = p;
			st.pop();
		}

		if (st.empty())
			break;

		if (p->right && p->right != q) {
			p = p->right;
			st.push(p);
		}
	}
}

void bfs(const Node &root) {
	queue<const Node *> q;
	q.push(&root);
	do {
		auto p = q.front();
		cout << p->x << " ";
		q.pop();
		if (p->left)
			q.push(p->left);
		if (p->right)
			q.push(p->right);
	} while (!q.empty());
}

int main (int argc, char **argv) {
	Node root("F");
	root.addLeft("B");
	root.addRight("G");
	root.left->addLeft("A");
	root.left->addRight("D");
	root.left->right->addLeft("C");
	root.left->right->addRight("E");
	root.right->addRight("I");
	root.right->right->addLeft("H");

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

	return 0;
}
