#include "eval.h"
#include "type.h"
#include<iostream>
Type* Eval::eval(Type* type, Env* env)
{
	Type* result = nullptr;
	if (!type)
		return result;
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
	case BASE_TYPE_DOUBLE:
	case BASE_TYPE_CONS:
		result = eval_base_type(type, env);
		break;
	case BASE_PROCEDURE_ADD:
	case BASE_PROCEDURE_SUB:
	case BASE_PROCEDURE_MUL:
	case BASE_PROCEDURE_DIV:
	case BASE_PROCEDURE_G:
	case BASE_PROCEDURE_L:
	case BASE_PROCEDURE_E:
	case BASE_PROCEDURE_GE:
	case BASE_PROCEDURE_LE:
	case BASE_PROCEDURE_CONS:
	case BASE_PROCEDURE_LIST:
	case BASE_PROCEDURE_BEGIN:
	case BASE_PROCEDURE_CAR:
	case BASE_PROCEDURE_CDR:
		result = eval_base_procedure(type, env);
		break;
	}
	return result;
}



Type* Eval::eval_procedure(Type* type,Env* env) //过程定义
{
	auto exp_procedure = static_cast<Type_Procedure*>(type);
	exp_procedure->outer_env = env; //设置过程体的外部环境
	return exp_procedure;
}


Type* Eval::eval_apply(Type* type, Env* env)
{
	

	auto exp_apply = static_cast<Type_Apply*>(type);
	std::vector<Type*> value_parms;
	for (auto i : exp_apply->parms)
		value_parms.push_back(eval(i, env));  //对参数求值

	auto procedure = static_cast<Type_Procedure*>(env->find(exp_apply->name));

	switch (procedure->type_info)
	{
	case BASE_PROCEDURE_ADD:
	case BASE_PROCEDURE_SUB:
	case BASE_PROCEDURE_MUL:
	case BASE_PROCEDURE_DIV:
	case BASE_PROCEDURE_G:
	case BASE_PROCEDURE_L:
	case BASE_PROCEDURE_E:
	case BASE_PROCEDURE_GE:
	case BASE_PROCEDURE_LE:
	case BASE_PROCEDURE_CONS:
	case BASE_PROCEDURE_LIST:
	case BASE_PROCEDURE_BEGIN:
	case BASE_PROCEDURE_CAR:
	case BASE_PROCEDURE_CDR:
		Type* _procedure = procedure;
		auto base_apply = static_cast<Type_BaseProcedureApply*>(_procedure);
		base_apply->parms = value_parms;
		return eval(base_apply, env);
		break;
	}
	auto curr_env = new Env(procedure->parms, value_parms, procedure->outer_env);
	return eval(procedure->body, curr_env);
}


Type* Eval::eval_define(Type* type, Env* env)
{
	auto type_define = static_cast<Type_Define*>(type);
	(*env).env[type_define->name] = eval(type_define->value, env);
	return nullptr;
}
Type* Eval::eval_base_procedure(Type* type, Env* env)
{
	auto base_procedure = static_cast<Type_BaseProcedureApply*>(type);
	switch (base_procedure->type_info)
	{
	case BASE_PROCEDURE_ADD:
		return eval_base_procedure_add(base_procedure, env);
		break;
	case BASE_PROCEDURE_SUB:
		return eval_base_procedure_sub(base_procedure, env);
		break;
	case BASE_PROCEDURE_MUL:
		return eval_base_procedure_mul(base_procedure, env);
		break;
	case BASE_PROCEDURE_DIV:
		return eval_base_procedure_div(base_procedure, env);
		break;
	case BASE_PROCEDURE_G:
		return eval_base_procedure_greater(base_procedure, env);
		break;
	case BASE_PROCEDURE_L:
		return eval_base_procedure_less(base_procedure, env);
		break;
	case BASE_PROCEDURE_E:
		return eval_base_procedure_equal(base_procedure, env);
		break;
	case BASE_PROCEDURE_GE:
		return eval_base_procedure_greater_equal(base_procedure, env);
		break;
	case BASE_PROCEDURE_LE:
		return eval_base_procedure_less_equal(base_procedure, env);
		break;
	case BASE_PROCEDURE_CONS:
		return eval_base_procedure_cons(base_procedure, env);
		break;
	case BASE_PROCEDURE_BEGIN:
		return eval_base_procedure_begin(base_procedure, env);
		break;
	case BASE_PROCEDURE_CAR:
		return eval_base_procedure_car(base_procedure, env);
		break;
	case BASE_PROCEDURE_CDR:
		return eval_base_procedure_cdr(base_procedure, env);
		break;
	case BASE_PROCEDURE_LIST:
		return eval_base_procedure_list(base_procedure, env);
		break;
	}
}



Type* Eval::eval_base_type(Type* type, Env* env)
{
	return type;
}



Type* Eval::eval_if(Type* type, Env* env)
{
	auto exp_if = static_cast<Type_If*>(type);
	if (eval(exp_if->condition, env)->type_info == BASE_TYPE_TRUE)
	{
		return eval(exp_if->conseq, env);
	}
	else
	{
		return eval(exp_if->alter, env);
	}
}

Type* Eval::eval_lambda(Type* type, Env* env)
{
	return type;
}

Type* Eval::eval_variable(Type* type, Env* env)
{
	auto exp_variable = static_cast<Type_Variable*>(type);
	std::string name = exp_variable->name;
	return eval(env->find(name), env);
}
Type* Eval::eval_base_procedure_add(Type_BaseProcedureApply* base_procedure, Env* env)
{
	Type_BaseType* result = new Type_BaseType(BASE_TYPE_DOUBLE);
	result->u.value_double = 0;
	for (auto i : base_procedure->parms)
	{
		result->u.value_double += static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
	}
	return result;
}
Type* Eval::eval_base_procedure_sub(Type_BaseProcedureApply* base_procedure, Env* env)
{
	Type_BaseType* result = new Type_BaseType(BASE_TYPE_DOUBLE);
	result->u.value_double = 0;
	bool is_first = true;
	for (auto i : base_procedure->parms)
	{
		if (is_first)
		{
			result->u.value_double += static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
			is_first = false;
		}
		else
			result->u.value_double -= static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
	}
	return result;
}
Type* Eval::eval_base_procedure_mul(Type_BaseProcedureApply* base_procedure, Env* env)
{
	Type_BaseType* result = new Type_BaseType(BASE_TYPE_DOUBLE);
	result->u.value_double = 1;
	bool is_first = true;
	for (auto i : base_procedure->parms)
	{
		result->u.value_double *= static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
	}
	return result;
}
Type* Eval::eval_base_procedure_div(Type_BaseProcedureApply* base_procedure, Env* env)
{
	Type_BaseType* result = new Type_BaseType(BASE_TYPE_DOUBLE);
	result->u.value_double = 1;
	bool is_first = true;
	for (auto i : base_procedure->parms)
	{
		if (is_first)
		{
			result->u.value_double *= static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
			is_first = false;
		}
		else
			result->u.value_double /= static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
	}
	return result;
}
Type* Eval::eval_base_procedure_greater(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for >";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for >";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm > second_parm)
			return new Type_True;
		else
			return new Type_False;
	}
}
Type* Eval::eval_base_procedure_less(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for <";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for <";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm < second_parm)
			return new Type_True;
		else
			return new Type_False;
	}
}
Type* Eval::eval_base_procedure_equal(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for =";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for =";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm == second_parm)
			return new Type_True;
		else
			return new Type_False;
	}

}
Type* Eval::eval_base_procedure_greater_equal(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for >=";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for >=";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm >= second_parm)
			return new Type_True;
		else
			return new Type_False;
	}
}
Type* Eval::eval_base_procedure_less_equal(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for <=";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for <=";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm <= second_parm)
			return new Type_True;
		else
			return new Type_False;
	}
}
Type* Eval::eval_base_procedure_cons(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 2)
		throw "cons need 2 paras!";
	return new Type_Cons(base_procedure->parms[0], base_procedure->parms[1]);
}

Type* list2cons(parms_value_list& parms, size_t index)
{
	if (index == parms.size())
		return nullptr;
	else
	{
		return new Type_Cons(parms[index], list2cons(parms, index + 1));
	}
}


Type* Eval::eval_base_procedure_list(Type_BaseProcedureApply* base_procedure, Env* env)
{
	return list2cons(base_procedure->parms,0);
}

Type* Eval::eval_base_procedure_begin(Type_BaseProcedureApply* base_procedure, Env* env)
{
	for (auto iter = base_procedure->parms.begin(); iter != base_procedure->parms.end() - 1; ++iter)
		eval(*iter,env);
	return eval(*(base_procedure->parms.rbegin()), env);
}

Type* Eval::eval_base_procedure_car(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 1)
		throw "car need 1 parms!";
	auto cons_type = static_cast<Type_Cons*>(base_procedure->parms[0]);
	return eval(cons_type->left);
}
Type* Eval::eval_base_procedure_cdr(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 1)
		throw "cdr need 1 parms!";
	auto cons_type = static_cast<Type_Cons*>(base_procedure->parms[0]);
	return eval(cons_type->right);
}
void Eval::convert_scheme2str(Type* type)
{
	if (!type)
		return;
	switch (type->type_info)
	{
	case BASE_TYPE_DOUBLE:
		std::cout << static_cast<Type_BaseType*>(type)->u.value_double << std::endl;
		break;
	default:
		break;
	}
}
void Eval::create_base_env()
{
	//初始化基本环境
	base_env->env["+"] = new Type_BaseProcedureApply(BASE_PROCEDURE_ADD);
	base_env->env["-"] = new Type_BaseProcedureApply(BASE_PROCEDURE_SUB);
	base_env->env["*"] = new Type_BaseProcedureApply(BASE_PROCEDURE_MUL);
	base_env->env["/"] = new Type_BaseProcedureApply(BASE_PROCEDURE_DIV);
	base_env->env["="] = new Type_BaseProcedureApply(BASE_PROCEDURE_E);
	base_env->env[">"] = new Type_BaseProcedureApply(BASE_PROCEDURE_G);
	base_env->env["<"] = new Type_BaseProcedureApply(BASE_PROCEDURE_L);
	base_env->env[">="] = new Type_BaseProcedureApply(BASE_PROCEDURE_GE);
	base_env->env["<="] = new Type_BaseProcedureApply(BASE_PROCEDURE_LE);
	base_env->env["cons"] = new Type_BaseProcedureApply(BASE_PROCEDURE_CONS);
	base_env->env["list"] = new Type_BaseProcedureApply(BASE_PROCEDURE_LIST);
	base_env->env["begin"] = new Type_BaseProcedureApply(BASE_PROCEDURE_BEGIN);
	base_env->env["car"] = new Type_BaseProcedureApply(BASE_PROCEDURE_CAR);
	base_env->env["cdr"] = new Type_BaseProcedureApply(BASE_PROCEDURE_CDR);
}
