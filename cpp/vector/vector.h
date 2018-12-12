#ifndef MZHANG_VECTOR
#define MZHANG_VECTOR

#include <algorithm> // for std::max

namespace mzhang {

/*
 * This implementation does NOT
 * 1. have typedefs such as reference_type, pointer etc
 * 2. have iterators
 * 3. take allocator type argument
 * 4. work with bool
 * 5. work with primitive types
 * 6. check max size and index
 */

template <typename T>
class vector {
public:
	using size_type = size_t;
	using value_type = T;

	vector() = default;

	vector(size_type n) {
		if (n == 0)
			return;

		start = end = static_cast<T *>(malloc(sizeof(T) * n));
		cap = start + n;
		while (end++ != cap)
			new (end) T();
	}

	vector(const vector &rhs) {
		start = end = static_cast<T *>(malloc(sizeof(T) * rhs.capacity()));
		cap = start + rhs.capacity();
		size_type i = 0;
		while (end++ != cap)
			new (end) T(rhs[i++]);
	}

	vector(vector &&rhs) {
		start = rhs.start;
		end = rhs.end;
		cap = rhs.cap;
		rhs.start = rhs.end = rhs.cap = nullptr;
	}

	vector &operator=(const vector &rhs) {
		if (this == &rhs)
			return *this;

		clear();
		if (capacity() < rhs.size())
			grow(rhs.size());

		T *p = rhs.start;
		while (p != rhs.end)
			new (end++) T (*(p++));

		return *this;
	}

	/*
	 * Per standards, move assignment operator doesn't need to check self
	 */
	vector &operator=(vector &&rhs) {
		clear();
		start = rhs.start;
		end = rhs.end;
		cap = rhs.cap;
		rhs.start = rhs.end = rhs.cap = nullptr;
		return *this;
	}
	
	~vector() {
		clear();
		free(start);
	}

	void clear() noexcept {
		for (T *it = start; it != end; ++it)
			it->~T();
		end = start;
	}
	
	T &operator[](size_type index) {
		return *(start + index);
	}

	const T &operator[](size_type index) const {
		return *(start + index);
	}

	bool empty() const noexcept {
		return start == end;
	}

	size_type size() const noexcept {
		return end - start;
	}

	size_type capacity() const noexcept {
		return cap - start;
	}

	void reserve(size_type n) {
		if (capacity() <= n)
			return;
		grow(n);
	}

	void push_back(const T &t) {
		ensure_size();
		new (end++) T(t);
	}
			
	void push_back(T &&t) {
		ensure_size();
		new (end++) T(std::move(t));
	}

	void pop_back() {
		if (empty())
			return;

		--end;
		end->~T();
	}

private:
	size_type recommend() const noexcept {
		return std::max<size_type>(capacity() * 2, 8);
	}

	void ensure_size() {
		if (end != cap)
			return;

		grow(recommend());
	}

	void grow(size_type to) {
		T *tmp = static_cast<T *>(malloc(sizeof(T) * to));
		T *p = tmp;
		T *q = start;
		while (q != end) {
			new (p) T(std::move(*q));
			++p;
			++q;
		}

		q = start;
		while (q != end) {
			q->~T();
			++q;
		}

		free(start);
		start = tmp;
		end = p;
		cap = start + to;
	}

	T *start {nullptr};
	T *end {nullptr};
	T *cap {nullptr};
};

} /* namespace mzhang */

#endif /* MZHANG_VECTOR */
