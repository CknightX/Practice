#include<iostream>
#include<vector>
#include<string>
#include "parser.h"
#include "eval.h"
#include "test.h"
int main()
{

	std::string code = "(define x (cons 1 (cons 2 3))) (car (cdr x))";
	std::string code1 = "(+ 1 (* 2 3))";
	Parser a(code);
	Env* env = new Env;
	Eval b(env);
	b.create_base_env();
	auto source = a.get_all_ast(); 
	Type* result = nullptr;
	for (auto code_line : source)
	{
		b.convert_scheme2str(b.eval(code_line));
	}
	std::cin.get();
}