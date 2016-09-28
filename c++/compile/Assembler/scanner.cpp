#include "scanner.h"
#include<stdlib.h>
void Scanner::_TestPrint()
{
    for_each(Source.begin(),Source.end(),[](const string&b){cout<<b<<endl;});
}
Scanner::Scanner(string path)
    :Line(0),Index(0)
{
    if (!OpenSourceFile(path))
    {
        cerr<<"Can't Open the file";
        exit(0);
    }
    RemoveBlank();
    RemoveNote();
}
bool Scanner::OpenSourceFile(string path)
{
    ifstream file(path);
    if (!file.is_open())
        return false;
    string tmp;
    while(getline(file,tmp))
    {
        if (tmp=="")
            continue;
        Source.push_back(tmp);
    }
    return true;
}
void Scanner::ErrorExit(string error)
{
    cout<<"Line:"<<Line+1<<' '<<"Index:"<<Index-1<<' '<<"Error:"<<error;
    exit(0);
}
void Scanner::ThrowChar()
{
    if (Index==0)
        return;
    --Index;
}
char Scanner::EatChar()
{
    return Source[Line][Index++];
}
void Scanner::RemoveBlank()
{
    for (auto &code:Source)
    {
        const char *ptr=code.c_str();
        while(*ptr==' '||*ptr=='\t')
            ++ptr;
        code=string(ptr);
    }
}
void Scanner::RemoveNote()
{
    for (auto &code : Source)
    {
        int index=code.find_first_of(';');
        code=code.substr(0,index);
    }
}

void Scanner::MoveNextLine()
{
    if (Line>=Source.size())
        return;
    Line++;
    Index=0;
}
bool Scanner::IsAlp(char c)
{
    return ((c>='a'&&c<='z')||(c>='A'&&c<='z'));
}
bool Scanner::IsNum(char c)
{
    return (c>='0'&&c<='9');
}
Token Scanner::GetNextToken()
{
    if (Index==Source[Line].size())
        MoveNextLine();
    if (Line>=Source.size())
        return TOKEN_END;
    CurrToken="";
    bool is_add=true;
    bool is_end=false;
    Status curr_status=STATUS_START;
    while(1)
    {
        is_add=true;
        if (Index==Source[Line].size())
        {
            break;
        }
        char c=EatChar();
        switch(curr_status)
        {
        case STATUS_START:
            if (IsAlp(c))
            {
                curr_status=STATUS_IDENT;
            }
            else if (IsNum(c))
            {
                curr_status=STATUS_INT;
            }
            else if (c=='"')
            {
                is_add=false;
                curr_status=STATUS_STRING;
            }
            else if (c==' '||c=='\n')
            {
                is_add=false;
                curr_status=STATUS_START;
            }
            else
            {
                is_add=false;
                ErrorExit("error");
            }
            break;
        case STATUS_IDENT:
            if (!IsAlp(c))
            {
                is_end=true;
                is_add=false;
                ThrowChar();
            }
            break;
        case STATUS_INT:
            if (IsNum(c))
            {

            }
            else if (c=='.')
            {
                curr_status=STATUS_FLOAT;
            }
            else
            {
                is_add=false;
                is_end=true;
                ThrowChar();
            }
            break;
        case STATUS_FLOAT:
            if (IsNum(c))
            {

            }
            else
            {
                is_add=false;
                is_end=true;
                ThrowChar();
            }
            break;
        default:
            break;
        }
        if (is_add==true)
        {
            CurrToken+=c;
        }
        if (is_end==true)
        {
            break;
        }
    }
    switch(curr_status)
    {
    case STATUS_IDENT:
        return TOKEN_IDENT;
    case STATUS_INT:
        return TOKEN_INT;
    case STATUS_FLOAT:
        return TOKEN_FLOAT;
    default:
        return TOKEN_END;
    }

}
bool Scanner::IsInIndentVec(string indent)
{
    if (find(IndentVec.begin(),IndentVec.end(),indent)!=IndentVec.end())
        return true;
    return false;
}
void Scanner::InitIndentList()
{
    IndentVec.push_back("mov");
    IndentVec.push_back("add");
    IndentVec.push_back("func");
    IndentVec.push_back("mul");
    IndentVec.push_back("");
}
char Scanner::LookAheadOneChar()
{
    char c=EatChar();
    ThrowChar();
    return c;
}
