#include "eval.h"
#include "type.h"
Type* Eval::eval(Type* type, Env* env)
{
	Type* result = nullptr;
	switch (type->type_info)
	{
	case APPLY:
		result = eval_apply(type,env);
		break;
	case DEFINE:
		result = eval_define(type, env);
		break;
	case PROCEDURE:
		result = eval_procedure(type, env);
		break;
	case BEGIN:
		result = eval_begin(type, env);
		break;
	case VARIABLE:
		result = eval_variable(type, env);
		break;
	case IF:
		result = eval_if(type, env);
		break;
	case BASE_PROCEDURE_ADD:
		result = eval_base_procedure(type, env);
		break;
	}
	return result;
}



Type* Eval::eval_procedure(Type* type,Env* env) //过程定义
{
	auto exp_procedure = static_cast<Type_Procedure*>(type);
	exp_procedure->outer_env = env;
	return type;
}


Type* Eval::eval_apply(Type* type, Env* env)
{
	auto exp_apply = static_cast<Type_Apply*>(type);
	auto curr_env = new Env(exp_apply->procedure->parms, exp_apply->parms, env);
	return eval(exp_apply->procedure, curr_env);
}


Type* Eval::eval_define(Type* type, Env* env)
{
	auto type_define = static_cast<Type_Define*>(type);
	(*env).env[type_define->name] = eval(type_define->value,env);
	return nullptr;
}
Type* Eval::eval_base_procedure(Type* type, Env* env)
{
	auto base_procedure = static_cast<Type_BaseProcedureApply*>(type);
	switch (base_procedure->type_info)
	{
	case BASE_PROCEDURE_ADD:
		Type_BaseType* result = new Type_BaseType(BASE_TYPE_INT);
		result->u.value_int = 0;
		for (auto i : base_procedure->parms)
		{
			result->u.value_int += (static_cast<Type_BaseType*>(i))->u.value_int;
		}
		return result;
	}
}

Type* Eval::eval_if(Type* type, Env* env)
{

}

Type* Eval::eval_lambda(Type* type, Env* env)
{

}

Type* Eval::eval_variable(Type* type, Env* env)
{
	auto exp_variable = static_cast<Type_Variable*>(type);
	return eval(exp_variable->value,env);
}
