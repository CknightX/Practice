/*
#include "lexer.h"
#include "parser.h"
#include "env.h"
#include"eval.h"
class A
{
public:
	A* hehe(){ return this; }
};
int main()
{
	Env* env = new Env;
	Eval eval(env);

	Type_Define* exp = new Type_Define;
	Type_BaseType* a = new Type_BaseType(BASE_TYPE_INT);
	Type_BaseType* b = new Type_BaseType(BASE_TYPE_INT);

	a->u.value_int = 1;
	b->u.value_int = 2;

	Type_BaseProcedureApply* add = new Type_BaseProcedureApply(BASE_PROCEDURE_ADD);
	add->parms.push_back(a);
	add->parms.push_back(b);

	exp->name = "haha";
	exp->value = add;

	auto c=eval.eval(exp,env);


	return 0;
}
*/
#include<iostream>
#include<vector>
#include<string>
#include "parser.h"
#include "eval.h"
using namespace std;
int main()
{
	Parser a("(define y (* 2 2)");
	Env* env = new Env;
	Eval b(new Env);
	auto ast = a.get_ast();
	b.eval(ast, env);
	cin.get();
}