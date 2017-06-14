#define _SCL_SECURE_NO_WARNINGS

#include<iostream>
#include<algorithm>
#include<map>
#include<vector>

using namespace std;
class Base
{
public:
	Base(int n) :suspend_counter(0), block(new int[n]), block_size(n){ fill_n(block, block_size, 0); }
	virtual void get(int* _page,size_t n) = 0;
	int get_suspend(){ return suspend_counter; } //获取中断次数
	int* is_in_block(int page){ auto result = find(block, block + block_size, page); return (result == block + block_size ? nullptr : result); } //是否位于页表
	void move_page(int* page){ copy(page + 1, block + block_size, page); }  //移走某个页
	void add_page(int page){ block[block_size - 1] = page; } //向页表末尾添加
	void print_block(){ for_each(block, block + block_size, [](const int& i){if (i)cout << i << ' '; }); cout << endl; } //输出当前页表
	~Base(){ delete[]block; }
protected:
	int suspend_counter;
	int* block;
	size_t block_size;
};

class FIFO :public Base
{
public:
	FIFO(int n) :Base(n){}
	void get(int* _page,size_t n)
	{
		for (size_t i = 0; i < n; ++i)
		{
			auto page = _page[i];
			auto _end = block + block_size;
			auto result = find(block, _end, page);
			if (result != _end)
				continue;
			else
			{
				++suspend_counter;
				move_page(block);
				add_page(page);
			}
		print_block();
		}
	}
};

class LRU :public Base
{
public:
	LRU(int n) :Base(n){}
	void get(int* _page, size_t n)
	{
		for (size_t i = 0; i < n; ++i)
		{
			auto page = _page[i];
			auto _end = block + block_size;
			auto result = find(block, _end, page);
			if (result != _end)
			{
				move_page(result);
			}
			else
			{
				++suspend_counter;
				move_page(block);
			}
			add_page(page);
		print_block();
		}
	}
};

class OPT :public Base
{
public:
	OPT(int n) :Base(n){}
	void get(int* _page,size_t n)
	{
		map<int, int> page_num;
		for (size_t i = 0; i < n; ++i)
		{
			if (is_in_block(_page[i]))
				continue;
			else
			{
				++suspend_counter;
				vector<int> result;
				//  最久未使用
				for (size_t j = 0; j < block_size; ++j) 
				{
					int len = 0;
					for (size_t k = i + 1; k < n; ++k)
					{
						if (block[j] == _page[k])
							break;
						++len;
					}
					result.emplace_back(len);
				}
				size_t index = max_element(result.begin(), result.end()) - result.begin();
				move_page(block + index);
				add_page(_page[i]);
			}
		print_block();
		}
	}
};
int main()
{
	FIFO fifo(3);
	LRU lru(3);
	OPT opt(3);
	int a[12] = { 2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2 };
	fifo.get(a, 12);
	lru.get(a, 12);
	opt.get(a, 12);
	cout << fifo.get_suspend()<<endl<<lru.get_suspend()<<endl<<opt.get_suspend();
	cin.get();
	return 0;
}