#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED
#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include<algorithm>
#include<stdlib.h>
using namespace std;
enum Token {
	TOKEN_INT, TOKEN_FLOAT, TOKEN_STRING, TOKEN_QUOTE, TOKEN_IDENT, TOKEN_COLON, TOKEN_SEMI,
            TOKEN_OPEN_BRACKET,TOKEN_CLOSE_BRACKET,TOKEN_COMMA,TOKEN_OPEN_BRACE,TOKEN_SUB,TOKEN_MUL,TOKEN_DIV,TOKEN_BIG,TOKEN_SMALL,TOKEN_EQUAL,
            TOKEN_CLOSE_BRACE,TOKEN_INSTR,TOKEN_SETSTACKSIZE,TOKEN_VAR,TOKEN_FUNC,TOKEN_ELSE,TOKEN_BIG_EQUAL,TOKEN_SMALL_EQUAL,TOKEN_OR,TOKEN_AND,TOKEN_TWO_EQUAL,TOKEN_NOT_EQUAL,
            TOKEN_PARAM,TOKEN_REG,TOKEN_INVALID,TOKEN_END,TOKEN_ADD,TOKEN_MOV,TOKEN_IF,TOKEN_WHILE,TOKEN_FOR,TOKEN_MOD};
enum Status {STATUS_IS_OPPO_NUM,STATUS_STRING,STATUS_INT,STATUS_FLOAT,STATUS_IDENT,STATUS_SIGN,STATUS_START,STATUS_DELIM,STATUS_OP,STATUS_END,STATUS_OPPO_NUM};

class Scanner
{
public:
	void AssmblSourceFile();
    Scanner();
    Scanner(string path);
    bool OpenSourceFile(string path);
    Token GetNextToken();
	void PutToken();
    void Reset();
	void LoadString(string str);
    string CurrToken;
    Token ICurrToken;
    void _TestPrint();
private:
	bool _IsBack = false;
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
	bool IsInOpVec(char c);
    //Ԥ���----------------
    vector<string> IndentVec; //�ؼ��ֱ�
    vector<char> SignVec; //���ű�


    vector<char> DelimVec; //�ָ�����
	vector<char> OpVec; //�������

    vector<string> Source; //Դ����
    int Index; //�ַ��α�
    int Line; //��ǰ����
} ;



#endif // SCANNER_H_INCLUDED
