#define _SCL_SECURE_NO_WARNINGS

#include<iostream>
#include<algorithm>
#include<map>
#include<vector>

using namespace std;


typedef vector<int> page_list;
class Base
{
public:
	Base(int n) :suspend_counter(0), block(new int[n]), block_size(n){ fill_n(block, block_size, -1); }
	virtual void get(page_list& _page) = 0;
	int get_suspend(){ return suspend_counter; }
	int* is_in_block(int page){ auto result = find(block, block + block_size, page); return (result == block + block_size ? nullptr : result); }
	void move_page(int* page){ copy(page + 1, block + block_size, page); }
	void add_page(int page){ block[block_size - 1] = page; }
	void print_block(){ for_each(block, block + block_size, [](const int& i){if (i >= 0)cout << i << ' '; }); }
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
	void get(page_list& _page)
	{
		cout << "FIFO:" << endl;
		for (auto page : _page)
		{
			auto _end = block + block_size;
			auto result = find(block, _end, page);
			bool flag = false;
			if (result == _end)
			{
				++suspend_counter;
				move_page(block);
				add_page(page);
				flag = true;
			}
			print_block();
			if (flag)cout << "*";
			cout << endl;
		}
		cout << "lose page:" << get_suspend() << endl;
	}
};

class LRU :public Base
{
public:
	LRU(int n) :Base(n){}
	void get(page_list& _page)
	{
		cout << "LRU:" << endl;
		for (auto page : _page)
		{
			auto _end = block + block_size;
			auto result = find(block, _end, page);
			bool flag = false;
			if (result != _end)
			{
				move_page(result);
			}
			else
			{
				++suspend_counter;
				move_page(block);
				flag = true;
			}
			add_page(page);
			print_block();
			if (flag)cout << "*";
			cout << endl;
		}
		cout << "lose page:" << get_suspend() << endl;
	}
};

class OPT :public Base
{
public:
	OPT(int n) :Base(n){}
	void get(page_list& _page)
	{
		cout << "OPT:" << endl;
		for (auto page_iter = _page.begin(); page_iter != _page.end(); ++page_iter)
		{
			bool flag = true;
			if (!is_in_block(*page_iter))
			{
				++suspend_counter;
				vector<size_t> result;

				//寻找最久未使用
				for_each(block, block + block_size, [&](const int& page_no){
					size_t len = find(page_iter + 1, _page.end(), page_no) - _page.begin();
					result.emplace_back(len);
				});


				size_t index = max_element(result.begin(), result.end()) - result.begin(); //得到最久未使用的页序号
				//移除
				move_page(block + index);

				add_page(*page_iter);
			}
			print_block();
			if (flag)cout << "*";
			cout << endl;
		}
		cout << "lose page:" << get_suspend() << endl;
	}
};
int main()
{
	FIFO fifo(4);
	LRU lru(4);
	OPT opt(4);

	vector<int> page = { 4, 3, 2, 1, 4, 3, 5, 4, 3, 2, 1, 5 };

	fifo.get(page);
	lru.get(page);
	opt.get(page);
	cout << endl;
	cin.get();
	return 0;
}