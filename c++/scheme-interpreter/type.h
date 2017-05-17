#ifndef _TYPE_H
#define _TYPE_H
#include<vector>
#include<string>
#include "env.h"

/*  基本类型
	包含：变量，定义，lambda，begin，if，cond，

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
	TRUE,
	FALSE,

	BASE_TYPE,
	BASE_TYPE_INT,
	BASE_TYPE_DOUBLE,
	BASE_TYPE_STRING,

	BASE_PROCEDURE_ADD,
	BASE_PROCEDURE_SUB,
	BASE_PROCEDURE_MUL,
	BASE_PROCEDURE_DIV,

};

enum BASE_TYPE_
{
	INT,DOUBLE,STRING
};

class Type
{
public:
	Type(TYPE type) :type_info(type){}
	//virtual Type* eval(Env* outer_env) = 0;
	virtual ~Type()=0 {}
	TYPE type_info;

};

class Type_True :public Type
{
public:
	Type_True() :Type(TRUE){}

	
};
class Type_False :public Type
{
public:
	Type_False() :Type(FALSE){}

};

class Type_Define :public Type
{
public:
	Type_Define(const std::string& name,Type* value) :Type(DEFINE),name(name),value(value){}

	std::string name;
	Type* value;

};
class Type_BaseType :public Type
{
public:
	Type_BaseType(TYPE type) :Type(type){}
	union _u
	{
		int value_int;
		double value_double;
	}u;



};

class Type_BaseProcedureApply :public Type
{
public:
	Type_BaseProcedureApply(TYPE type) :Type(type){}

	parms_value_list parms;
};

class Type_Variable :public Type
{
public:
	Type_Variable(const std::string& name) :Type(VARIABLE),name(name),value(value){}
	std::string name;
	Type* value;
};

class Type_Procedure :public Type
{
public:
	Type_Procedure(parms_name_list& parms, Type* body)
		:Type(PROCEDURE), parms(parms), body(body){}


public:
	parms_name_list parms;
	Type* body;
	Env* outer_env;
};


class Type_Apply:public Type
{
public:
	Type_Apply(const std::string& name,parms_value_list& parms) 
		:Type(APPLY),name(name),parms(parms){}
	std::string name;
	Type_Procedure* procedure;
	parms_value_list parms;

};

class Type_If :public Type
{
public:
	Type_If() :Type(IF){}

private:
	Type* condition;
	Type* conseq;
	Type* alter;

};






#endif