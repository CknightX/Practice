#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include<memory>
#include<string>
#include<vector>
#include<stack>
#include<queue>
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

extern stack<shared_ptr<FunctionNode>> tmp_func; //保存当前所调用的函数
extern int is_in_func; //当前是否在执行函数

extern vector<shared_ptr<Statement>> statement_list; //语句表
extern vector<shared_ptr<FunctionNode>> func_list; //函数表
extern vector<shared_ptr<VariableNode>> variable_list; //全局变量表
extern vector<string> native_func;  //原生函数表



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

	// 一遍扫描生成变量函数表
	void assembly_statement();

	// 执行
	void executive_statement(shared_ptr<Statement> s);
	void executive_variable_assign(shared_ptr<Statement> s);
	void executive_native_func_call(shared_ptr<Statement> s);
	void executive_func_call(shared_ptr<Statement> s);
	void executive_while(shared_ptr<Statement> s);
	void executive_for(shared_ptr<Statement> s);
	void executive_if(shared_ptr<Statement> s);
	void _add_native_func(string name, int i);
	void init_native_func(); //初始化原生函数表
	//statement_deal
	string deal_expression(int mode=0);
	shared_ptr<vector<shared_ptr<VariableNode>>> deal_parameter_extern();
	void deal_parameter_call(string func_name);
	shared_ptr<Statement> deal_if();
	shared_ptr<Statement> deal_while();

	shared_ptr<Statement> deal_var_extern(vector<shared_ptr<VariableNode>> &variable_list);
	shared_ptr<Statement> deal_statement(Token begin_state);
	shared_ptr<Statement> deal_func_extern();
	shared_ptr<Statement> deal_for();
	shared_ptr<Statement> deal_func_call(string _name);
	shared_ptr<Statement> deal_var_assign(string _name);
	shared_ptr<StatementList> deal_block();  //语句块
};
#endif