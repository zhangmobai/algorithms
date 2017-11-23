#include <iostream>
#include <stack>
#include <cassert>

using namespace std;

class IntervalTreeNode {
public:
	IntervalTreeNode(int s, int e) : start(s), end(e), left(NULL), right(NULL) {
		assert(s <= e);
		max_end = e;
	}

	void updateMax(void) {
		max_end = end;
		if (left)
			max_end = max(max_end, left->max_end);
		if (right)
			max_end = max(max_end, right->max_end);
	}

	~IntervalTreeNode(void) noexcept {
		delete left;
		left = NULL;
		delete right;
		right = NULL;
	}

	int start;
	int end;
	int max_end;
	IntervalTreeNode *left;
	IntervalTreeNode *right;
};

class IntervalTree {
public:
	IntervalTree(void) noexcept : root(NULL) { }

	const IntervalTreeNode *insert(int s, int e) {
		if (root == NULL) {
			root = new IntervalTreeNode(s, e);
			return root;
		}

		IntervalTreeNode *p = root;

		for (;;) {
			if (p->start == s && p->end == e) {
				return p;
			}

			p->max_end = max(p->max_end, e);
			if (s < p->start) {
				if (p->left == NULL) {
					p->left = new IntervalTreeNode(s, e);
					return p->left;
				} else {
					p = p->left;
					continue;
				}
			} else {
				if (p->right == NULL) {
					p->right = new IntervalTreeNode(s, e);
					return p->right;
				} else {
					p = p->right;
					continue;
				}
			}
		}
	}

	bool remove(int s, int e) {
		IntervalTreeNode *p = root;

		stack<IntervalTreeNode *> ancestors;
		stack<IntervalTreeNode *> ancestors1;
		bool r = false;

		for (;;) {
			if (p == NULL) {
				break;
			}

			if (p->max_end < e) {
				break;
			}

			if (p->start == s && p->end == e) {
				r = true;
				if (p->left == NULL && p->right == NULL) {
					delete p;
					p = NULL;
				}

				if (p->left == NULL) {
					delete p;
					p = p->right;
				}

				if (p->right == NULL) {
					delete p;
					p = p->left;
				}

				IntervalTreeNode *pp = p;
				IntervalTreeNode *q = p->right;
				while (q->left != NULL) {
					ancestors1.push(pp);
					pp = q;
					q = q->left;
				}

				if (pp != p)
					pp->left = q->right;

				while (!ancestors1.empty()) {
					IntervalTreeNode *curr = ancestors1.top();
					curr->updateMax();
					ancestors1.pop();
				}

				if (ancestors.empty() == false) {
					IntervalTreeNode *curr = ancestors.top();
					if (curr->left == p) {
						curr->left = q;
					} else {
						curr->right = q;
					}
				}

				q->left = p->left;

				if (p->right != q)
					q->right = p->right;

				q->updateMax();
				while (ancestors.empty() == false) {
					IntervalTreeNode *curr = ancestors.top();
					curr->updateMax();
					ancestors.pop();
				}

				break;
			} 

			ancestors.push(p);

			if (s < p->start) {
				p = p->left;
			} else {
				p = p->right;
			}
		}

		return r;
	}

	void findHelper(int s, int e, const IntervalTreeNode *r) const {
		if (r == NULL) {
			return;
		}

		const IntervalTreeNode *p = r;
		if (p->max_end < s) {
			return;
		}

		bool overlap = !(p->start > e || p->end < s);
		if (overlap) {
			cout << "[" << p->start << ", " << p->end << "] ";
		}
		
		findHelper(s, e, p->left);

		if (e >= p->start) {
			findHelper(s, e, p->right);
		}
	}

	void find(int s, int e) const {
		findHelper(s, e, root);
	}

	IntervalTreeNode *root;
};

int main (int argc, char **argv) {

	IntervalTree tree;
	tree.insert(10, 15);
	tree.insert(5, 100);
	tree.insert(4, 6);
	tree.insert(7, 150);
	tree.insert(20, 22);
	tree.insert(14, 16);
	tree.insert(21, 28);

	tree.find(50, 52);

	tree.remove(5, 70);
	tree.find(50, 52);

	tree.remove(5, 100);
	tree.find(50, 52);

	return 0;
}
