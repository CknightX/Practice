#include "parser.h"
#include "eval.cpp"
#define _CRT_SECURE_NO_WARNINGS

vector<shared_ptr<Statement>> statement_list;
vector<shared_ptr<FunctionNode>> func_list;
vector<shared_ptr<VariableNode>> variable_list;
vector<string> native_func = { "print", "newline" };
class StatementList;
struct Statement
{
	StatementType type;
	vector<string> para;
	shared_ptr<StatementList> block;
	shared_ptr<StatementList> else_block=nullptr;
};

class StatementList
{
public:
	vector<shared_ptr<Statement>> statement_list;
};



struct VariableNode
{
	VariableNode(){}
	VariableNode(const string& _name, VariableType _type) :name(_name), type(_type){}
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
	shared_ptr<vector<VariableNode>> v_list; //局部变量表，参数同样添加进来
	shared_ptr<StatementList> s_list; //函数体语句

};
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
			executive_func_call(s);
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
		for (auto i = variable_list.begin(); i != variable_list.end(); ++i)
		{
			if ((*i)->name == s->para[0])
			{
				(*i)->u.double_type = eval(s->para[1],variable_list);
				break;
			}
			else if (i + 1 == variable_list.end())
			{
				throw s->para[0];
			}
		}
	}
	void Parser:: executive_native_func_call(shared_ptr<Statement> s)
	{
		string name = s->para[0];
		if (name == "print")
		{
			cout << eval(s->para[1], variable_list);
		}
		else if (name == "newline")
		{
			cout << endl;
		}
	}
	void Parser:: executive_func_call(shared_ptr<Statement> s)
	{
	}
	void Parser:: executive_while(shared_ptr<Statement> s)
	{
		double value = eval(s->para[0], variable_list);
		while (value != 0)
		{
			for (auto i : s->block->statement_list)
			{
				executive_statement(i);
			}
		 value = eval(s->para[0], variable_list);
		}
	}
	void Parser:: executive_for(shared_ptr<Statement> s)
	{
		executive_statement(s->else_block->statement_list[0]);
		while (eval(s->para[0],variable_list))
		{
			for (auto i : s->block->statement_list)
			{
				executive_statement(i);
			}
				executive_statement(s->else_block->statement_list[1]);
		}
	}
	void Parser:: executive_if(shared_ptr<Statement> s)
	{
		double value = eval(s->para[0], variable_list);
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

	void Parser:: assembly_statement()
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
	shared_ptr<Statement> Parser::deal_var_extern()
	{
		string expression;
		auto var_node = make_shared<VariableNode>();
		scanner.GetNextToken();
		string name = scanner.CurrToken;
		scanner.GetNextToken(); //  =/;
		if (scanner.ICurrToken == TOKEN_SEMI) //无初始值
		{
			expression = "0";
		}
		else
		{
			expression = deal_expression();
		}
		var_node->name = name;
		var_node->u.double_type = eval(expression, variable_list);

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
			statement_node = deal_var_extern();
			break;
		case TOKEN_FUNC:
			statement_node = deal_func_extern();
			break;
		}
		return statement_node;
	}
	shared_ptr<Statement> Parser::deal_func_extern()
	{
		auto func_statement = make_shared<FunctionNode>();

		return nullptr;

	}
	shared_ptr<vector<VariableNode>> deal_func_extern_para()
	{
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
		func_statement->para.push_back(_name);
		if (find(native_func.begin(), native_func.end(), _name) != native_func.end()) //native func
		{
			func_statement->type = STATEMENT_NATIVE_FUNC_CALL;
			func_statement->para.push_back(deal_expression());
		}
		else
		{
			func_statement->type = STATEMENT_FUNC_CALL;
			func_statement->para.push_back(deal_expression());
		}
		return func_statement;
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
	string Parser::deal_expression() // i为默认左括号数，用于适应左侧无括号而右部
	{
		string expression = "";
		Token tmp;
		int num = 0;
		bool flag = false;
		while ((tmp = scanner.GetNextToken()) != TOKEN_SEMI&&tmp!=TOKEN_COMMA)
		{
			if (scanner.ICurrToken == TOKEN_OPEN_BRACKET)
			{
				flag = true;
				++num;
			}
			else if (scanner.ICurrToken == TOKEN_CLOSE_BRACKET)
			{
				if (num == 0)
					break;
				else
				--num;
			}
			expression += scanner.CurrToken;
			if (num == 0 && flag ==true)
				break;

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




	int main()
	{
	//	cout << "--------------------" << endl;
		cout << "script is running..."<<endl;
		cout << "--------------------"<<endl;
		Parser a("D:\\1.txt");
		a.Assembly();
		a.run();
		cin.get();
	}