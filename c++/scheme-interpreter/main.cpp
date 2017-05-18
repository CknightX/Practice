#include<iostream>
#include<vector>
#include<string>
#include "parser.h"
#include "eval.h"
using namespace std;
int main()
{
	string code1 = "\
				   		(define x (lambda (a b) (lambda (c) (+ a b c))))\
								(define y (x 1 2))\
										(y 3)\
												";
	string code2 = "(define x (lambda (a b) (+ a b))) (x 1 2)";
	Parser a(code1);
	Env* env = new Env;
	Eval b(env);
	auto source = a.get_all_ast(); 
	Type* result = nullptr;
	for (auto code_line : source)
	{
		result=b.eval(code_line);
	}
	result;
	cin.get();
}