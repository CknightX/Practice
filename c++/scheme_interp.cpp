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
#define P(x) shared_ptr<x>
#define V(x) vector<x>

#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<memory>
#include<map>
#include "scanner.h"
using namespace std;

typedef map<string, double> var_list;
var_list variable_list; //全局变量表
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
	else if (op == "<")
		return a < b;
	else if (op == ">")
		return a > b;
	else if (op == "=")
		return a == b;
}
class expression
{
public:
	virtual ~expression(){}
	virtual double calc(var_list & _variable_list=variable_list) = 0;
};

typedef shared_ptr<expression> p_exp;
typedef shared_ptr<vector<p_exp>> p_vec_exp;

class func_define
{
public:
	func_define(const string& name, shared_ptr<vector<string> > para, p_exp exp)
		:name(name), para(para), exp(exp)
	{

	}
	string name;
	shared_ptr<vector<string> > para;
	p_exp exp;
};

class and_expression :public expression
{
public:
	and_expression(shared_ptr<vector<shared_ptr<expression>>> para)
		:para(para){}
	shared_ptr<vector<shared_ptr<expression>>> para;
	double calc(map<string, double>& _variable_list = variable_list)
	{
		for (auto &i : *para)
		{
			if (i->calc(_variable_list) == 0)
				return 0;
		}
		return 1;
	}
};
class or_expression :public expression
{
public:
	or_expression(shared_ptr<vector<shared_ptr<expression>>> para)
		:para(para){}
	shared_ptr<vector<shared_ptr<expression>>> para;
	double calc(map<string, double>& _variable_list = variable_list)
	{
		for (auto &i : *para)
		{
			if (i->calc(_variable_list) == 1)
				return 1;
		}
		return 0;
	}
};
class not_expression :public expression
{
public:
	not_expression(P(expression) exp)
		:exp(exp){}
	P(expression) exp;
	double calc(var_list & _variable_list = variable_list)
	{
		return (!exp->calc(_variable_list));
	}
};
class if_expression :public expression
{
public:
	if_expression(p_exp condition,p_exp t,p_exp f)
		:condition(condition), t(t), f(f){}
	p_exp condition, t, f;
	double calc(map<string, double>& _variable_list = variable_list)
	{
		if (condition->calc(_variable_list) == 1)
			return t->calc(_variable_list);
		else
			return f->calc(_variable_list);
	}
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
			return (*para)[0]->calc(_variable_list);
		auto fir = (*para)[0]->calc(_variable_list);
		for (auto i = 1; i < (*para).size(); ++i)
			fir = op(fir, _op, (*para)[i]->calc(_variable_list));
		return fir;
	}
private:
	string _op;
	shared_ptr<vector<shared_ptr<expression> > > para;
};
class list_exp :public expression
{
public:
	list_exp(shared_ptr<vector<shared_ptr<expression> > > para) :para(para){}
	shared_ptr<vector<shared_ptr<expression> > > para;
	double calc(map<string, double>& _variable_list = variable_list)
	{
		auto &_para = *(para->rbegin()); //返回最后一个元素的求值结果
		return _para->calc(_variable_list);
	}
	bool judge(map<string, double>& _variable_list = variable_list)
	{
		auto i = ((*para)[0])->calc(_variable_list);
		return (i!=0);
	}
};
class condition_exp :public expression
{
public:
	shared_ptr<vector<shared_ptr<list_exp>>> para;
	double calc(map<string, double>& _variable_list = variable_list)
	{
		_variable_list["else"] = 1;
		for (auto &i : (*para))
		{
			if (i->judge(_variable_list))
			{
				return i->calc(_variable_list);
			}
		}
		return 0;

	}
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
		else
		{
			if (variable_list.count(name) > 0)
				return variable_list[name];
			else
				throw;
		}
		return 0;
	}
};

class func_call_exp :public expression
{
public:
	func_call_exp(const string& name, shared_ptr<vector<shared_ptr<expression> > > para_list) :name(name), para_list(para_list){}
	string name;
	shared_ptr<vector<shared_ptr<expression> > > para_list;
	double calc(map<string, double>& _variable_list = variable_list)
	{
		for (auto func : func_list)
		{
			if ((*func).name == name)
			{
				auto para = make_shared<map<string, double> >();
				if ((*func->para).empty())
					return 0;
				for (auto i = 0; i < (*func->para).size(); ++i)
					(*para)[(*func->para)[i]] = (*para_list)[i]->calc(_variable_list);
					return (*func).exp->calc(*para);
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
	void load_code(const string& code){ scan.OpenSourceFile(code); }
	void analyze()
	{
		while (1)
		{
			try
			{
				auto exp = deal_statement();
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
	P(expression) deal_statement()
	{
		scan.GetNextToken();
		scan.PutToken();
		if (scan.ICurrToken == TOKEN_INT || scan.ICurrToken == TOKEN_FLOAT)
		{
			return deal_num();
		}
		else
		{
			return deal_expression();
		}
			
	}
	shared_ptr<expression> deal_func_define()
	{
		scan.GetNextToken();
		auto name = scan.CurrToken;
		auto para = make_shared<vector<string> >();
		scan.GetNextToken();
		while (scan.ICurrToken != TOKEN_CLOSE_BRACKET)
		{
			(*para).push_back(scan.CurrToken);
			scan.GetNextToken();
		}
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
	shared_ptr<expression> deal_func_call(const string& _name)
	{
		auto name = _name;
		auto para_list = deal_list();
		auto func_call = make_shared<func_call_exp>(name, para_list);
		scan.GetNextToken(); // )
		return func_call;
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
	shared_ptr<list_exp> deal_list_exp()// ((> 1 2) 1)
	{
		scan.GetNextToken();  // (
		if (scan.ICurrToken != TOKEN_OPEN_BRACKET) // 结束  )
		{
			scan.PutToken();
			return nullptr;
		}
		auto para = deal_list();
		scan.GetNextToken(); // )
		return make_shared<list_exp>(para);
	}
	shared_ptr<vector<shared_ptr<list_exp>>> deal_para_cond_exp() // () () ()
	{
		auto para = make_shared<vector<shared_ptr<list_exp>>>();
		while (1)
		{
			auto tmp = deal_list_exp();
			if (tmp == nullptr)
				break;
			(*para).push_back(tmp);
		}
		scan.GetNextToken(); // ) 
		return para;
	}
	p_exp deal_cond_exp()
	{
		auto cond = make_shared<condition_exp>();
		cond->para = deal_para_cond_exp();
		return cond;
	}
	p_exp deal_and()
	{
		auto tmp = make_shared<and_expression>(deal_list());
		scan.GetNextToken(); // )
		return tmp;
	}
	p_exp deal_if()
	{
		auto cond = deal_num();
		auto t = deal_num();
		auto f = deal_num();
		scan.GetNextToken();
		return make_shared<if_expression>(cond, t, f);
	}
	p_exp deal_or()
	{
		auto tmp = make_shared<or_expression>(deal_list());
		scan.GetNextToken(); // )
		return tmp;
	}
	p_exp deal_not()
	{
		auto exp = deal_num();
		scan.GetNextToken(); // )
		return make_shared<not_expression>(exp);
	}
	p_exp deal_expression()
	{
		scan.GetNextToken(); // (
		if (scan.ICurrToken == TOKEN_END||scan.ICurrToken!=TOKEN_OPEN_BRACKET)
			throw 1;
		scan.GetNextToken(); //运算符 
		if (scan.ICurrToken == TOKEN_INT || scan.ICurrToken == TOKEN_FLOAT)
			return make_shared<num_exp>(strtod(scan.CurrToken.c_str(),nullptr));
		else if (scan.CurrToken == "if")
			return deal_if();
		else if (scan.CurrToken == "define")
			return deal_define();
		else if (scan.CurrToken == "cond")
			return deal_cond_exp();
		else if (scan.CurrToken == "and")
			return deal_and();
		else if (scan.CurrToken == "or")
			return deal_or();
		else if (scan.CurrToken == "not")
			return deal_not();
		else if (scan.ICurrToken == TOKEN_IDENT)
		{
			auto name = scan.CurrToken;
			scan.GetNextToken();
			if (scan.ICurrToken == TOKEN_CLOSE_BRACKET)
				return make_shared<var_exp>(name);
			else // func call
			{
				scan.PutToken();
				return deal_func_call(name);
			}
		}
		else // op_exp
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
		if (scan.ICurrToken == TOKEN_SUB)  // 负数
		{
			scan.GetNextToken();
			if (scan.ICurrToken == TOKEN_INT || scan.ICurrToken == TOKEN_FLOAT)
				return make_shared<num_exp>(-strtod(scan.CurrToken.c_str(), nullptr));
			else
				throw 1;

		}
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
	P(V(P(expression))) deal_list()
	{
		auto num_list = make_shared <V(P(expression)) >();
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


	string s = "D:\\1.txt";

	Parser a;
	a.load_code(s);
	a.analyze();
	a.execute();
	cin.get();
}