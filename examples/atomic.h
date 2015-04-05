#include <atomic>

namespace lemmy {

qualifier atomic;

template<typename T>
using atomic T = std::atomic<T>; //should be trivial.

template<typename T>
T qualifier_cast(atomic T t) = delete; //the qualifier cannot be cast away, this also inclued atomic T& and atomic T*


}

namespace lemmy2
{

struct test_struct
{
	lemmy::atomic int i{0};

	lemmy::atomic int operator++() lemmy::atomic {return i++;}
	lemmy::atomic int operator*=(int rhs) {return i.load() *= rhs;}
};


}

void main()
{
	//calls the atomic-ctor, because it isn't explicit
	lemmy::atomic int la = 42;

	int i = la; //copy, uses implicit cast.

	int &j = qualifier_cast<T&>(la); //error, because explicitly deleted.

	using lemmy::atomic;


	//undefined type, allocates the size of int.
	atomic int x;

	using lemmy::atomic int; //because the using is defined in the namespace.
	//picks the definition from the namespace
	atomic int& k = la; //error, no operator defined


	using lemmy2::test_struct;

	test_struct ts;

	ts++;
	ts*= 2;
	//picks the definition of lemmy2.
	atomic test_struct t2 = &ts;

}

