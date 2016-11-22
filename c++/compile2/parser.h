#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include<memory>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include "scanner.h"
#include"global_define.h"
#include"eval.h"





class Parser
{
public:
	Parser(string path);
	void Assembly();
	void run();
	void _test();
private: 
	void Init();
	vector<p_Statement> statement_list;
	v_Function func_list;
	v_Variable variable_list;
	vector<string> native_func;
	stack<shared_ptr<FunctionNode>> tmp_func; //  ����ջ
	Scanner scanner;
	v_Function::iterator find_func_by_name(string _name);
	// һ��ɨ�����ɱ���������
	void assembly_statement();

	// ִ��
	void executive_statement(p_Statement s);
	void executive_variable_assign(p_Statement s);
	void executive_native_func_call(p_Statement s);
	void executive_func_call(p_Statement s);
	void executive_while(p_Statement s);
	void executive_for(p_Statement s);
	void executive_if(p_Statement s);
	void _add_native_func(string name, int i);
	void init_native_func(); //��ʼ��ԭ��������
	//statement_deal
	string deal_expression();
	shared_ptr<v_Variable> deal_parameter_extern();
	void deal_parameter_call(string func_name);
	p_Statement deal_if();
	p_Statement deal_while();

	p_Statement deal_var_extern(v_Variable &variable_list);
	p_Statement deal_statement(Token begin_state);
	p_Statement deal_func_extern();
	p_Statement deal_for();
	p_Statement deal_func_call(string _name);
	p_Statement deal_var_assign(string _name);
	shared_ptr<StatementList> deal_block();  //����
};
#endif