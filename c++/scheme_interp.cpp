/*
expression -> ([+-/*] list)
expression -> (define ident num)
expression -> (define (ident1 ident2) ([+-/*] ident2 ident2))
expression -> (ident x)
expression -> (num)
num -> expression
num -> [0-9]+
num -> var
list -> @
list -> num list


*/


#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<memory>
#include<map>
#include "scanner.h"
using namespace std;
map<string,double> variable_list; //全局变量表
class func_define;
vector<shared_ptr<func_define> > func_list;
double op(double a, string op, double b)
{
	if (op == "+")
		return a + b;
	else if (op == "-")
		return a - b;
	else if (op == "*")
		return a*b;
	else if (op == "/")
		return a / b;
}
class expression;

class func_define
{
public:
	func_define(const string& name, shared_ptr<map<string, double>> para, shared_ptr<expression> exp)
		:name(name), para(para), exp(exp)
	{

	}
	string name;
	shared_ptr<map<string, double>> para;
	shared_ptr<expression> exp;
};

class expression
{
public:
	virtual ~expression(){}
	virtual double calc(map<string, double>& _variable_list=variable_list) = 0;
};
class num_exp :public expression
{
public:
	num_exp(const double& value) :value(value){}
	double calc(map<string, double>& _variable_list = variable_list){ return value; }
	double value;
};
class op_exp :public expression
{
public:
	op_exp(const string &op, shared_ptr<vector<shared_ptr<expression> > > para) :_op(op), para(para){}
	double calc(map<string, double>& _variable_list = variable_list)
	{
		if ((*para).empty())
			return 0;
		if ((*para).size() == 1)
			return (*para)[0]->calc();
		auto fir = (*para)[0]->calc();
		for (auto i = 1; i < (*para).size(); ++i)
			fir = op(fir, _op, (*para)[i]->calc());
		return fir;
	}
private:
	string _op;
	shared_ptr<vector<shared_ptr<expression> > > para;
};

class var_exp :public expression
{
public:
	string name;
	var_exp(const string& name) :name(name){}
	double calc(map<string, double>& _variable_list = variable_list)
	{
		if (_variable_list.count(name) > 0)
		{
			return _variable_list[name];
		}
		return 0;
	}
};

class func_call_exp :public expression
{
public:
	string name;
	shared_ptr<map<string,double>> para;
	double calc(map<string, double>& _variable_list = variable_list)
	{
		for (auto func : func_list)
		{
			if ((*func).name == name)
			{
				(*func).exp->calc(*para);
			}
		}
	}
};
template<typename T>
class var_define_exp :public expression
{
public:
	string name;
	T value;
	var_define_exp(const string& name, const T& value)
		:name(name), value(value)
	{

	}
private:

};




class Parser  //语法分析器
{
public:
	void load_code(const string& code){ scan.LoadString(code); }
	void analyze()
	{
		while (1)
		{
			try
			{
				auto exp = deal_expression();
				if (exp)
					expression_list.push_back(exp);
			}
			catch (...){ break; }
		}
	}
	void execute()
	{
		for (auto exp : expression_list)
		{
			_execute(exp);
		}
	}
private:
	Scanner scan;
	vector<shared_ptr<expression> > expression_list;
	void _execute(shared_ptr<expression> exp)
	{
		cout << exp->calc() << endl;
	}
	shared_ptr<expression> deal_func_define()
	{
		scan.GetNextToken();
		auto name = scan.CurrToken;
		auto para = make_shared<map<string,double> >();
		scan.GetNextToken();
		while (scan.ICurrToken != TOKEN_CLOSE_BRACKET)
		{
			(*para)[scan.CurrToken] = 0;
			scan.GetNextToken();
		}
		scan.GetNextToken(); // )
		auto exp = deal_expression();
		scan.GetNextToken(); // )
		func_list.push_back(make_shared<func_define>(func_define(name, para, exp)));
		return nullptr;
	}
	shared_ptr<expression> deal_var_define()
	{
		string s = scan.CurrToken;
		auto value = deal_num()->calc();
		variable_list[s] = value;
		scan.GetNextToken(); // )
		return nullptr;
	}
	shared_ptr<expression> deal_func_call()
	{

	}
	shared_ptr<expression> deal_define()
	{
		scan.GetNextToken(); //name
		if (scan.ICurrToken == TOKEN_OPEN_BRACKET)
			return deal_func_define();
		else
		{
			return deal_var_define();
		}
	}
	shared_ptr<expression> deal_expression()
	{
		scan.GetNextToken(); // (
		if (scan.ICurrToken == TOKEN_END)
			throw 1;
		scan.GetNextToken(); //运算符 
		if (scan.ICurrToken == TOKEN_FLOAT || scan.ICurrToken == TOKEN_INT || scan.ICurrToken == TOKEN_IDENT)
		{
			if (scan.ICurrToken == TOKEN_IDENT)
			{
				if (scan.CurrToken == "define")
					return deal_define();
				else //变量
				{
					scan.GetNextToken();
					if (scan.ICurrToken == TOKEN_CLOSE_BRACKET)
						return make_shared<var_exp>(scan.CurrToken);
					else
						return deal_func_call();
				}

			}
			else
			{
				return make_shared<num_exp>(strtod(scan.CurrToken.c_str(), nullptr));
			}


		}

		else
		{
			auto op = scan.CurrToken;
			auto num_list = deal_list();
			scan.GetNextToken(); // )
			return make_shared<op_exp>(op, num_list);
		}
	}
	shared_ptr<expression> deal_num()
	{
		scan.GetNextToken();
		if (scan.ICurrToken == TOKEN_INT || scan.ICurrToken == TOKEN_FLOAT)
			return make_shared<num_exp>(strtod(scan.CurrToken.c_str(), nullptr));
		else if (scan.ICurrToken == TOKEN_OPEN_BRACKET)
		{
			scan.PutToken();
			return deal_expression();
		}
		else if (scan.ICurrToken == TOKEN_IDENT)
		{
			return make_shared<var_exp>(scan.CurrToken);
		}
		else
		{
			scan.PutToken();
			throw 1;
		}

	}
	shared_ptr<vector<shared_ptr<expression>>> deal_list()
	{
		auto num_list = make_shared < vector<shared_ptr<expression>> >();
		try
		{
			while (1)
				(*num_list).push_back(deal_num());
		}
		catch (...){ return num_list; }
	}

};



int main()
{
	string s = "(define (pow x) (* x x))";
	Parser a;
	a.load_code(s);
	a.analyze();
	a.execute();
	cin.get();
}