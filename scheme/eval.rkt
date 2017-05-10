#lang scheme
(define (eval exp env)
  (cond
    ((self-evaluating? exp) exp)  ;自求值
    ((variable? exp) (lookup-variable-value exp env)) ;变量
    ((quoted? exp) (text-of-quotation exp)) ;引用
    ((assignment? exp) (eval-assignment exp env)) ;赋值
    ((definition? exp) (eval-definition exp env)) ;定义
    ((if? exp) (eval-if exp env)) ;if语句
    ((lambda? exp) ;lambda
     (make-procedure (lambda-parameters exp)
                     (lambda-body exp)
                     env))
    ((begin? exp) ;begin
     (eval-sequence (begin-actions exp) env))
    ((cond? exp) (eval(cond->if exp) env)) ;cond
    ((application? exp)
     (apply (eval (operator exp) env)
            (list-of-values? (operands exp) env)))
    (else
     (error "Unknown expression type --EVAL" exp))
    ))


(define (apply procedure arguments)
  (cond
    ((primitive-procedure? procedure)
     (apply-primitive-procedure procedure arguments))
    ((compound-procedure? procedure)
     (eval-sequence
      (procedure-body procedure)
      (extend-environment
       (procedure-parameters procedure)
       arguments
       (procedure-environment procedure))))
    (else
     (error
      "Unknown procedure type --APPLY" procedure))
    
    ))

(define (list-of-values? exps env) ;以组合式的运算对象为参数，求值各个运算对象，返回这些值的表
  (if (no operands? exps)
      '()
      (cons
       (eval (first-operand exps) env)
       (list-of-values (rest-operands exps) env)
       )
      ))

(define (eval-if exp env) ;求值-if语句
  (if (true? (eval (if-predicate exp) env)) ;条件为真?
      (eval (if-consequent exp) env)
      (eval (if-alternative exp) env)))

(define (eval-sequence exps env) ;求值-过程体里的表达式序列
  (cond
    ((last-exp? exps exps) (eval (first-exp exps) env))
    (else
     (eval (first-exp exps) env)
     (eval-sequence (rest-exps exps) env))
    ))

(define (eval-assignment exp env) ;求值-变量赋值
  (set-variable-value! (assignment-variable exp) ;取得变量
                       (eval (assignment-value exp) env) ;计算值
                       env)
  'ok)

(define (eval-definition exp env) ;求值-变量定义
  (define-variable! (definition-variable exp)
                    (eval (definition-value exp) env))
  'ok)

(define (self-evaluating? exp) ;自求值?
  (cond
    ((number? exp) true)
    ((string? exp) true)
    (else false)
    ))

(define (variable? exp) ;变量?
  (symbol? exp))

(define (quoted? exp) ;引号表达式?
  (tagged-list? exp 'quote))

(define (text-of-quotation exp) (cadr exp)) ;获取引号表达式引用的值

(define (tagged-list? exp tag) ;获取list的tag
  (if (pair? exp)
      (eq? (car exp) tag)
      false))

(define (assignment? exp) ;赋值语句?
  (tagged-list? exp 'set!))

(define (assignment-variable exp) (cadr exp)) ;获取赋值表达式的被赋值变量
(define (assignment-value exp) (caddr exp)) ;获取赋值表达式要赋的值

(define (definition? exp) (tagged-list? exp 'define)) ;定义语句?

(define (definition-variable exp) ;获取定义式的变量
  (if (symbol? (cadr exp))
      (cadr exp)
      (caddr exp)))

(define (definition-value exp) ;获取定义式的值
  (if (symbol? (cadr exp)) ;如果是变量,说明为(define <var> <value>)形式
      (caddr exp)
      (make-lambda
       (cdadr exp) ;lambda参数
       (cddr exp)  ;lambda函数体
       )))

(define (lambda? exp) (tagged-list? exp 'lambda)) ;lambda?
(define (lambda-parameters exp) (cadr exp)) ;lambda的参数
(define (lambda-body exp) (caddr exp)) ;lambda的函数体

(define (make-lambda parameters body) ;lambda构造函数
  (cons 'lambda (cons parameters body)))

(define (if? exp) (tagged-list? exp 'if))
(define (if-predicate exp) (cadr exp)) ;获取if语句的条件式
(define (if-consequent exp) (caddr exp)) ;获取if为真时的表达式
(define (if-alternative exp)
  (if (not (null? (cdddr exp))) ;if为假时是否存在表达式
      (cadddr exp) ;存在
      'false)) ;不存在,返回'false
(define (make-if predicate consequent alternative) ;if构造函数
  (list 'if predicate consequent alternative))

(define (begin? exp) (tagged-list? exp 'begin))
(define (begin-actions exp) (cdr exp))
(define (last-exp? seq) (null? (cdr sqe))) ;为list中最后一个exp?
(define (first-exp seq) (car seq))
(define (rest-exps seq) (cdr seq))
(define (sequence->exp seq) ;序列转变为一个表达式
  (cond
    ((null? seq) seq)
    ((last-exp? seq) (first-exp seq))
    (else (make-begin seq)) ;将一个表达式序列转变为begin表达式
    ))

(define (make-begin seq) (cons 'begin seq))

(define (application? exp) (pair? exp))

(define (operator exp) (car exp)) ;(operator <pr1> <pr2> ...)
(define (operands exp) (cdr exp))
(define (no-operands? ops) (null? ops)) ;不含有参数?
(define (first-operand ops) (car ops))
(define (rest-operands ops) (cdr ops))


(define (cond? exp) (tagged-list? exp 'cond))
(define (cond-clauses exp) (cdr exp))
(define (cond-else-clause? clause) ;是否为cond中的else
  (eq? (cond-predicate clause) (cdr clause)))
(define (cond-predicates clause) (car clause)) ;clause的条件部分
(define (cond-actions clause) (cdr clause)) ;clause的结果部分

(define (cond->if exp) ;cond->if
  expand-clauses (cond-clauses exp))

(define (expand-clauses clauses)
  (if (null? clauses)
      'false
      (let ((first (car clauses))
        (rest (cdr clauses)))
        (if (cond-else-clauses? first)
            (if (null? rest)
                (sequence->exp (cond-actions first))
                (error "ELSE clause isn't last --COND->IF"
                       clauses))
            (make-if (cond-predicate first)
                     (sequence->exp (cond-actions first))
                     (expand-clauses rest))))))

(define (true? x)
  (not (eq? x false)))
(define (false? x)
  (eq? x false))

(define (make-procedure parameters body env) ;过程构造
  (list 'procedure parameters body env))

(define (compound-procedure? p)
  (tagged-list? p 'procedure))

(define (procedure-parameters p) (cadr p))
(define (procedure-body p) (caddr p))
(define (procedure-environment p) (cadddr p))

(define (enclosing-environment env) (cdr env)) ;外围环境
(define (first-frame env) (car env))
(define the-empty-environment '())

(define (make-frame variables values)
  (cons variables values))
(define (frame-variables frame) (car frame))
(define (frame-values frame) (cdr frame))
(define (add-binding-to-frame! var val frame)
  (set-car! frame (cons var (car frame)))
  (set-cdr! frame (cons val (cdr frame))))
(define (extend-environment vars vals base-env)
  (if (= (length vars) (length vals))
      (cons (make-frame vars vals) base-env)
      (error "Wrong Num of arguments supplied" vars vals)))

(define (lookup-variable-value var env)
  (define (env-loop env)
    (define (scan vars vals)
      (cond ((null? vars)
             (env-loop (enclosing-environment env))) ;当前frame无所找变量,则去外围环境寻找
            ((eq? var (car vars)) (car vals))
            (else (scan (cdr vars) (cdr vals)))))
    (if (eq? env the-empty-environment)
        (error "Unbound variable" var)
        (let ((frame (first-frame env)))
          (scan (frame-variables frame)
                (frame-values frame)))))
  (env-loop env))

(define (set-variable-value! var val env)
  (define (env-loop env)
    (define (scan vars vals)
      (cond ((null? vars)
             (env-loop (enclosing-environment env)))
            ((eq? var (car vars))
             (set-car! vals val))
            (else (scan (cdr vars) (cdr vals)))))
    (if (eq? env the-empty-environment)
        (error "Unbound variable -- SET!" var)
        (let ((frame (first-frame env)))
          (scan (frame-variable frame)
                (frame-values frame)))))
  (env-loop env))
 
(define (define-varialbe! var val env)
  (let ((frame (first-frame env)))
    (define (scan vars vals)
      (cond ((null? vars)
             (add-binding-to-frame! var val frame))
            ((eq? var (car vars))
             (set-car! vals val))
            (else (scan (cdr vars) (cdr vals)))))
    (scan (frame-variables frame)
          (frame-values frame))))

(define (setup-environment)
  (let ((initial-env
         (extend-environment(primitive-procedure-names)
                            (primitive-procedure-objects)
                            the-empty-environment)))))
(define the-global-environment (setup-environment))

(define (primitive-procedure? proc) ;为原始过程？
  (tagged-list? proc 'primitive))
(define (primitive-implementation proc) (cadr proc))

(define primitive-procedures
  (list (list 'car car)
        (list 'cdr cdr)
        ()))