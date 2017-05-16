#ifndef _EVAL_H
#define _EVAL_H
#include "env.h"
#include<string>
/*��ֵ��*/

class Eval
{
public:
	Eval(Env* base_env = nullptr) :base_env(base_env){}
	Type* eval(Type* type,Env* env);
private:
	std::string scheme_type2str(Type* type); //��scheme������ת��Ϊ�ı�
	Env* base_env;
	void create_base_env();

	/*������ֵ*/
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