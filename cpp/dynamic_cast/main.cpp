#include <iostream>

using namespace std;

class B {
};

class D : public B {
};

class BV {
public:
	virtual void foo(void) {  }
};

class DV : public BV {
public:
	void foo(void) override { }
};

class E {
};

template<class T, class> struct m_is_polymorphic_impl   : false_type {};
template<class T> struct m_is_polymorphic_impl
    <T, decltype(dynamic_cast<void*>(declval<T*>()))> : true_type {};

template<class T> struct m_is_polymorphic :
	    m_is_polymorphic_impl<remove_cv_t<T>, void*> {};

int main(int argc, char **argv) {
	//cout << sizeof(dynamic_cast<const volatile void *>(declval<const D *>())) << endl;
	cout << m_is_polymorphic<DV>::value << endl;

	return 0;
}
