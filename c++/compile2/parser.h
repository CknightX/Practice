#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include<memory>
#include<string>
#include<vector>
#include<stack>
#include "scanner.h"
using namespace std;

enum StatementType{
	STATEMENT_IF,STATEMENT_FOR, STATEMENT_WHILE, ASSIGN, FOR, STATEMENT_FUNC_CALL, STATEMENT_NATIVE_FUNC_CALL, STATEMENT_VARIABLE_ASSIGN
};

enum VariableType
{
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_NULL
};
class StatementList;
struct Statement;
class StatementList;
struct VariableNode;
struct FunctionNode;



extern vector<shared_ptr<Statement>> statement_list; //����
extern vector<shared_ptr<FunctionNode>> func_list; //������
extern vector<shared_ptr<VariableNode>> variable_list; //ȫ�ֱ�����
extern vector<string> native_func;  //ԭ��������



vector<shared_ptr<FunctionNode>>::iterator find_func_by_name(string _name);

class Parser
{
public:
	Parser(string path);
	void Assembly();
	void run();
	void _test();
private: 
	Scanner scanner;

	bool is_in_func = false; //�Ƿ��ں�������

	// һ��ɨ�����ɱ���������
	void assembly_statement();

	// ִ��
	void executive_statement(shared_ptr<Statement> s);
	void executive_variable_assign(shared_ptr<Statement> s);
	void executive_native_func_call(shared_ptr<Statement> s);
	void executive_func_call(shared_ptr<Statement> s);
	void executive_while(shared_ptr<Statement> s);
	void executive_for(shared_ptr<Statement> s);
	void executive_if(shared_ptr<Statement> s);


	//statement_deal
	string deal_expression(int mode=0);

	shared_ptr<Statement> deal_if();
	shared_ptr<Statement> deal_while();
	shared_ptr<Statement> deal_var_extern(vector<shared_ptr<VariableNode>> &variable_list);
	shared_ptr<Statement> deal_statement(Token begin_state);
	shared_ptr<Statement> deal_func_extern();
	shared_ptr<Statement> deal_for();
	shared_ptr<Statement> deal_func_call(string _name);
	shared_ptr<Statement> deal_var_assign(string _name);
	shared_ptr<StatementList> deal_block();  //����
};
#endif