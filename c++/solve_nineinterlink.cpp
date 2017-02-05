#include<iostream>
using namespace std;
bool nine[9] = { false };
int sum = 0;
void func(int i,int k) //方法1： 操作第i个环 , k=0 解，k=1套
{
	if (k == 0) //解环
	{
		if (nine[i] == true)
			return;
		if (i == 0)
		{
			cout << "解1" << endl;
			nine[i] = true;
		}
		else
		{
			func(i-1, 1);
			for (int j = i - 2; j >= 0; --j)
				func(j, 0);
			cout << "解" << i+1 << endl;
			nine[i] = true;
		}
	}
	else //套环
	{
		if (nine[i] == false)
			return;
		if (i == 0)
		{
			cout << "套1" << endl;
			nine[i] = false;
		}
		else
		{
			func(i-1, 1);
			for (int j = i - 2; j >= 0; --j)
				func(j, 0);
			cout << "套" << i + 1 << endl;
			nine[i] = false;
		}
	}
	sum++;
}

void solve(int i) //方法2： 不区分解和套，统一为改变环状态
{
	if (i >= 1 && nine[i - 1])	solve(i - 1);
	for (int j = i - 2; j >= 0; --j)		solve(j);
	cout << (nine[i] ? "套" : "解") << i+1 << endl;
	nine[i] = !nine[i];
	sum++;
}
int main()
{
	for (int i = 8; i >= 0; --i)
		func(i,0);
	cout << endl << endl;
	for (int i = 8; i >= 0;--i)
		solve(i);
	cout << sum;
	cin.get();
}