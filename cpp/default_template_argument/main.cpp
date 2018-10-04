#include <iostream>

using namespace std;

template <typename T = int>
class Foo {
	public:
		Foo() = default;
		Foo(const Foo &r) {
			cout << "copy constructor" << endl;
		}
		Foo(Foo &&r) {
			cout << "move constructor" << endl;
		}
		template <typename = double>
		void bar() {
			cout << "bar " << endl;
		}

		T x{2};
};

int main(int argc, char **argv) {
	Foo<double> f;
	f.bar();
	return 0;
}
