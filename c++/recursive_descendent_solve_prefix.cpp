#include<iostream>
#include<memory>
#include<stdlib.h>
using namespace std;
string s="(+ 6 (/ (- (* 2 7) 2) 2))";
int index=0;
//deal char
inline bool isNum(char c){return (c>='0'&&c<='9');}
inline bool isOp(char c){return (c=='+'||c=='-'||c=='*'||c=='/');}


int getNum();
char getOp();
int getExpression();
int main()
{
    cout<<getExpression();
    return 0;
}
int getExpression()
{
    while(s[index]==' ')
        ++index;
    cout<<s[index]<<endl;
    if (isNum(s[index]))
    {
        return getNum();
    }
    else if (s[index++]=='(')
    {
        if (isOp(s[index]))
        {
            char op=s[index++];
            int left=getExpression();
            int right=getExpression();
            if (s[index]!=')')
            {
                cout<<"1lost )";
                exit(0);
            }
            ++index;
            int result;
            switch(op)
            {
            case '+':
                result=left+right;
                break;
            case '-':
                result=left-right;
                break;
            case '*':
                result=left*right;
                break;
            case '/':
                result=left/right;
                break;
            }

            return result;
        }
        else
        {
            int result= getExpression();
            if (s[index]!=')')
            {
                cout<<"2lost )";
                exit(0);
            }
            ++index;
            return result;
        }
    }
}
int getNum()
{

    int sum=0;
    while(isNum(s[index]))
    {
        sum=10*sum+(s[index]-'0');
        ++index;
    }
    return sum;
}
char getOp()
{

    return s[index++];
}