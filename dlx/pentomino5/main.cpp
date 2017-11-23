#include <iostream>
#include <string>
#include <sstream>
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

	matrix(void) : h(NULL), rows(0), columns(0) {
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

	void add_columns(const vector<string> &column_names) {
		for_each (column_names.begin(), column_names.end(), addColumn(h));
		columns += column_names.size();
	}

	void print_columns(void) {
		node *c = h->r;
		while (c != h) {
			cout << c->name << "(" << c->size << ")\t";
			c = c->r;
		}
		cout << endl;
	}

	matrix(const vector<string> &column_names) : h(NULL), rows(0), columns(0) {
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
					cout << n->c->name << "\t";
				}
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

			do {
				cout << p->c->name << "\t";
				p = p->r;
			} while (p != q);

			cout << "\n";

			solutions.pop();
		}

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

struct coord {
	coord(void) : x(0), y(0) { }
	coord(unsigned int _x, unsigned int _y) : x(_x), y(_y) { }
	unsigned int x;
	unsigned int y;
};

struct placement {
	placement(const vector<coord> &coords) {
		unsigned int i = 0;
		for (vector<coord>::const_iterator it = coords.begin();
		     it != coords.end();
		     ++it) {
			place[i].x = it->x;
			place[i].y = it->y;
			i++;
		}
	}
	coord place[5];
};

struct placements {
	string name;
	vector<placement> places;
};

// double checked
placements F = { 
	.name = "F",
	.places = {
		{
			{{1, 2}, {2, 1}, {2, 2}, {2, 3}, {3, 1}}
		},
		{
			{{1, 1}, {1, 2}, {2, 2}, {2, 3}, {3, 2}}
		},
		{
			{{1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 2}}
		},
		{
			{{1, 2}, {2, 1}, {2, 2}, {3, 2}, {3, 3}}
		},
		{
			{{1, 2}, {2, 1}, {2, 2}, {2, 3}, {3, 3}}
		},
		{
			{{1, 2}, {2, 2}, {2, 3}, {3, 1}, {3, 2}}
		},
		{
			{{1, 1}, {2, 1}, {2, 2}, {2, 3}, {3, 2}}
		},
		{
			{{1, 2}, {1, 3}, {2, 1}, {2, 2}, {3, 2}}
		}
	}
};

// double checked
placements I = {
	.name = "I",
	.places = {
		{
			{{1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}}
		},
		{
			{{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}
		}
	}
};

placements L = { 
	.name = "L",
	.places = {
		{
			{{1, 1}, {2, 1}, {2, 2}, {2, 3}, {2, 4}}
		},
		{
			{{1, 1}, {1, 2}, {2, 1}, {3, 1}, {4, 1}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 4}}
		},
		{
			{{1, 2}, {2, 2}, {3, 2}, {4, 1}, {4, 2}}
		},
		{
			{{1, 4}, {2, 1}, {2, 2}, {2, 3}, {2, 4}}
		},
		{
			{{1, 1}, {2, 1}, {3, 1}, {4, 1}, {4, 2}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 1}}
		},
		{
			{{1, 1}, {1, 2}, {2, 2}, {3, 2}, {4, 2}}
		}
	}
};

// double checked
placements P = { 
	.name = "P",
	.places = {
		{
			{{1, 1}, {1, 2}, {2, 1}, {2, 2}, {2, 3}}
		},
		{
			{{1, 1}, {1, 2}, {2, 1}, {2, 2}, {3, 1}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {2, 2}, {2, 3}}
		},
		{
			{{1, 2}, {2, 1}, {2, 2}, {3, 1}, {3, 2}}
		},
		{
			{{1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}}
		},
		{
			{{1, 1}, {2, 1}, {2, 2}, {3, 1}, {3, 2}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}}
		},
		{
			{{1, 1}, {1, 2}, {2, 1}, {2, 2}, {3, 2}}
		}
	}
};

placements N = { 
	.name = "N",
	.places = {
		{
			{{1, 2}, {1, 3}, {1, 4}, {2, 1}, {2, 2}}
		},
		{
			{{1, 1}, {2, 1}, {2, 2}, {3, 2}, {4, 2}}
		},
		{
			{{1, 3}, {1, 4}, {2, 1}, {2, 2}, {2, 3}}
		},
		{
			{{1, 1}, {2, 1}, {3, 1}, {3, 2}, {4, 2}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {2, 3}, {2, 4}}
		},
		{
			{{1, 2}, {2, 2}, {3, 1}, {3, 2}, {4, 1}}
		},
		{
			{{1, 1}, {1, 2}, {2, 2}, {2, 3}, {2, 4}}
		},
		{
			{{1, 2}, {2, 1}, {2, 2}, {3, 1}, {4, 1}}
		}
	}
};

// double checked
placements T = { 
	.name = "T",
	.places = {
		{
			{{1, 1}, {1, 2}, {1, 3}, {2, 2}, {3, 2}}
		},
		{
			{{1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 3}}
		},
		{
			{{1, 2}, {2, 2}, {3, 1}, {3, 2}, {3, 3}}
		},
		{
			{{1, 1}, {2, 1}, {2, 2}, {2, 3}, {3, 1}}
		}
	}
};

// double checked
placements U = { 
	.name = "U",
	.places = {
		{
			{{1, 1}, {1, 3}, {2, 1}, {2, 2}, {2, 3}}
		},
		{
			{{1, 1}, {1, 2}, {2, 1}, {3, 1}, {3, 2}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 3}}
		},
		{
			{{1, 1}, {1, 2}, {2, 2}, {3, 1}, {3, 2}}
		}
	}
};

placements V = { 
	.name = "V",
	.places = {
		{
			{{1, 1}, {1, 2}, {1, 3}, {2, 1}, {3, 1}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {2, 3}, {3, 3}}
		},
		{
			{{1, 3}, {2, 3}, {3, 1}, {3, 2}, {3, 3}}
		},
		{
			{{1, 1}, {2, 1}, {3, 1}, {3, 2}, {3, 3}}
		}
	}
};

placements W = { 
	.name = "W",
	.places = {
		{
			{{1, 2}, {1, 3}, {2, 1}, {2, 2}, {3, 1}}
		},
		{
			{{1, 1}, {1, 2}, {2, 2}, {2, 3}, {3, 3}}
		},
		{
			{{1, 3}, {2, 2}, {2, 3}, {3, 1}, {3, 2}}
		},
		{
			{{1, 1}, {2, 1}, {2, 2}, {3, 2}, {3, 3}}
		}
	}
};

placements X = {
	.name = "X",
	.places = {
		{
			{{1, 2}, {2, 1}, {2, 2}, {2, 3}, {3, 2}}
		}
	}
};

placements Y = { 
	.name = "Y",
	.places = {
		{
			{{1, 2}, {2, 1}, {2, 2}, {2, 3}, {2, 4}}
		},
		{
			{{1, 1}, {2, 1}, {2, 2}, {3, 1}, {4, 1}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 3}}
		},
		{
			{{1, 2}, {2, 2}, {3, 1}, {3, 2}, {4, 2}}
		},
		{
			{{1, 3}, {2, 1}, {2, 2}, {2, 3}, {2, 4}}
		},
		{
			{{1, 1}, {2, 1}, {3, 1}, {3, 2}, {4, 1}}
		},
		{
			{{1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 2}}
		},
		{
			{{1, 2}, {2, 1}, {2, 2}, {3, 2}, {4, 2}}
		}
	}
};

placements Z = { 
	.name = "Z",
	.places = {
		{
			{{1, 1}, {1, 2}, {2, 2}, {3, 2}, {3, 3}}
		},
		{
			{{1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}}
		},
		{
			{{1, 2}, {1, 3}, {2, 2}, {3, 1}, {3, 2}}
		},
		{
			{{1, 1}, {2, 1}, {2, 2}, {2, 3}, {3, 3}}
		}
	}
};

const unsigned int N_PENTO = 12;

placements pentiminoes[N_PENTO] = {
	F, I, L, P, N, T, 
	U, V, W, X, Y, Z
};

bool valid (unsigned int x, unsigned int y) {
	if ((x == 3 && y == 6) ||
	    (x == 4 && y == 5) ||
	    (x == 5 && y == 4) ||
	    (x == 6 && y == 3)) {
		return false;
	}

	return true;
}

void setup_pentiminoes(matrix &m) {
	for (unsigned int i = 0; i != N_PENTO; ++i) {
		placements &p = pentiminoes[i];

		for (vector<placement>::const_iterator it = p.places.begin();
		     it != p.places.end();
		     ++it) {

			unsigned int max_x = 0, max_y = 0;
			for (unsigned int j = 0; j != 5; j++) {
				if (it->place[j].x > max_x)
					max_x = it->place[j].x;
				if (it->place[j].y > max_y)
					max_y = it->place[j].y;
			}
			
			for (unsigned int j = 0; j != 8 - max_x + 1; j++) {
				for (unsigned int k = 0; k != 8 - max_y + 1; k++) {
					vector<string> row{p.name};

					for (unsigned int l = 0; l != 5; ++l) {
						if (!valid(it->place[l].x + j, it->place[l].y + k))
							break;
						stringstream ss;
						ss << it->place[l].x + j << it->place[l].y + k;
						row.push_back(ss.str());
					}

					if (row.size() == 6) {
						m.add_row(row);
					}
				}
			}
		}
	}
}

int main (int argc, char **argv) {
	matrix m( {"F", "I", "L", "P", "N", "T",
		   "U", "V", "W", "X", "Y", "Z"} );

	m.add_columns( { "11", "12", "13", "14", "15", "16", "17", "18" } );
	m.add_columns( { "21", "22", "23", "24", "25", "26", "27", "28" } );
	m.add_columns( { "31", "32", "33", "34", "35",       "37", "38" } );
	m.add_columns( { "41", "42", "43", "44",       "46", "47", "48" } );
	m.add_columns( { "51", "52", "53",       "55", "56", "57", "58" } );
	m.add_columns( { "61", "62",       "64", "65", "66", "67", "68" } );
	m.add_columns( { "71", "72", "73", "74", "75", "76", "77", "78" } );
	m.add_columns( { "81", "82", "83", "84", "85", "86", "87", "88" } );

	m.print_matrix();

	setup_pentiminoes(m);

	m.print_matrix();

	m.search();

	return 0;
}
