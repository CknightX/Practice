#define _SCL_SECURE_NO_WARNINGS

#include<iostream>
#include<algorithm>
using namespace std;
class Base
{
public:
	Base(int n) :suspend_counter(0), block(new int[n]), block_size(n){ fill_n(block, block_size, 0); }
	virtual void get(int page) = 0;
	int get_suspend(){ return suspend_counter; }
	void print_block(){ for_each(block, block + block_size, [](const int& i){if (i)cout << i << ' '; }); }
	~Base(){ delete[]block; }
protected:
	int suspend_counter;
	int* block;
	int block_size;
};

class FIFO :public Base
{
public:
	FIFO(int n) :Base(n){}
	void get(int page)
	{
		auto _end = block + block_size;
		auto result = find(block, _end, page);
		if (result != _end)
			return;
		else
		{
			++suspend_counter; //ȱҳ
			copy(block+1, _end, block);
			block[block_size - 1] = page;
		}
	}
};

class LRU :public Base
{
public:
	LRU(int n) :Base(n){}
	void get(int page)
	{
		auto _end = block + block_size;
		auto result = find(block, _end, page);
		if (result != _end)
		{
			copy(result + 1, _end, result);
			block[block_size - 1] = page;
		}
		else
		{
			++suspend_counter; //ȱҳ
			copy(block + 1, _end, block);
			block[block_size - 1] = page;
		}
	}
};
int main()
{
	LRU test(3);
	int a[12] = { 2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2 };
	for (int i = 0; i < 12; ++i)
	{
		test.get(a[i]);
		test.print_block();
		cout << endl;
	}
	cout << test.get_suspend();
	cin.get();
	return 0;
}