#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED
#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include<algorithm>
using namespace std;
enum Token {TOKEN_INT,TOKEN_FLOAT,TOKEN_STRING,TOKEN_QUOTE,TOKEN_IDENT,TOKEN_COLON,
            TOKEN_OPEN_BRACKET,TOKEN_CLOSE_BRACKET,TOKEN_COMMA,TOKEN_OPEN_BRACE,
            TOKEN_CLOSE_BRACE,TOKEN_INSTR,TOKEN_SETSTACKSIZE,TOKEN_VAR,TOKEN_FUNC,
            TOKEN_PARAM,TOKEN_REG,TOKEN_INVALID,TOKEN_END};
enum Status {STATUS_STRING,STATUS_INT,STATUS_FLOAT,STATUS_IDENT,STATUS_START,STATUS_DELIM,STATUS_END};

class Scanner
{
public:
    Scanner();
    Scanner(string path);
    bool OpenSourceFile(string path);
    Token GetNextToken();
    void Reset();
    string CurrToken;
    Token ICurrToken;
    void _TestPrint();
private:
    Token _GetNextToken();
    char LookAheadOneChar(); //������һ��char
    char EatChar(); //��ȡһ��char��Index����1��
    void ThrowChar(); //Index ǰ��һ��
    void InitAllVec(); //��ʼ���ؼ��ֱ�
    void MoveNextLine();
    void RemoveNote(string &s); //�Ƴ�ע��
    void RemoveBlank(string &s); //�Ƴ�����ո�
    void ErrorExit(string error);

    static bool IsAlp(char c);
    static bool IsNum(char c);
    static bool IsOp(char c);
    static bool IsLineEmpty(string s);


    bool IsInIndentVec(string indent);
    bool IsInSignVec(char c);
    bool IsInDelimVec(char c);

    vector<string> IndentVec; //�ؼ��ֱ�
    vector<char> SignVec; //���ű�
    vector<char> DelimVec;

    vector<string> Source; //Դ����
    int Index; //�ַ��α�
    int Line; //��ǰ����
} ;



#endif // SCANNER_H_INCLUDED
