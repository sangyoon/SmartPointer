#include <iostream>
#include <algorithm> // for swap
#include <crtdbg.h>
#include <cstdlib>

struct Base;
template <typename T = Base> class SharedPointer; typedef SharedPointer<> SharedPtr;

struct Base {
public:
	Base(int v1, int v2) : val1(v1), val2(v2) {}
	
public:
	int val1;
	int val2;
};

template <typename T>
class SharedPointer
{
public:
	SharedPointer(T* p = nullptr)
	{
		try
		{
			ptr = p;
			count = p ? new size_t(1) : nullptr;
		}
		catch (...)
		{
			delete p;
		}
	}
	
	SharedPointer(const SharedPointer<T>& sp) : ptr(sp.ptr), count(sp.count)
	{
		if (count != nullptr) ++(*count);
	}

	SharedPointer<T>& operator=(const SharedPointer<T>& sp)
	{
		swap(sp);
		return *this;
	}

	~SharedPointer()
	{
		if ((count != nullptr) && (--(*count) == 0))
		{
			delete count; count = nullptr;
			delete ptr; ptr = nullptr;
		}
	}
	
	T& operator*() const { return *ptr; }
	T* operator->() const { return ptr; }
	size_t GetRefCount() { return (count) ? *count : -1; }

private:
	void swap(SharedPointer<T>& sp) throw()
	{
		std::swap(ptr, sp.ptr);
		std::swap(count, sp.count);
	}

private:
	T* ptr;
	size_t* count;
};

void Test1()
{
	SharedPtr p = new Base(10, 20);
	
	int i = p->val1 + (*p).val2;
	_ASSERTE(i == 30);
}

void Test2()
{
	SharedPtr p1 = new Base(10, 20);
	{
		SharedPtr p2 = p1;
		_ASSERTE(p2.GetRefCount() == 2);
	}
	
	_ASSERTE(p1.GetRefCount() == 1);
}

int main(int argc, char** argv)
{
	Test1();
	Test2();

	return EXIT_SUCCESS;
}
