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

bool isBalancedHelp(const Node *root, unsigned int *depth) {
	if (root == NULL) {
		*depth = 0;
		return true;
	}

	unsigned int ld, rd;

	bool lb = isBalancedHelp(root->left, &ld);
	if (!lb)
		return false;

	bool rb = isBalancedHelp(root->right, &rd);
	if (!rb)
		return false;

	*depth = max(ld, rd) + 1;

	if (!lb || !rb)
		return false;

	if (abs((int)ld - (int)rd) < 2) {
		return true;
	}

	return false;
}

bool isBalanced(const Node *root) {
	unsigned int depth = 0;
	return isBalancedHelp(root, &depth);
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

	cout << "is balanced: " << isBalanced(root) << endl;

	delete root;

	return 0;
}
