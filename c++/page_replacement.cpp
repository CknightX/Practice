#define _SCL_SECURE_NO_WARNINGS

#include<iostream>
#include<algorithm>
#include<map>
#include<vector>
#include<random>

#define TEST { 4, 3, 2, 1, 4, 3, 5, 4, 3, 2, 1, 5 }

using namespace std;


typedef vector<int> page_list;
class Base
{
public:
	static vector<int> rand_list()
	{
		random_device rand;
		mt19937 mt(rand());
		vector<int> page;
		for (unsigned int i = 0; i < (mt()%15+5); ++i) 
			page.emplace_back(mt() % 9 + 1);
		return page;
	}
	Base(int n) :suspend_counter(0), block(new int[n]), block_size(n)
	{ 
		fill_n(block, block_size, -1);
	}
	virtual void get(page_list& _page) = 0;

	int get_suspend()
	{ 
		return suspend_counter; 
	}
	int* is_in_block(int page)
	{ 
		auto result = find(block, block + block_size, page);
		return (result == block + block_size ? nullptr : result);
	}
	void move_page(int* page)
	{ 
		copy(page + 1, block + block_size, page);
	}
	void add_page(int page)
	{ 
		block[block_size - 1] = page;
	}
	void get_block(bool is_suspend)
	{
		vector<int> sg_re; 
		sg_re.reserve(block_size+1);
		for_each(block, block + block_size, [&sg_re, &is_suspend](const int& i){ sg_re.emplace_back(i);  }); sg_re.emplace_back((int)is_suspend); result.push_back(sg_re);
	}
	void print_h()
	{ 
		for (size_t i = 0; i < block_size; ++i)
		{
			for_each(result.begin(), result.end(), [&i](const vector<int>& vec){if (vec[i] < 0)cout << ' '; else cout << vec[i]; cout<< ' '; });
			cout << endl;
		}
		for_each(result.begin(), result.end(), [&](const vector<int>& vec){if (vec[block_size]== 0)cout << ' '; else cout << '*'; cout << ' '; });
		cout << endl;
	}
	~Base(){ delete[]block; }
protected:
	int suspend_counter;
	int* block;
	size_t block_size;
	vector<vector<int>> result;
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
			get_block(flag);

		}
		cout << "lose page:" << get_suspend() << endl;
		print_h();
		cout << (double)get_suspend() / _page.size() * 100<<"%"<<endl;
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
			get_block(flag);

		}
		cout << "lose page:" << get_suspend() << endl;
		print_h();
		cout << (double)get_suspend() / _page.size() * 100 << "%" << endl;
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
			bool flag = false;
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
				flag = true;
			}
			get_block(flag);

		}
		cout << "lose page:" << get_suspend() << endl;
		print_h();
		cout << (double)get_suspend() / _page.size() * 100 << "%" << endl;
	}
};
int main()
{
	FIFO fifo(3);
	LRU lru(3);
	OPT opt(3);


	
	vector<int> page = Base::rand_list();

	cout << "list:";
	for_each(page.begin(), page.end(), [&page](const int& i){cout << i << ' '; });
	cout << endl<<endl;


	fifo.get(page);
	lru.get(page);
	opt.get(page);

	cout << endl;
	cin.get();
	return 0;
}