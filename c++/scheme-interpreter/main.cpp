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
	std::string code4 = "(cdr 1)";
	std::string code5 = "(cons (cons 1 2) 3)";
	std::string code6 = "(list (list 1 2) 3)";
	std::string code7 = "(cdr (list 1 2 3 4))";
	std::string code8 = "(cons (cons 1 2) (cons 3 4))";
	std::string code9 = "(define length (lambda (items) (if (null? items) 0 (+ 1 (length (cdr items))))))\
		(length(list 1 2 3))";
	std::string code10 = "(define append (lambda (list1 list2) (if (null? list1) list2 (cons (car list1) (append (cdr list1) list2)))))\
		(append(list 1 2) (list 3 4))";
	Parser a(code10);
	Env* env = new Env;
	Eval b(env);
	b.create_base_env();	//创建基础环境
	auto source = a.get_all_ast(); 
	Type* result = nullptr;
	try
	{
		for (auto code_line : source)
		{
			std::string result = b.convert_scheme2str(b.eval(code_line));
			if (result != "")
				std::cout << result << std::endl;
			//result = b.eval(code_line);
		}
	}
	catch (const char* str)
	{
		std::cerr << str;
	}
	std::cin.get();
}