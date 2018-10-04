#include <iostream>

using namespace std;

class Foo {
	public:
		Foo() = default;
		Foo(const Foo &r) {
			cout << "copy constructor" << endl;
			x = 1;
		}
		Foo(Foo &&r) {
			cout << "move constructor" << endl;
			x = 2;
		}
		int x{0};
};

void bar(Foo &&f) {
	cout << f.x << endl;
}

int main(int argc, char **argv) {
	Foo f{};
	bar(std::move(f));
	//bar(f);
	return 0;
}
