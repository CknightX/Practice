# -*- coding:utf-8 -*-

import re
import math
import operator as op

Symbol=str

class Procedure(object):
    def __init__(self,parms,body,env):
        self.parms,self.body,self.env=parms,body,env
    def __call__(self,*args):
        return eval(self.body,Env(self.parms,args,self.env))

class Env(dict):
    def __init__(self,parms=(),args=(),outer=None):
        self.update(zip(parms,args)) #形成{var:value}的约束
        self.outer=outer
    def find(self,var): #继承dict
        return self if (var in self) else self.outer.find(var)

def tokenize(chars): #切分token
    return chars.replace('(',' ( ').replace(')',' ) ').split()

def parse(program): #生成语法树
    return read_from_tokens(tokenize(program))
def read_from_tokens(tokens):
    if len(tokens)==0:
        raise SyntaxError("unexpected EOF")
    token=tokens.pop(0) # (
    if token=='(':
        L=[]
        while tokens[0]!=')':
            L.append(read_from_tokens(tokens))
        tokens.pop(0) # )
        return L
    elif token==')':
        raise SyntaxError('unexpected )')
    else:
        return atom(token)
def atom(token): #将token转化为语法树元素
    try:
        return int(token)
    except ValueError:
        try:
            return float(token)
        except ValueError:
            return Symbol(token)


def standard_env():
    env=Env()
    env.update(vars(math))
    env.update({
        '+': op.add,
        '-':op.sub,
        '*':op.mul,
        '/':op.truediv,
        '>':op.gt,
        '<':op.lt,
        '>=':op.ge,
        '<=':op.le,
        '=':op.eq,
        'abs':abs,
        'append':op.add,
        #'apply':apply,
        'begin': lambda *x:x[-1],
        'car': lambda x:x[0],
        'cdr': lambda x:x[1:],
        'cons': lambda x,y:[x]+y,
        'eq?':op.is_,
        'equal?':op.eq,
        'length':len,
        'list':lambda *x:list(x),
        'list?':lambda x:isinstance(x,list),
        'map':map,
        'max':max,
        'min':min,
        'not':op.not_,
        'null?': lambda x:x==[],
        'number?':lambda x:isinstance(x,Number),
        'procedure?':callable,
        'round':round,
        'symbol?':lambda x:isinstance(x,Symbol),
    })
    return env

global_env = standard_env()


def cond2if(cond_clauses):
    if len(cond_clauses)==0:
        return None
    clause=cond_clauses.pop(0)
    if clause[0]=='else':
        return clause[1]
    else:
        if_exp=['if',clause[0],clause[1],cond2if(cond_clauses)]
        return if_exp
def define2lambda(parms,body):
    return ['lambda',parms,body]

def body2begin(body):
    begin_exp=['begin']
    begin_exp.extend(body)
    return begin_exp

def eval(x, env=global_env):
    if isinstance(x,Symbol): #变量
        return env.find(x)[x]
    elif not isinstance(x,list):
        return x
    elif x[0]=='quote':
        (_,exp)=x
        return exp
    elif x[0]=='if':
        (_,test,conseq,alt)=x 
        exp=(conseq if eval(test,env) else alt) 
        return eval(exp,env)
    elif x[0]=='define':
        if isinstance(x[1],Symbol): #变量定义
            (_,var,exp)=x
            env[var]=eval(exp,env) #拷贝外部环境，并加入约束
        else: #过程定义
            var=x[1][0]
            parms=x[1][1:]
            body=x[2:]
            env[var]= Procedure(parms,body2begin(body),env)
    elif x[0]=='set!':
        (_,var,exp)=x
        env.find(var)[var]=eval(exp,env)
    elif x[0]=='lambda':
        (_,parms,body)=x
        return Procedure(parms,body2begin(body),env) #返回一个过程
    elif x[0]=='cond':
       exp=x[1:]
       return eval(cond2if(exp),env)
    
    else: #procedure call
        proc=eval(x[0],env)
        args=[eval(arg,env) for arg in x[1:]] #非惰性求值
        return proc(*args)

def repl(prompt='scheme>'):  #read-eval-print-loop
    while True:
        val=eval(parse(input(prompt)))
        if val is not None:
            print(schemestr(val))

def schemestr(exp):  #convert python object to scheme-readable string.
    if isinstance(exp,list):
        return '('+''.join(map(schemestr,exp))+')'
    else:
        return str(exp)


program='''
(define (cons x y)
  (define (dispatch m)
    (cond ((= m 0) x)
          ((= m 1) y)))
  dispatch)

(define (car z) (z 0))
(define (cdr z) (z 1))

(cdr (cons 2 15))
'''

program2='''
(define (x y) (begin (define z 10) (* z y)))
(x 50)
'''

tokens=tokenize(program)
while len(tokens):
    L=read_from_tokens(tokens)
    result=eval(L)
    if result:
        print(result)