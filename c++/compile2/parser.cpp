#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
vector<shared_ptr<Statement>> statement_list;
vector<shared_ptr<FunctionNode>> func_list;
vector<shared_ptr<VariableNode>> variable_list;
vector<string> native_func = { "print", "newline" };



class StatementList;
struct Statement
{
	StatementType type;
	vector<string> para;
	shared_ptr<StatementList> block=nullptr;
	shared_ptr<StatementList> else_block=nullptr;
};

class StatementList
{
public:
	vector<shared_ptr<Statement>> statement_list;
};

stack<shared_ptr<FunctionNode>> tmp_func; //保存当前所调用的函数
int is_in_func = 0; //当前是否在执行函数

struct VariableNode
{
	VariableNode(){}
	string name;
	VariableType type;
	union _u
	{
		double double_type;
		char char_type;
	}u;
};

struct FunctionNode
{
	string name;
	shared_ptr<vector<shared_ptr<VariableNode>>> v_list; //局部变量表，参数同样添加进来
	shared_ptr<StatementList> s_list; //函数体语句
	FunctionNode(const FunctionNode& fc)
	{
		name = fc.name;
		s_list = fc.s_list;
		auto tmp_v_list = make_shared<vector<shared_ptr<VariableNode>>>();
		for (auto i : (*fc.v_list))
		{
			auto tmp_v_node = make_shared<VariableNode>();
			tmp_v_node->name = i->name;
			tmp_v_node->type = i->type;
			tmp_v_node->u = i->u;
			tmp_v_list->push_back(tmp_v_node);
		}
		v_list = tmp_v_list;
	}
	FunctionNode(){}

};
#include "eval.cpp"
	Parser::Parser(string path) :scanner(path){}
	void Parser::Assembly()
	{
		assembly_statement();
	}
	void Parser:: run()
	{
		for (auto s : statement_list)
		{
			executive_statement(s);
		}
	}
	void Parser:: _test()
	{
		cout << "----------------代码-------------\n";
		scanner._TestPrint();
		cout << endl;
		cout << "---------------------------------\n";
		cout << "---------------函数表------------\n";
		for (auto i : func_list)
		{
			cout << i->name << endl;
		}
		cout << endl;
		cout << "---------------------------------\n";
		cout << "---------------变量表------------\n";
		for (auto i : variable_list)
		{
			cout << i->name << '=' << i->u.double_type << endl;
		}
		cout << endl;

		cout << "---------------------------------\n";
		cout << "---------------语句表------------\n";
		cout << "---------------------------------\n";
		for (auto i : statement_list)
		{
			cout << i->type;
		}
		cout << endl;

	}
	// 执行
	void Parser:: executive_statement(shared_ptr<Statement> s)
	{
		switch (s->type)
		{
		case STATEMENT_IF:
			executive_if(s);
			break;
		case STATEMENT_FUNC_CALL:
			is_in_func++;
			if (tmp_func.empty())
				tmp_func.push( make_shared<FunctionNode>(*(*(find_func_by_name(s->para[0])))));
			executive_func_call(s);
			tmp_func.pop();
			
			is_in_func--;
			break;
		case STATEMENT_NATIVE_FUNC_CALL:
			executive_native_func_call(s);
			break;
		case STATEMENT_VARIABLE_ASSIGN:
			executive_variable_assign(s);
			break;
		case STATEMENT_WHILE:
			executive_while(s);
			break;
		case STATEMENT_FOR:
			executive_for(s);
			break;
		}
	}
	void Parser:: executive_variable_assign(shared_ptr<Statement> s)
	{
		vector<shared_ptr<VariableNode>> _variable_list;
		if (is_in_func)
			_variable_list = *(tmp_func.top())->v_list;
		else
			_variable_list = variable_list;
		for (auto i = variable_list.begin(); i != variable_list.end(); ++i)
		{
			if ((*i)->name == s->para[0])
			{
				(*i)->u.double_type = eval(s->para[1]);
				break;
			}
			else if (i + 1 == variable_list.end())
			{
				throw s->para[0];
			}
		}
	}
	void Parser::executive_native_func_call(shared_ptr<Statement> s)
	{
		if (s->para[0] == "print")
		{
			cout << eval(s->para[1]);
		}
		else if (s->para[1] == "newline")
		{
			cout << endl;
		}

	}
	void Parser:: executive_func_call(shared_ptr<Statement> s)
	{
		for (auto i : func_list)
		{
			if (i->name == s->para[0])
			{
				int k = 0;
				for (auto j = s->para.begin() + 1; j != s->para.end(); ++j)  //把实参计算出，并添加进函数的临时变量表
				{
					(*(i->v_list))[k++]->u.double_type = eval((*j));
				}
				tmp_func.push(make_shared<FunctionNode>(*(*(find_func_by_name(s->para[0])))));
				for (auto j : i->s_list->statement_list)
				{
					executive_statement(j);
				}
				return;
			}
		}
		throw;
	}
	void Parser::executive_while(shared_ptr<Statement> s)
	{
		double value = eval(s->para[0]);
		while (value != 0)
		{
			for (auto i : s->block->statement_list)
			{
				executive_statement(i);
			}
			value = eval(s->para[0]);
		}
	}
	void Parser::executive_for(shared_ptr<Statement> s)
	{
		executive_statement(s->else_block->statement_list[0]);
		while (eval(s->para[0]))
		{
			for (auto i : s->block->statement_list)
			{
				executive_statement(i);
			}
			executive_statement(s->else_block->statement_list[1]);
		}
	}
	void Parser::executive_if(shared_ptr<Statement> s)
	{

		double value = eval(s->para[0]);
		if (value == 0) //false
		{
			if (s->else_block)
			{
				for (auto i : s->else_block->statement_list)
				{
					executive_statement(i);
				}
			}
			else
				return;
		}
		else
		{
			for (auto i : s->block->statement_list)
			{
				executive_statement(i);
			}
		}
	}

	// 一遍扫描生成变量函数表

	void Parser::assembly_statement()
	{
		while (scanner.GetNextToken() != TOKEN_END)
		{
			auto statement_node = deal_statement(scanner.ICurrToken);
			if (statement_node)
			{
				statement_list.push_back(statement_node);
			}
		}
	}



	//statement_deal
	shared_ptr<Statement> Parser::deal_if()
	{
		auto if_statement = make_shared<Statement>();
		string expression = deal_expression();

		auto block = deal_block();
		if_statement->block = block;
		if_statement->type = STATEMENT_IF;
		if_statement->para.push_back(expression);
		scanner.GetNextToken(); // 是否有else
		if (scanner.ICurrToken == TOKEN_ELSE)
		{
			if_statement->else_block = deal_block();
		}
		else
			scanner.PutToken();
		return if_statement;
	}
	shared_ptr<Statement> Parser::deal_while()
	{
		auto while_statement = make_shared<Statement>();
		string expression = deal_expression();

		auto block = deal_block();
		while_statement->block = block;
		while_statement->type = STATEMENT_WHILE;
		while_statement->para.push_back(expression);
		return while_statement;
	}
	shared_ptr<Statement> Parser::deal_var_extern(vector<shared_ptr<VariableNode>> &variable_list) //添加进哪一个变量表
	{

		string expression;
		auto var_node = make_shared<VariableNode>();
		scanner.GetNextToken();
		string name = scanner.CurrToken;
		scanner.GetNextToken(); //  =/;
		if (scanner.ICurrToken == TOKEN_SEMI || scanner.ICurrToken == TOKEN_CLOSE_BRACKET || scanner.ICurrToken == TOKEN_COMMA) //无初始值
		{
			expression = "0";
		}
		else
		{
			expression = deal_expression();
		}
		var_node->name = name;
		var_node->u.double_type = eval(expression);
		var_node->type = TYPE_DOUBLE;
		variable_list.push_back(var_node);

		return nullptr;

	}
	shared_ptr<Statement> Parser::deal_statement(Token begin_state)
	{
		shared_ptr<Statement> statement_node = nullptr;
		string name;
		switch (begin_state)
		{
		case TOKEN_IF:
			statement_node = deal_if();
			break;
		case TOKEN_WHILE:
			statement_node = deal_while();
			break;
		case TOKEN_FOR:
			statement_node = deal_for();
			break;
		case TOKEN_IDENT:
			name = scanner.CurrToken;
			scanner.GetNextToken();
			if (scanner.ICurrToken == TOKEN_OPEN_BRACKET) //func call
			{
				scanner.PutToken();  // 把( 放回代码流
				statement_node = deal_func_call(name);
			}
			else //variable assign
			{
				statement_node = deal_var_assign(name);
			}
			break;
		case TOKEN_VAR:
			statement_node = deal_var_extern(variable_list);
			break;
		case TOKEN_FUNC:
			statement_node = deal_func_extern();
			break;
		}
		return statement_node;
	}

	shared_ptr<Statement> Parser::deal_func_extern()
	{
		auto func_node = make_shared<FunctionNode>();
		scanner.GetNextToken(); //func name
		func_node->name = scanner.CurrToken;
		func_node->v_list = deal_parameter_extern();
		func_node->s_list = deal_block();
		func_list.push_back(func_node);
		return nullptr;
		/*
		auto func_node = make_shared<FunctionNode>();
		scanner.GetNextToken(); //func name
		string name = scanner.CurrToken;
		scanner.GetNextToken(); //(
		while (1)
		{
		auto i = scanner.GetNextToken();
		if (i == TOKEN_OPEN_BRACE || i == TOKEN_CLOSE_BRACKET)
		break;
		deal_var_extern(*(func_node->v_list)); //将函数参数放入临时变量表
		}
		scanner.PutToken();
		auto _statement_list = make_shared<StatementList>();
		_statement_list = deal_block();
		func_node->name = name;
		func_node->s_list = _statement_list;
		func_list.push_back(func_node);
		return nullptr;
		*/
		return nullptr;
	}


	shared_ptr<Statement> Parser::deal_for()   // for (i=0;i<10;i=i+1)
	{
		auto for_statement = make_shared<Statement>();
		for_statement->type = STATEMENT_FOR;
		scanner.GetNextToken(); // (
		scanner.GetNextToken(); // 读取变量名
		auto left_statement = deal_statement(scanner.ICurrToken); // 左侧赋值
		auto mid_expression = deal_expression(); //中部表达式
		scanner.GetNextToken(); // 读取变量名
		auto right_statement = deal_statement(scanner.ICurrToken); //右侧赋值 最右括号被匹配
		auto block = deal_block();
		for_statement->block = block;
		for_statement->else_block = make_shared<StatementList>();
		for_statement->else_block->statement_list.push_back(left_statement);
		for_statement->else_block->statement_list.push_back(right_statement);
		for_statement->para.push_back(mid_expression);
		return for_statement;
	}

	shared_ptr<Statement> Parser::deal_func_call(string _name)
	{
		auto func_statement = make_shared<Statement>();
		if (find(native_func.begin(), native_func.end(), _name) != native_func.end()) //native func
		{
			func_statement->type = STATEMENT_NATIVE_FUNC_CALL;
		}
		else
		{
			func_statement->type = STATEMENT_FUNC_CALL;
		}
		scanner.GetNextToken(); // (
		func_statement->para.push_back(_name);
		while (scanner.GetNextToken() != TOKEN_SEMI)
		{
			scanner.PutToken();
			string exp = deal_expression();
			func_statement->para.push_back(exp);
		}

		return func_statement;
	}

	vector<shared_ptr<FunctionNode>>::iterator find_func_by_name(string _name)
	{
		for (auto i = func_list.begin(); i != func_list.end(); ++i)
		{
			if ((*i)->name == _name) //找到
			{
				return i;
			}
		}
		return func_list.end();
	}
	shared_ptr<vector<shared_ptr<VariableNode>>> Parser::deal_parameter_extern()
	{
		auto para_variable_list = make_shared<vector<shared_ptr<VariableNode>>>();
		scanner.GetNextToken();  //(
		if (scanner.ICurrToken != TOKEN_OPEN_BRACKET)
			throw;
		shared_ptr<VariableNode> tmp;
		while (scanner.GetNextToken() != TOKEN_OPEN_BRACE)
		{
			deal_var_extern(*para_variable_list);
		}
		scanner.PutToken();
		return para_variable_list;
	}
	shared_ptr<StatementList> Parser::deal_block()  //语句块
	{
		scanner.GetNextToken(); // {
		auto block_node = make_shared<StatementList>();
		while (scanner.GetNextToken() != TOKEN_CLOSE_BRACE)
		{
			auto statement_node = deal_statement(scanner.ICurrToken);
			if (statement_node)
			{
				block_node->statement_list.push_back(statement_node);
			}
		}
		return block_node;
	}
	string Parser::deal_expression(int mode) // mode 为模式
	{
		string expression = "";
		Token tmp;
		int num = 0;
		bool flag = false;
		if (mode == 0) //默认模式 
			while ((tmp = scanner.GetNextToken()) != TOKEN_SEMI&&tmp != TOKEN_COMMA)
			{
			if (scanner.ICurrToken == TOKEN_OPEN_BRACE)
			{
				scanner.PutToken();
				break;
			}
			if (scanner.ICurrToken == TOKEN_OPEN_BRACKET)
			{
				++num;
			}
			else if (scanner.ICurrToken == TOKEN_CLOSE_BRACKET)
			{
				--num;
			}
			if (num == -1)
				break;
			expression += scanner.CurrToken;


			}
		return expression;
	}
	shared_ptr<Statement> Parser::deal_var_assign(string _name)
	{

		auto statement_tmp = make_shared<Statement>();
		statement_tmp->para.push_back(_name);
		statement_tmp->type = STATEMENT_VARIABLE_ASSIGN;
		string value = deal_expression();
		statement_tmp->para.push_back(value);
		return statement_tmp;

	}
	void Parser::_add_native_func(string name, int i) //命名及参数个数
	{
		auto func_node = make_shared<FunctionNode>();
		func_node->name = name;
		auto v_list = make_shared<vector<shared_ptr<VariableNode>>>();
		for (int j = 0; j < i; ++i)
		{
			auto v_node = make_shared<VariableNode>();
			char str[10];
			sprintf(str, "%c", 'a' + j);
			v_node->name = string(str);
			v_list->push_back(v_node);
		}
		func_node->v_list = v_list;
		func_list.push_back(func_node);
	}
	void Parser::init_native_func()
	{
		_add_native_func("print", 1);
		_add_native_func("newline", 0);
	}

	void Parser::deal_parameter_call(string func_name)
	{
		auto func = find_func_by_name(func_name);
		scanner.GetNextToken(); //(
		int i = 0;
		while (scanner.GetNextToken() != TOKEN_OPEN_BRACE)
		{
			string exp = deal_expression();
			(*((*func)->v_list))[i]->u.double_type = eval(exp);
		}
		scanner.PutToken();
	}


	int main()
	{
		cout << "--------------------" << endl;
		cout << "script is running..." << endl;
		cout << "--------------------" << endl;
		Parser a("D:\\1.txt");
		try
		{
			a.Assembly();
		}
		catch (except_scanner& a)
		{
		}
		a.run();
		cin.get();
	}