#include<iostream>
using namespace std;
bool nine[9] = { false };
int sum = 0;
void func(int i,int k) //����1�� ������i���� , k=0 �⣬k=1��
{
	if (k == 0) //�⻷
	{
		if (nine[i] == true)
			return;
		if (i == 0)
		{
			cout << "��1" << endl;
			nine[i] = true;
		}
		else
		{
			func(i-1, 1);
			for (int j = i - 2; j >= 0; --j)
				func(j, 0);
			cout << "��" << i+1 << endl;
			nine[i] = true;
		}
	}
	else //�׻�
	{
		if (nine[i] == false)
			return;
		if (i == 0)
		{
			cout << "��1" << endl;
			nine[i] = false;
		}
		else
		{
			func(i-1, 1);
			for (int j = i - 2; j >= 0; --j)
				func(j, 0);
			cout << "��" << i + 1 << endl;
			nine[i] = false;
		}
	}
	sum++;
}

void solve(int i) //����2�� �����ֽ���ף�ͳһΪ�ı价״̬
{
	if (i >= 1 && nine[i - 1])	solve(i - 1);
	for (int j = i - 2; j >= 0; --j)		solve(j);
	cout << (nine[i] ? "��" : "��") << i+1 << endl;
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