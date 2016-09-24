#include <iostream>
#include <stdlib.h>
#include<string.h>
#include<stdio.h>
using namespace std;

string Exp="(1+2)*3";
int Index=0;

struct Expression
{
    bool IsNum;
    int Num;
    char Op;
    Expression *Left;
    Expression *Right;
    Expression(int num)
    {
        IsNum=true;
        Num=num;
        Op=0;
        Left=0;
        Right=0;
    }
    Expression(char op,Expression *left,Expression *right)
    {
        IsNum=false;
        Num=0;
        Op=op;
        Left=left;
        Right=right;
    }
    ~Expression()
    {
        if (Left)delete Left;
        if (Right) delete Right;
    }
    void Print()
    {
        if (IsNum)
            cout<<Num;
        else
        {
            cout<<"("<<Op<<" ";
            Left->Print();
            cout<<" ";
            Right->Print();
            cout<<")";
        }
    }
};
void ExitError(string s)
{
    cerr<<s;
    exit(0);
}
bool IsNum(char c)
{
    return (c>='0'&&c<='9');
}
bool IsOp(char c)
{
    return (c=='+'||c=='-'||c=='*'||c=='/');
}
int GetNum();
char GetOp();
Expression* GetTerm();   //Term=<数字> | “(“ Exp “)”
Expression* GetFactor();     //Factor = Term ( ( “*” | “/” ) Term) *
Expression* GetExp();    //Exp = Factor ( ( “+” | “-“ ) Factor) *
int main()
{
    Expression *p=GetExp();
    p->Print();
    return 0;
}
int GetNum()
{
    int result=0;
    while(IsNum(Exp[Index]))
    {
        result=10*result+(Exp[Index]-'0');
        Index++;
    }
    return result;
}
char GetOp()
{
    return Exp[Index++];
}
Expression* GetTerm()
{
    Expression *result;
    while(Exp[Index]==' ') //去空格
        ++Index;
    if (IsNum(Exp[Index]))
    {
        result= new Expression(GetNum());
    }
    else if (Exp[Index]=='(')
    {
        ++Index;
        result=GetExp();
        if (Exp[Index]!=')')
        {
            return NULL;
            ExitError("Error:)");
        }
        ++Index;
    }
    else
    {
        result=NULL;
    }
    return result;
}
Expression* GetFactor()
{
     while(Exp[Index]==' ') //去空格
        ++Index;
    Expression *result=GetTerm();
    while(1)
    {
        char op=Exp[Index];
        if (op=='*'||op=='/')
        {
            char op=GetOp();
            result=new Expression(op,result,GetTerm());
        }
        else
            break;
    }
    return result;
}
Expression* GetExp()//Exp = Factor ( ( “+” | “-“ ) Factor) *
{
     while(Exp[Index]==' ') //去空格
        ++Index;
   Expression *result=GetFactor();
    while(1)
    {
        char op=Exp[Index];
        if (op=='+'||op=='-')
        {
            char op=GetOp();
            result=new Expression(op,result,GetFactor());
        }
        else
            break;
    }
    return result;
}
