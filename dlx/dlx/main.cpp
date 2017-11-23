#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>

using namespace std;

struct node {
	node *r;
	node *l;
	node *d;
	node *u;

	node *c;

	unsigned int row;

	bool is_column;
	unsigned int column;
	string name;
	unsigned int size;

	node(void) : r(this), l(this), d(this), u(this), c(this), row(0), is_column(false), column(0), size(0) { }
	node(const string &name_) :r(this), l(this), d(this), u(this), c(this), row(0), is_column(true), column(0), name(name_), size(0) { }
};

struct matrix {
	node *h;
	unsigned int rows;
	unsigned int columns;

	matrix(void) : h(NULL), rows(0) {
		h = new node();
	}

	struct addColumn {
		node *h;
		addColumn(struct node *m) : h(m) { }
		void operator() (const string &name) {
			node *p = h;
			while (p->r != h)
				p = p->r;

			node *n = new node(name);
			n->r = h;
			n->l = p;
			p->r = n;
			h->l = n;

			if (p != h) {
				n->column = p->column + 1;
			}
		}
	};

	void add_column(const string &name) {
		node *p = h;
		while (p->r != h)
			p = p->r;

		node *n = new node(name);
		n->r = h;
		n->l = p;
		p->r = n;
		h->l = n;

		if (p != h) {
			n->column = p->column + 1;
		}
		columns++;
	}

	void print_columns(void) {
		node *c = h->r;
		while (c != h) {
			cout << c->name << "(" << c->size << ")\t";
			c = c->r;
		}
		cout << endl;
	}

	matrix(const vector<string> &column_names) : h(NULL), rows(0) {
		h = new node();
		for_each (column_names.begin(), column_names.end(), addColumn(h));
		columns = column_names.size();
	}

	bool add_row(const vector<string> &column_names) {
		node *head = NULL;
		for (vector<string>::const_iterator it = column_names.begin();
		     it != column_names.end();
		     ++it) {
			node *n = new node();
			if (head == NULL) {
				head = n;
			}

			node *r = head;
			while (r->r != head)
				r = r->r;

			n->l = r;
			n->r = head;
			r->r = n;
			head->l = n;

			node *c = h->r;
			while (c != h && c->name != *it) {
				c = c->r;
			}

			if (c == h) {
				return false;
			}

			n->c = c;
			c->size++;

			node *p = c;
			while (p->d != c)
				p = p->d;
			n->u = p;
			n->d = c;
			c->u = n;
			p->d = n;

			n->row = rows;
		}

		rows++;
		return true;
	}

	void print_matrix(void) {
		print_columns();
		for (unsigned int r = 0; r != rows; ++r) {
			node *c = h->r;
			while (c != h) {
				node *n = c->d;
				while (n != c && n->row < r) {
					n = n->d;
				}
				if (n != c && n->row == r) {
					cout << "1";
				} else {
					cout << "0";
				}
				cout << "\t";
				c = c->r;
			}
			cout << "\n";
		}
	}

	void search(void) {
		stack<node *> solutions;
		unsigned int n = 0;
		unsigned long int examined_nodes = 0;
		solve(solutions, n, examined_nodes);
		cout << "Number of solutions: " << n << ". Nodes examined: " << examined_nodes << endl;
	}

	void print_solutions(stack<node *> &solutions, unsigned int &n) {
		cout << "Solution " << n << ": \n";
		stack<node *> tmp;
		while (!solutions.empty()) {
			tmp.push(solutions.top());

			node *q = tmp.top();
			while (q->c->column > q->l->c->column) {
				q = q->r;
			}

			node *p = q;

			unsigned int i = 0;

			do {
				for (; i < p->c->column; ++i) {
					cout << "0\t";
				}
				cout << p->c->name << "\t";
				i = p->c->column + 1;
				p = p->r;
			} while (p != q);

			for (; i != columns; ++i) {
				cout << "0\t";
			}

			cout << "\n";

			solutions.pop();
		}
		cout << "\n";
		while (!tmp.empty()) {
			solutions.push(tmp.top());
			tmp.pop();
		}
	}

	void cover(node *c, unsigned long int &examined_nodes) {
		c->r->l = c->l;
		c->l->r = c->r;
		node *r = c->d;
		examined_nodes++;
		while (r != c) {
			node *p = r->r;
			//examined_nodes++;
			while (p != r) {
				//examined_nodes++;
				p->u->d = p->d;
				p->d->u = p->u;
				p->c->size--;
				p = p->r;
			}
			r = r->d;
		}
	}

	void uncover(node *c, unsigned long int &examined_nodes) {
		node *r = c->u;
		//examined_nodes++;
		while (r != c) {
			node *p = r->l;
			//examined_nodes++;
			while (p != r) {
				p->u->d = p;
				p->d->u = p;
				p->c->size++;
				p = p->l;
			}
			//examined_nodes++;
			r = r->u;
		}
		c->r->l = c;
		c->l->r = c;
	}

	bool solve(stack<node *> &solutions, unsigned int &n, unsigned long int &examined_nodes) {
		if (h->r == h) {
			n++;
			return true;
		}

		unsigned long int min = ULONG_MAX;

		node *curr = h->r;
		node *c = curr;
		while (curr != h) {
			if (curr->size < min) {
				min = curr->size;
				c = curr;
			}
			curr = curr->r;
		}

		cover(c, examined_nodes);
		node *r = c->d;
		while (r != c) {
			node *p = r->r;
			solutions.push(r);

			while (p != r) {
				cover(p->c, examined_nodes);
				p = p->r;
				//examined_nodes++;
			}

			bool found = solve(solutions, n, examined_nodes);

			p = r->l;
			while (p != r) {
				uncover(p->c, examined_nodes);
				p = p->l;
				//examined_nodes++;
			}

			if (found) {
				print_solutions(solutions, n);
			}

			solutions.pop();
			r = r->d;
		}

		uncover(c, examined_nodes);

		return false;
	}
};

int main (int argc, char **argv) {
	matrix m( {"A", "B", "C", "D", "E", "F", "G"} );
	m.add_row( {"C", "E", "F" } );
	m.add_row( {"A", "D", "G" } );
	m.add_row( {"B", "C", "F" } );
	m.add_row( {"A", "D"} );
	m.add_row( {"B", "G"} );
	m.add_row( {"D", "E", "G"} );

	m.print_matrix();

	m.search();

	return 0;
}
