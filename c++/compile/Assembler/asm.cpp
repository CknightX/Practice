#include<iostream>
#include<fstream>
#include "parser.h"
#include<stdio.h>
using namespace std;
Scanner a("D:\\1.txt");
int Token;
void GetFunc();
bool GetIns();
int main()
{
   a._TestPrint();
    while((a.GetNextToken())!=TOKEN_END)
    {
        GetFunc();
    }
}
void GetFunc()
{
    if (a.ICurrToken==TOKEN_IDENT&&a.CurrToken=="Func")
    {
        cout<<"Func  ";
        a.GetNextToken(); //获取函数名
        cout<<a.CurrToken<<endl;
        if ((a.GetNextToken())!=TOKEN_OPEN_BRACE)
        {
            cout<<"error{";
            exit(0);
        }
        cout<<a.CurrToken<<endl;
        while(1)
        {
            if(!GetIns())
            break;

        }
        if (a.ICurrToken!=TOKEN_CLOSE_BRACE)
        {
            cout<<"error}";
            cout<<a.CurrToken;
            exit(0);
        }
        cout<<a.CurrToken<<endl;
    }

}
bool GetIns()
{
    if ((a.GetNextToken())!=TOKEN_IDENT)
    {
        return false;
    }
    cout<<a.CurrToken<<'\t';
    a.GetNextToken();
    cout<<a.CurrToken;
     if ((a.GetNextToken())!=TOKEN_COMMA)
    {
        cout<<"error,";
            exit(0);
    }
    cout<<a.CurrToken;
    a.GetNextToken();
    cout<<a.CurrToken;
    cout<<endl;
    return true;
}
