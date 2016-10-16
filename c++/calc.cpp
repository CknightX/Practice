#include<iostream>
#include<string>
#include "scanner.h"
using namespace std;
string s="1+2.94*(5.6-8/(4-8))#";
int index = 0;
struct Node
{
	Node(string _c, double _result = 0, Node *_left = nullptr, Node *_right = nullptr) :c(""), result(_result), left(_left), right(_right){ c += _c; }
	string c;
	double result;
	Node *left, *right;
};
Node* _deal_mul();
Node* _deal_add();
Node* _deal_num();
void error(char err,int i)
{
	for (int j = 0; j < i; ++j)
		cout << ' ';
	cout << "^\n";
	cerr <<"index: "<<i<<'\t'<<"Error: "<< err<<endl;
	throw index;
}
double count(double a, char b, double c)
{
	int d;
	switch (b)
	{
	case '+':
		return a + c;
		break;
	case '-':
		return a - c;
		break;
	case '*':
		return a*c;
		break;
	case '/':
		return a / c;
		break;
	}
}
double str_to_dbl(string s)
{
	bool flag = false;
	double sum = 0.0;
	int n = -1;
	for (auto c : s)
	{
		if (c == '.')
		{
			flag = true;
			continue;
		}
		if (!flag)
		{
			sum = sum * 10 + (c - '0');
		}
		else
		{
			sum += pow(10, n--)*(c - '0');
		}
	}
	return sum;
}
char eat(){ return s[index++]; }
void throw_char(){ --index; }
void back_in(Node *p)
{
	if (p)
	{
		if (p->left)
			back_in(p->left);
		if (p->right)
			back_in(p->right);
		cout << p->c<<' ';
		delete p;
	}

}
void mid_in(Node *p)
{

	if (p)
	{
		if (p->left)
			mid_in(p->left);
		cout << p->c<<' ';
		if (p->right)
			mid_in(p->right);
	}

}
void front_in(Node *p)
{
	if (p)
	{
		cout << p->c<<' ';
		if (p->left)
			front_in(p->left);
		if (p->right)
			front_in(p->right);
	}


}
int main()
{
	cout << "Calc  by:CknightX"<<endl;
	cout << "-----------------"<<endl;
	while (1)
	{
		cout << "% ";
		cin >> s;
		s += '#';
		Node *p=nullptr;
		try
		{
			p = _deal_add();
		}
		catch (int i)
		{
			index = 0;
			continue;
		}
		double sum = p->result;
		cout << "前缀表达式：[ ";
		front_in(p);
		cout << ']';
		cout << endl;
		cout << "中缀表达式：[ ";
		mid_in(p);
		cout << ']';
		cout << endl;
		cout << "后缀表达式：[ ";
		back_in(p);
		cout << ']';
		cout << endl;
		cout << endl << ">>" << sum<<endl;
		index = 0;
	}
	return 0;
}
Node * _deal_num()
{
	char c = eat();
	string tmp = "";
	if (c >= '0'&&c <= '9')
	{
		while ((c >= '0'&&c <= '9') || c == '.')
		{
			tmp += c;
			c = eat();
		}
		throw_char();
		return new Node(tmp, str_to_dbl(tmp));
	}
	else if (c == '(')
	{
		Node *tmp = _deal_add();
		if ((c = eat()) != ')')
		{
			error(')', index);
		}
		return tmp;
	}
	else if (c == '-')
	{
		Node *tmp = _deal_num();
		return new Node("-", count(0, '-', tmp->result),new Node("0",0),tmp);
	}
	else
	{
		error(c,index);
	}
}
Node * _deal_mul()
{
	Node *left = _deal_num();
	char c = eat();
	if (c == '*' || c == '/')
	{
		Node *result = left;
		while (c == '*' || c == '/')
		{
			Node *right = _deal_num();
			string tmp = "" ;
			tmp += c;
			result = new Node (tmp,count(result->result,c,right->result), result, right);
			c = eat();
		}
		throw_char();
		return result;
	}
	else if (c == '+' || c == '-' || c == ')')
	{
		throw_char();
		return left;
	}
	else if (c == '#')
	{
		throw_char();
		return left;
	}
	else
	{
		error(c,index);
	}
}
Node * _deal_add()
{
	Node *left = _deal_mul();
	char c = eat();
	if (c == '+' || c == '-')
	{
		Node *result = left;
		while (c == '+' || c == '-')
		{
			Node *right = _deal_mul();
			string tmp = "" ;
			tmp += c;
			result = new Node(tmp,count(result->result, c, right->result), result, right);
			c = eat();
		}
		throw_char();
		return result;
	}
	else if (c == ')')
	{
		throw_char();
		return left;
	}
	else if (c == '#')
		return left;
	else
	{
		error(c,index);
	}
}
