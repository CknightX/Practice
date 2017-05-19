#ifndef _TYPE_H
#define _TYPE_H
#include<vector>
#include<string>
#include "env.h"

/*  ��������
	���������������壬lambda��begin��if��cond��

*/
class Type;
class Env;


enum TYPE
{
	APPLY,
	DEFINE,
	PROCEDURE,
	BEGIN,
	VARIABLE,
	IF,

	BASE_TYPE_TRUE,
	BASE_TYPE_FALSE,
	BASE_TYPE_DOUBLE,
	BASE_TYPE_STRING,

	BASE_PROCEDURE_ADD,
	BASE_PROCEDURE_SUB,
	BASE_PROCEDURE_MUL,
	BASE_PROCEDURE_DIV,
	BASE_PROCEDURE_G,
	BASE_PROCEDURE_L,
	BASE_PROCEDURE_E,
	BASE_PROCEDURE_GE,
	BASE_PROCEDURE_LE,
	BASE_PROCEDURE_CONS,
	BASE_PROCEDURE_BEGIN,

};


/*
	����ֵ����
*/
class Type //���ͻ���
{
public:
	Type(TYPE type) :type_info(type){}
	//virtual Type* eval(Env* outer_env) = 0;
	virtual ~Type()=0 {}
	TYPE type_info;

};

class Type_True :public Type //True����
{
public:
	Type_True() :Type(BASE_TYPE_TRUE){}

	
};
class Type_False :public Type //False����
{
public:
	Type_False() :Type(BASE_TYPE_FALSE){}

};

class Type_Define :public Type  //Define���� --> (define ... ...)
{
public:
	Type_Define(const std::string& name,Type* value) :Type(DEFINE),name(name),value(value){}

	std::string name;
	Type* value;

};
class Type_BaseType :public Type //�������� --> double string
{
public:
	Type_BaseType(TYPE type) :Type(type){}
	union _u
	{
		int value_int;
		double value_double;
	}u;

};

class Type_BaseProcedureApply :public Type //��������apply --> + - * / ...
{
public:
	Type_BaseProcedureApply(TYPE type) :Type(type){}
	parms_value_list parms;
};

class Type_Variable :public Type //�������� 
{
public:
	Type_Variable(const std::string& name) :Type(VARIABLE),name(name),value(value){}
	std::string name;
	Type* value;
};

class Type_Procedure :public Type //�������� --> (lambda <parms> <procedure> )
{
public:
	Type_Procedure(parms_name_list& parms, Type* body)
		:Type(PROCEDURE), parms(parms), body(body){}


public:
	parms_name_list parms;
	Type* body;
	Env* outer_env;
};


class Type_Apply:public Type //apply���� --> (<procedure> <parms>)
{
public:
	Type_Apply(const std::string& name,parms_value_list& parms) 
		:Type(APPLY),name(name),parms(parms){}
	std::string name;
	parms_value_list parms;

};

class Type_If :public Type //if���� --> (if condition conseq alter)
{
public:
	Type_If(Type* condition,Type* conseq,Type* alter) 
		:Type(IF),condition(condition),conseq(conseq),alter(alter){}

	Type* condition;
	Type* conseq;
	Type* alter;

};






#endif