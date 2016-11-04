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

shared_ptr<FunctionNode> tmp_func; //���浱ǰ�����õĺ���

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
	vector<shared_ptr<VariableNode>> v_list; //�ֲ�����������ͬ����ӽ���
	shared_ptr<StatementList> s_list; //���������

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
		cout << "----------------����-------------\n";
		scanner._TestPrint();
		cout << endl;
		cout << "---------------------------------\n";
		cout << "---------------������------------\n";
		for (auto i : func_list)
		{
			cout << i->name << endl;
		}
		cout << endl;
		cout << "---------------------------------\n";
		cout << "---------------������------------\n";
		for (auto i : variable_list)
		{
			cout << i->name << '=' << i->u.double_type << endl;
		}
		cout << endl;

		cout << "---------------------------------\n";
		cout << "---------------����------------\n";
		cout << "---------------------------------\n";
		for (auto i : statement_list)
		{
			cout << i->type;
		}
		cout << endl;

	}
	// ִ��
	void Parser:: executive_statement(shared_ptr<Statement> s)
	{
		switch (s->type)
		{
		case STATEMENT_IF:
			executive_if(s);
			break;
		case STATEMENT_FUNC_CALL:
			is_in_func = true;
			executive_func_call(s);
			is_in_func = false;
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
			_variable_list = tmp_func->v_list;
		else
			_variable_list = variable_list;
		for (auto i = variable_list.begin(); i != variable_list.end(); ++i)
		{
			if ((*i)->name == s->para[0])
			{
				(*i)->u.double_type = eval(s->para[1],_variable_list);
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
		vector<shared_ptr<VariableNode>> _variable_list;
		if (is_in_func)
			_variable_list = tmp_func->v_list;
		else
			_variable_list = variable_list;
		string name = s->para[0];
		if (name == "print")
		{
			cout << eval(s->para[1], _variable_list);
		}
		else if (name == "newline")
		{
			cout << endl;
		}
	}
	void Parser:: executive_func_call(shared_ptr<Statement> s)
	{
		is_in_func = true; //�ں�������
		for (auto i : func_list)
		{
			if (i->name == s->para[0])
			{
				for (auto j : i->s_list->statement_list)
				{
					executive_statement(j);
				}
				return;
			}
		}
		throw;
	}
	void Parser:: executive_while(shared_ptr<Statement> s)
	{
		vector<shared_ptr<VariableNode>> _variable_list;
		if (is_in_func)
			_variable_list = tmp_func->v_list;
		else
			_variable_list = variable_list;
		double value = eval(s->para[0], _variable_list);
		while (value != 0)
		{
			for (auto i : s->block->statement_list)
			{
				executive_statement(i);
			}
		 value = eval(s->para[0], _variable_list);
		}
	}
	void Parser:: executive_for(shared_ptr<Statement> s)
	{
		vector<shared_ptr<VariableNode>> _variable_list;
		if (is_in_func)
			_variable_list = tmp_func->v_list;
		else
			_variable_list = variable_list;
		executive_statement(s->else_block->statement_list[0]);
		while (eval(s->para[0],_variable_list))
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
		vector<shared_ptr<VariableNode>> _variable_list;
		if (is_in_func)
			_variable_list = tmp_func->v_list;
		else
			_variable_list = variable_list;
		double value = eval(s->para[0], _variable_list);
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

	// һ��ɨ�����ɱ���������

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
		scanner.GetNextToken(); // �Ƿ���else
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
	shared_ptr<Statement> Parser::deal_var_extern(vector<shared_ptr<VariableNode>> &variable_list) //��ӽ���һ��������
	{
		vector<shared_ptr<VariableNode>> _variable_list;
		if (is_in_func)
			_variable_list = tmp_func->v_list;
		else
			_variable_list = variable_list;
		string expression;
		auto var_node = make_shared<VariableNode>();
		scanner.GetNextToken();
		string name = scanner.CurrToken;
		scanner.GetNextToken(); //  =/;
		if (scanner.ICurrToken == TOKEN_SEMI || scanner.ICurrToken == TOKEN_CLOSE_BRACKET || scanner.ICurrToken == TOKEN_COLON) //�޳�ʼֵ
		{
			expression = "0";
		}
		else
		{
			expression = deal_expression();
		}
		var_node->name = name;
		variable_list.push_back(var_node);
		var_node->u.double_type = eval(expression, _variable_list);

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
				scanner.PutToken();  // ��( �Żش�����
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
		string name = scanner.CurrToken;
		scanner.GetNextToken(); //(
		while (1)
		{
			auto i = scanner.GetNextToken();
			if (i == TOKEN_OPEN_BRACE || i == TOKEN_CLOSE_BRACKET)
				break;
			deal_var_extern((func_node->v_list)); //����������������ʱ������
		}
		scanner.PutToken();
		auto _statement_list = make_shared<StatementList>();
		_statement_list = deal_block();
		func_node->name = name;
		func_node->s_list = _statement_list;
		func_list.push_back(func_node);
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
		scanner.GetNextToken(); // ��ȡ������
		auto left_statement = deal_statement(scanner.ICurrToken); // ��ำֵ
		auto mid_expression = deal_expression(); //�в����ʽ
		scanner.GetNextToken(); // ��ȡ������
		auto right_statement = deal_statement(scanner.ICurrToken); //�Ҳำֵ �������ű�ƥ��
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
		scanner.GetNextToken(); // (
		auto i = find_func_by_name(_name);
		if (i == func_list.end())
			throw;
		int j = 0;
		while (scanner.GetNextToken() != TOKEN_SEMI)
		{
			string p = deal_expression();
			((*i)->v_list[j])->u.double_type = eval(p, variable_list);
			((*i)->v_list[j++])->type = TYPE_DOUBLE;
		}
		func_statement->para.push_back(_name);
		if (find(native_func.begin(), native_func.end(), _name) != native_func.end()) //native func
		{
			func_statement->type = STATEMENT_NATIVE_FUNC_CALL;
		}
		else
		{
			func_statement->type = STATEMENT_FUNC_CALL;
		}
		return func_statement;
	}

	vector<shared_ptr<FunctionNode>>::iterator find_func_by_name(string _name)
	{
		for (auto i = func_list.begin(); i != func_list.end(); ++i)
		{
			if ((*i)->name == _name) //�ҵ�
			{
				return i;
			}
		}
		return func_list.end();
	}
	shared_ptr<StatementList> Parser::deal_block()  //����
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
	string Parser::deal_expression(int mode) // mode Ϊģʽ
	{
		string expression = "";
		Token tmp;
		int num = 0;
		bool flag = false;
		if (mode==0) //Ĭ��ģʽ 
		while ((tmp = scanner.GetNextToken()) != TOKEN_SEMI&&tmp!=TOKEN_COMMA)
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




	int main()
	{
		cout << "--------------------" << endl;
		cout << "script is running..."<<endl;
		cout << "--------------------"<<endl;
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