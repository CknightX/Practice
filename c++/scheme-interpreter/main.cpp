#include<iostream>
#include<vector>
#include<string>
#include "parser.h"
#include "eval.h"
#include "test.h"
int main()
{

	std::string code = "(define x (cons 1 (cons 2 3 4))) (car (cdr x))";
	std::string code1 = "(+ 1 (* 2 3))";
	std::string code2 = "(begin (define x 10) (* 3 4) (* x 5)) x";
	std::string code3 = "(define x (lambda (a b) (begin (define y 10) (+ a b y)))) (x 1 2) y";
	Parser a(code3);
	Env* env = new Env;
	Eval b(env);
	b.create_base_env();
	auto source = a.get_all_ast(); 
	Type* result = nullptr;
	try
	{
		for (auto code_line : source)
		{
			b.convert_scheme2str(b.eval(code_line));
			//result = b.eval(code_line);
		}
	}
	catch (const char* str)
	{
		std::cerr << str;
	}
	std::cin.get();
}