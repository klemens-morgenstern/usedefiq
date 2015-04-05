#include <vector>
#include <mutex>

using namespace std;

//ok, no namespace stuff.
qualifier thread_read;
qualifier thread_write : thread_read; //a write shall always be read-safe
qualifier thread_safe = thread_write; //equal.


//vector is considered read safe, if one only reads.
template<typename T, typename Allocator>
using thread_read vector<T, Allocator> = const vector<T, Allocator>;

class buffer
{
	vector<int> _data{};
	mutex _mtx;
public:
	buffer() {};

	void push_back(int i) {_data.push_back(i);}
	void push_back(int i) thread_write
	{
		lock_guard<mutex> lock(_mtx);
		qualifier_cast<vector<int>&>(_data).push_back(i);
	}
	const int& operator[](int idx) const
	{
		return _data[idx];
	}
	int& operator[](int idx)
	{
		return _data[idx];
	}

	int operator[](int i) thread_write
	{
		thread_write
		{
			lock_guard<mutex> lock(_mtx);
			return _data[i];
		}
	}
};


void main()
{
	buffer b;

	b.push_back(42); //calls the unqualified normal push_back

	thread_safe buffer &b2 = b; //trivial

	b2.push_back(21);  //calls push_back(int) thread_write, i.e. locks.

	auto &i = b2[0]; //return const int& because it calls operator[] const

	thread_write buffer &b3 = b2;

	auto j = b3[0]; //returns int, because it calls operator[] thread_write.

}



