#ifndef _EVAL_H
#define _EVAL_H
#include "env.h"
#include<string>
/*求值器*/

class Eval
{
public:
	Eval(Env* base_env = nullptr) :base_env(base_env){}
	Type* eval(Type* type,Env* env);
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
	Type* eval_base_procedure(Type* type, Env* env);

};
#endif