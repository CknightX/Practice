#include<iostream>
#include<fstream>
#include "parser.h"
#include<stdio.h>
using namespace std;
int main()
{
    Scanner a("D:\\1.txt");
    Token b;
    while((b=a.GetNextToken())!=TOKEN_END)
    {
        switch(b)
        {
        case TOKEN_IDENT:
            cout<<"ident:"<<a.CurrToken<<endl;
            break;
        case TOKEN_INT:
            cout<<"int:"<<a.CurrToken<<endl;
            break;
        case TOKEN_FLOAT:
            cout<<"float:"<<a.CurrToken<<endl;
        }
    }
}
