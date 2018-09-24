#include <iostream>
#include <memory>
#include <list>

using namespace std;

class Node {
	public:
		explicit Node(int x) noexcept : n(x) { }

		bool depends_on(Node *p, list<Node *> &l) noexcept {
			if (p == this) {
				cout << "depends on self, wrong" << endl;
				return false;
			}

			children.push_back(p);

			if (p->num_dependencies++ == 0) {
				l.remove(p);
				l.push_back(p);
			}

			return true;
		}

		Node *pop(void) noexcept {
			Node *r{nullptr};

			for (auto p : children) {
				if (--p->num_dependencies == 0)
					r = p;
			}

			return r;
		}

		int get(void) const noexcept {
			return n;
		}

		int get_num_dependencies(void) const noexcept {
			return num_dependencies;
		}

	private:
		int n;
		int num_dependencies = 0;
		list<Node *> children;
};

bool topsort(list<Node *> &l) {
	Node *p = l.front();
	list<Node *> r;

	while (!l.empty()) {
		if (p->get_num_dependencies() > 0) {
			cout << "Graph contains loop" << endl;
			return false;
		}
		
		r.push_back(p);
		l.remove(p);
		p = p->pop();
		if (p == NULL)
			p = l.front();
	}

	for (auto q : r)
		cout << q->get() << " ";
	cout << endl;

	return true;
}

int main(int argc, char **argv) {
	list<Node *> l;
	Node n5{5};
	Node n11{11};
	Node n2{2};
	Node n3{3};
	Node n9{9};
	Node n8{8};
	Node n7{7};
	Node n10{10};
	Node n12{12};
	l.push_back(&n11);
	l.push_back(&n2);
	l.push_back(&n9);
	l.push_back(&n8);
	l.push_back(&n3);
	l.push_back(&n5);
	l.push_back(&n12);
	l.push_back(&n7);
	l.push_back(&n10);

	n5.depends_on(&n11, l);
	n11.depends_on(&n2, l);
	n11.depends_on(&n9, l);
	n11.depends_on(&n10, l);
	n7.depends_on(&n11, l);
	n7.depends_on(&n8, l);
	n8.depends_on(&n9, l);
	n3.depends_on(&n8, l);
	n3.depends_on(&n10, l);
	//n10.depends_on(&n5, l);
	n12.depends_on(&n12, l);

	topsort(l);
	return 0;
}
