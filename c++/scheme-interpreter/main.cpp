#include<iostream>
#include<vector>
#include<string>
#include "parser.h"
#include "eval.h"
#include "test.h"
int main()
{
	std::string code = "(cdr (list (cons (list 1 2) (cons 3 4)) (list (cons 1 (cons 1 (list 1 2 3))) (cons 3 4))))";
	Parser a(code);
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