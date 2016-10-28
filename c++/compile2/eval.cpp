#ifndef EVAL_CPP
#define EVAL_CPP
#include<iostream>
#include<stack>
#include<vector>
#include<string>
#include<stdexcept>
#include<memory>
#include "scanner.h"
using namespace std;

const int num_op = 14; //运算符个数
//运算符顺序： + - * / % ( == != > < >= <= && ||
int op_pri_map[num_op][num_op]=  //运算符运算优先数组,为1则行上的符号大于列上的符号,0则小于，若相同优先级则规定行优先级大于列
{
	//  +  -  *  /  %  (  a  b  c  d  e  f  g  h

/*+*/	1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*-*/	1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 
/***/	1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 
/*/*/	1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*%*/	1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*(*/	1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*a*/	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*b*/	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*c*/	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*d*/	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*e*/	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*f*/	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*g*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
/*h*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};
char op_to_char(string op)
{
	char c;
	if (op.size() == 1)
		c = op[0];
	if (op == "==")
		c = 'a';
	if (op == "!=")
		c = 'b';
	if (op == ">")
		c = 'c';
	if (op == "<")
		c = 'd';
	if (op == ">=")
		c = 'e';
	if (op == "<=")
		c = 'f';
	if (op == "&&")
		c = 'g';
	if (op == "||")
		c = 'h';
	return c;
}
int f(char op)
{

	int i;
	switch (op)
	{
	case '+':
		i = 0;
		break;
	case '-':
		i = 1;
		break;
	case '*':
		i = 2;
		break;
	case '/':
		i = 3;
		break;
	case '%':
		i = 4; break;
	case '(':
		i = 5; break;
	case 'a':
		i = 6; break;
	case 'b':
		i = 7; break;
	case 'c':
		i = 8; break;
	case 'd':
		i = 9; break;
	case 'e':
		i = 10; break;
	case 'f':
		i = 11; break;
	case 'g':
		i = 12; break;
	case 'h':
		i = 13; break;
	default:
		i = -1;
	}
	return i;
}
int compare_op(char op1, char op2)
{
	
	return op_pri_map[f(op1)][ f(op2)];

}
double count_num(double num1, char op, double num2)
{
	double num = 0;
	switch (op)
	{
	case '+':
		num= num1 + num2;
		break;
	case '-':
		num= num1 - num2;
		break;
	case '*':
		num= num1*num2;
		break;
	case '/':
		num= num1 / num2;
		break;
	case '%':
		if (num1 != static_cast<int>(num1) || num2 != static_cast<int>(num2))
			throw invalid_argument("%运算两侧必须为整数！");
		else
			num = static_cast<int>(num1) % static_cast<int>(num2);
		break;
	case 'a':
		num=(num1==num2); break;
	case 'b':
		num = (num1 
			!= num2); break;
	case 'c':
		num = (num1 > num2); break;
	case 'd':
		num = (num1 < num2); break;
	case 'e':
		num = (num1 >= num2); break;
	case 'f':
		num = (num1 <= num2); break;
	case 'g':
		num = (num1 && num2); break;
	case 'h':
		num = (num1 || num2); break;
	}
	return num;
}
double string_to_int(string exp)
{
	int flag = -1; //-1- 开始状态 0-正整数 1-负整数 2-正小数 3-负小数
	int i = 1; //用于小数
	double sum = 0;
	for (auto c : exp)
	{
		switch (flag)
		{
		case -1:
			if (c >= '0'&&c <= '9')
			{
				sum += (c - '0');
				flag = 0;
			}
			else if (c == '.')
			{
				flag = 2;
			}
			else if (c == '-')
			{
				flag = 1;
			}
			else throw(c);
			break;
		case 0:
			if (c >= '0'&&c <= '9')
			{
				sum = 10 * sum + (c - '0');
			}
			else if (c == '.')
			{
				flag = 2;
			}
			else throw(c);
			break;
		case 1:
			if (c >= '0'&&c <= '9')
			{
				sum = 10 * sum + (c - '0');
			}
			else if (c == '.')
			{
				flag = 3;
			}
			else throw (c);
			break;
		case 2:
			if (c >= '0'&&c <= '9')
				sum += pow(10, -(i++))*(c - '0');
			else throw(c);
			break;
		case 3:
			if (c >= '0'&&c <= '9')
				sum += pow(10, -(i++))*(c - '0');
			else throw(c);
			break;
		}
	}
	if (flag == 1 || flag == 3)
		sum = -sum;
	return sum;
}
template<class T>
double LookforValue(string name, T  variable_list)
{
	for (auto i = variable_list.begin(); i != variable_list.end(); ++i)
	{
		if ((*i)->name == name)
			return ((*i)->u.double_type);
	}
	throw name;
}
template<class T>
double eval(string expression, T  variable_list) //求值 要求变量表为一个vector<shared_ptr> 且指针指向节点有name属性
{

	Scanner exp_scan;
	exp_scan.LoadString(expression);

	stack<char> op; //表达式栈
	stack<double> num; //数字栈
			bool flag = false;

	while (exp_scan.GetNextToken() != TOKEN_END)
	{
		switch (exp_scan.ICurrToken)
		{
			
		case TOKEN_INT:
		case TOKEN_FLOAT:
				num.push(string_to_int(exp_scan.CurrToken));
			break;
		case TOKEN_SUB:
		case TOKEN_ADD:
		
		case TOKEN_MUL:
		case TOKEN_DIV:
		case TOKEN_OPEN_BRACKET:
		case TOKEN_TWO_EQUAL:
		case TOKEN_NOT_EQUAL:
		case TOKEN_BIG:
		case TOKEN_SMALL:
		case TOKEN_BIG_EQUAL:
		case TOKEN_SMALL_EQUAL:
		case TOKEN_AND:
		case TOKEN_OR:
		case TOKEN_MOD:
			if (op.empty())
				op.push(op_to_char(exp_scan.CurrToken));
			else if (compare_op(op.top(), op_to_char(exp_scan.CurrToken)) > 0)
			{
				while (!op.empty()&&op.top()!='('&&compare_op(op.top(), op_to_char(exp_scan.CurrToken)) > 0)
				{
					double num1, num2;
					num2 = num.top();
					num.pop();
					num1 = num.top();
					num.pop();
					char c = op.top();
					op.pop();
					num.push(count_num(num1, c, num2));
				}
				op.push(op_to_char(exp_scan.CurrToken));
			}
			else
			{
				op.push(op_to_char(exp_scan.CurrToken));
			}
			break;
		case TOKEN_CLOSE_BRACKET:
			while (!op.empty() &&op.top()!='(')
			{
				double num1, num2;
				num2 = num.top();
				num.pop();
				num1 = num.top();
				num.pop();
				char c = op.top();
				op.pop();
				num.push(count_num(num1, c, num2));
			}
			if (op.empty())
			{ }
			else
			{
				if (op.top() != '(')
				throw op.top();
				else
				op.pop();
			}
			break;
		case TOKEN_IDENT:
			num.push(LookforValue(exp_scan.CurrToken, variable_list));
			break;
		}

	}
	while (!op.empty())
	{
		double num1, num2;
		num2 = num.top();
		num.pop();
		num1 = num.top();
		num.pop();
		char c = op.top();
		op.pop();
		num.push(count_num(num1, c, num2));
	}
	return num.top();

	/*
	auto compare_op = [](char l, char r)
	{
		if (l == '+' || l == '-')
		{
			if (r == '*' || r == '/')
				return false;
			else
				return true;
		}
		else
			return true;
	};
	char c;
	auto is_num = [&c](){return (c >= '0'&&c <= '9'); };
	auto is_alp = [&c](){return ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z')); };
	auto Count = [](double a, char b, double c)
	{
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
		default:
			return 0.0;
			break;
		}
	};
	int flag = 0;//0-start 1-num 2-ident 3-op
	double sum = 0;
	string ident = "";
	for (auto i = expression.begin(); i != expression.end(); ++i)
	{
		c = *i;
		if (is_num())
		{
			while (is_num())
			{
				sum = 10 * sum + c - '0';
				if (i + 1 == expression.end())
					break;
				c = *(++i);
			}
			if (c == '.')
			{
				int j = 1;
				c = *(++i);

				while (is_num())
				{
					sum += pow(10, -(j++))*(c - '0');
					if (i + 1 == expression.end())
						break;
					c = *(++i);

				}

			}
			num.push(sum);
			sum = 0;
			if (!is_num())
				--i;
		}
		else if (is_alp())
		{
			while (is_alp() || is_num())
			{
				ident += c;
				if (i + 1 == expression.end())
					break;
				c = *(++i);
			}
			try
			{
				sum = LookforValue(ident,variable_list);
			}
			catch (string s)
			{
				cout << "error,can't find " << s;
				exit(1);
			}
			num.push(sum);
			sum = 0;
			ident = "";
			if (!is_alp())
				--i;
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(')
		{
			if (op.empty() || c == '(')
			{
				op.push(c);
			}
			else if (c == '+' || c == '-')
			{
				while (!op.empty() && op.top() != '(')
				{
					double num2 = num.top();
					num.pop();
					double num1 = num.top();
					num.pop();
					num.push(Count(num1, op.top(), num2));
					op.pop();
				}
				op.push(c);
			}
			else if (c == '*' || c == '/')
			{
				if (op.top() == '*' || op.top() == '/')
				{
					double num2 = num.top();
					num.pop();
					double num1 = num.top();
					num.pop();
					num.push(Count(num1, op.top(), num2));
					op.pop();
					op.push(c);
				}
				else
					op.push(c);
			}

		}
		else if (c == ')')
		{
			while (op.top() != '(')
			{
				double num2 = num.top();
				num.pop();
				double num1 = num.top();
				num.pop();
				num.push(Count(num1, op.top(), num2));
				op.pop();
			}
			op.pop();
		}
	}
	while (!op.empty())
	{
		double num2 = num.top();
		num.pop();
		double num1 = num.top();
		num.pop();
		num.push(Count(num1, op.top(), num2));
		op.pop();
	}
	return num.top();
	*/
} //表达式求值

#endif