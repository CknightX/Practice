# -*- coding:utf-8 -*-

import re
import math
import operator as op
from functools import reduce

Symbol=str
Number=int

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

def parse(program): #生成语法
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

def and_exp(*items):
    for x in items:
        if not x:
            return False
    return True
def or_exp(*items):
    for x in items:
        if x:
            return True
    return False
def list2cons(items):
    if len(items)==0:
        return []
    else:
        return [items[0],list2cons(items[1:])]

def append(list1,list2):
    if list1==[]:
        return list2
    else:
        return [list1[0],append(list1[1],list2)]


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
        'append':append,
        #'apply':apply,
        'and':and_exp,#lambda *x:reduce (lambda a,b:a and b,x) ,
        'or':or_exp,#lambda *x:reduce (lambda a,b:a or b,x) ,
        'begin': lambda *x:x[-1],
        'car': lambda x:x[0],
        'cdr': lambda x:x[1],
        'cons': lambda x,y:[x,y],
        'equal?':op.eq,
        'length':len,
        'list':lambda *x:list2cons(x),
        'eq?':lambda x,y:x==y,
        'list?':lambda x:isinstance(x,list),
        'null?': lambda x:x==[],
        'number?':lambda x:isinstance(x,Number),
        'procedure?':callable,
        'pair?':lambda x:isinstance(x,list),
        'map':map,
        'max':max,
        'min':min,
        'not':op.not_,
        'round':round,
        'symbol?':lambda x:isinstance(x,Symbol),
        'display':print,
        'null':[]
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
    if len(body)==1:
        return body[0]
    elif len(body)>1:
        begin_exp=['begin']
        begin_exp.extend(body)
        return begin_exp
    else:
        raise SyntaxError(' ')
def cadr2nest(i,x):   # (cadr x) -> (car (cdr x))
    c=x[0][i]
    if c=='a':
        return ['car',cadr2nest(i+1,x)]
    elif c=='d':
        return ['cdr',cadr2nest(i+1,x)]
    elif c=='r':
        return x[1]
            
            
def eval(x, env=global_env):
    if isinstance(x,Symbol): #变量
        if x[0]=="'":  #引用表达式
            return eval(['quote',x[1:]])
        else:
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
        return Procedure(parms,body,env) #返回一个过程
    elif x[0]=='cond':
       exp=x[1:]
       return eval(cond2if(exp),env)

    elif x[0][0]=='c' and x[0][-1]=='r' and len(x[0])>3:  #caddr cadr
        return eval(cadr2nest(1,x))

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
        if exp==[]:
            return ')'
        else:
            return '('+''.join(map(schemestr,exp))
    else:
        return str(exp)


def convertstr(exp):
    if exp==[]:
        return ') '
    elif isinstance(exp[0],list):
        return '( '+convertstr(exp[0])+convertstr(exp[1])
    else:
        return str(exp[0])+' '+convertstr(exp[1])

            

program=open('D:\\1.txt','r').read()


a=[1,2,3]

tokens=tokenize(program)

while len(tokens):
    L=read_from_tokens(tokens)
    result=eval(L)
    if result!=None:
        print('( '+convertstr(result))
