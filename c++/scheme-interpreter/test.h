#ifndef _TEST_H
#define _TEST_H
#include <string>


/* define 语句测试 */

// 基本类型 double
#define CODE1 "(define x 2.33) x"

// lambda
#define CODE2 "(define x (lambda (a b) (+ a b))) (x 1 2)";  // 3

// 闭包
#define CODE3 "(define x (lambda (a b) (lambda (x) (+ a b x))))\
				(define y (x 1 2))\
				(y 3)" // 6
// 递归-阶乘
#define CODE4 "(define x (lambda (a) (if (= a 1) 1 (* a (x (- a 1)))))) (x 4)"; // 24

//递归-斐波那契数列
#define CODE5 "(define fib (lambda (i) (if (= i 1) 1 (if (= i 2) 1 (+ (fib (- i 1)) (fib (- i 2))))))) (fib 20)"


//cons实现1
#define CODE6 "(define cond (lambda (x y) (lambda (m) (m x y))))\
				(define car (lambda (x) (x (lambda(m n) m))))\
				(define cdr (lambda (x) (x (lambda(m n) n))))\
				(car (cond 1 2))\
				(cdr (cond 1 2))"  // 1 2

//cons实现2
#define CODE7 "(define cond (lambda (x y) (lambda (m) (if (= m 0) x y))))\
					(define car(lambda(z) (z 0)))\
					(define cdr(lambda(z) (z 1)))\
					(car(cond 1 2))\
		   			(cdr(cond 1 2))" // 1 2																																																																																					   																								   				   ";
#endif