#ifndef _EVAL_H
#define _EVAL_H
#include "env.h"
#include "type.h"
#include<string>
/*求值器*/

class Eval
{
public:
	Eval(Env* base_env = nullptr) :base_env(base_env){}

	Type* eval(Type* type,Env* env);
	Type* eval(Type* type){ return eval(type, base_env); }
private:
	std::string scheme_type2str(Type* type); //将scheme的类型转化为文本
	Env* base_env;
	void create_base_env();

	/*类型求值*/
	Type* eval_if(Type* type, Env* env);
	Type* eval_define(Type* type, Env* env);
	Type* eval_lambda(Type* type, Env* env);
	Type* eval_variable(Type* type, Env* env);
	Type* eval_begin(Type* type, Env* env){ return nullptr; }
	Type* eval_procedure(Type* type, Env* env);
	Type* eval_apply(Type* type, Env* env);
	Type* eval_base_type(Type* type, Env* env);

	// 基础过程求值
	Type* eval_base_procedure(Type* type, Env* env);
	Type* eval_base_procedure_add(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_sub(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_mul(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_div(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_greater(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_less(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_equal(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_greater_equal(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_less_equal(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_cons(Type_BaseProcedureApply* base_procedure, Env* env);
	Type* eval_base_procedure_begin(Type_BaseProcedureApply* base_procedure, Env* env);
};
#endif