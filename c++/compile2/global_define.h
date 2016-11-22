/*
ȫ�����Ͷ���
*/
#ifndef GLOBAL_DEFINE
#define GLOBAL_DEFINE
#include<vector>
#include<memory>
#include<stack>
#include<iostream>
using namespace std;

enum StatementType{STATEMENT_IF, STATEMENT_FOR,
	STATEMENT_WHILE, STATEMENT_FUNC_CALL, STATEMENT_NATIVE_FUNC_CALL, STATEMENT_VARIABLE_ASSIGN}; //�������
enum VariableType{TYPE_DOUBLE,TYPE_STRING,TYPE_NULL};  //��������

struct VariableNode;
struct FunctionNode;
struct Statement;
struct StatementList;

typedef shared_ptr<Statement> p_Statement;
typedef shared_ptr<FunctionNode> p_FunctionNode;
typedef shared_ptr<VariableNode> p_VariableNode;

typedef vector<p_VariableNode> v_Variable;
typedef vector<p_FunctionNode> v_Function;
typedef vector<p_Statement>    v_Statement;

struct Statement
{
	StatementType type;
	vector<string> para;
	shared_ptr<StatementList> block = nullptr;
	shared_ptr<StatementList> else_block = nullptr;
};

struct StatementList
{
	vector<shared_ptr<Statement>> statement_list;
};

struct VariableNode //�����ڵ�
{
	VariableNode(){}
	string name;
	VariableType type;
	friend ostream& operator<<(ostream& o, const VariableNode& v)
	{
		switch (v.type)
		{
		case TYPE_DOUBLE:
			o << v.u.double_type;
			break;
		case TYPE_STRING:
			o << v.u.string_type;
			break;
		}
		return o;
	}
	VariableNode& operator=(const VariableNode& v)
	{
		type = v.type;
		u = v.u;
		return *this;
	}
	struct _u
	{
		double double_type;
		string string_type;
	}u;
};

struct FunctionNode //�����ڵ�
{
	string name;
	shared_ptr<vector<shared_ptr<VariableNode>>> v_list; //�ֲ�����������ͬ����ӽ���
	shared_ptr<StatementList> s_list; //���������
	FunctionNode(const FunctionNode& fc) //��������
	{
		name = fc.name;
		s_list = fc.s_list;
		auto tmp_v_list = make_shared<v_Variable>();
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

#endif