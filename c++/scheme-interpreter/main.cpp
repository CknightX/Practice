#include<iostream>
#include<vector>
#include<string>
#include "parser.h"
#include "eval.h"
using namespace std;
int main()
{
	string code1 = "\
				   	(define cond (lambda (x y) (lambda (m) (m x y))))\
					(define car (lambda (x) (x (lambda(m n) m))))\
					(define cdr (lambda (x) (x (lambda(m n) n))))\
					(car (cond 1 2))\
					(cdr (cond 1 2))\
		";
	string code2 = "\
		(define x (lambda (a b) (lambda (x) (+ a b x))))\
		(define y (x 10 20))\
		(define z (y 10))\
		z\
		";

	string code3 = "\
				   	(define cond (lambda (x y) (lambda (m) (if (= m 0) x y))))\
				   (define car(lambda(z) (z 0)))\
				   (define cdr(lambda(z) (z 1)))\
				   (car(cond 1 2))\
				   (cdr(cond 1 2))\
				   ";
	Parser a(code3);
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