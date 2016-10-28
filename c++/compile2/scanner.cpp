#include "scanner.h"
void Scanner::LoadString(string str)
{
	RemoveNote(str);
	RemoveBlank(str);
	Source.push_back(str);
	InitAllVec();
}
void Scanner::_TestPrint()
{
    for_each(Source.begin(),Source.end(),[](const string&b)
    {
        cout<<b<<"\tsize:"<<b.size()<<endl;
    });
}
Scanner::Scanner()
    :Line(0),Index(0)
{

}
void Scanner::PutToken()
{
	_IsBack = true;
}
Scanner::Scanner(string path)
    :Line(0),Index(0)
{
    InitAllVec();
    if (!OpenSourceFile(path))
    {
        cerr<<"Can't Open the file";
        exit(0);
    }

}
bool Scanner::OpenSourceFile(string path)
{
    InitAllVec();
    ifstream file(path);
    if (!file.is_open())
        return false;
    string tmp;
    while(getline(file,tmp))
    {
        RemoveNote(tmp);
        RemoveBlank(tmp);

        if (IsLineEmpty(tmp))
            continue;
        Source.push_back(tmp);
    }
    return true;
}
bool Scanner::IsLineEmpty(string s)
{
    for (auto c:s)
    {
        if (c!='\n'&&c!=' '&&c!='\t')
            return false;
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
void Scanner::RemoveBlank(string &s)
{
    const char *ptr=s.c_str();
    while(*ptr==' '||*ptr=='\t')
        ++ptr;
    s=string(ptr); //开头空格移除
    ptr=s.c_str()+s.size()-1;

    while(*ptr==' '||*ptr=='\t')
        --ptr;
    s=s.substr(0,ptr-s.c_str()+1);
}
void Scanner::RemoveNote(string &s)
{
	int index=s.find_first_of('#');
    s=s.substr(0,index);
}

void Scanner::MoveNextLine()
{
    Line++;
    Index=0;
}
bool Scanner::IsAlp(char c)
{
    return ((c>='a'&&c<='z')||(c>='A'&&c<='Z'));
}
bool Scanner::IsNum(char c)
{
    return (c>='0'&&c<='9');
}
Token Scanner::GetNextToken()
{
	if (_IsBack)
	{
		_IsBack = false;
	}
	else
	{
		ICurrToken = _GetNextToken();
	}
    return ICurrToken;
}
Token Scanner::_GetNextToken()
{
    if (Line>=Source.size())
            return TOKEN_END;
    CurrToken="";
    bool is_add=true;
    bool is_end=false;
    Status curr_status=STATUS_START;
    while(1)
    {
        is_add=true;

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
            else if (c==' '||c=='\t')
            {
                is_add=false;
                curr_status=STATUS_START;
                //if (Index==Source[Line].size())
                  //  MoveNextLine();
            }
            else if (IsInDelimVec(c))
            {
                curr_status=STATUS_DELIM;
            }
			else if (IsInOpVec(c))
			{
				if (c == '-')
				{
					if (!IsInOpVec(Source[Line][Index - 2])&&!IsInDelimVec(Source[Line][Index - 2]))
					{
						curr_status = STATUS_OP;
						is_end = true;
					}
					else
					{
						curr_status = STATUS_IS_OPPO_NUM;
					}
				}
				else
				curr_status = STATUS_OP;
			}
            else
            {
                is_add=false;
                ErrorExit("error");
            }
            break;
        case STATUS_IDENT:
            if (!IsAlp(c)&&!(IsNum(c)))
            {
                is_end=true;
                is_add=false;
                ThrowChar();
            }
            break;
		case STATUS_IS_OPPO_NUM:
			if (IsNum(c))
			{
				curr_status = STATUS_INT;
			}
			else if (c == '.')
			{
				curr_status = STATUS_FLOAT;
			}
			else
			{
				throw invalid_argument("error");
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
		case STATUS_DELIM:
			is_add = false;
				is_end = true;
				ThrowChar();
			break;

		case STATUS_OP:
			if (IsInOpVec(c))
			{
				if (c == '-')
				{
					is_add = false;
					is_end = true;
					ThrowChar();
				}
			}
			else
			{
				is_add = false;
				is_end = true;
				ThrowChar();
			}
        }
        if (is_add==true)
        {
            CurrToken+=c;
        }
        if (Index==Source[Line].size())
        {
            MoveNextLine();
            break;
        }
        if (is_end==true)
        {
            break;
        }

    }
    switch(curr_status)
    {
    case STATUS_IDENT:
        if (CurrToken=="var")
        {
            return TOKEN_VAR;
        }
        else if (CurrToken=="func")
        {
            return TOKEN_FUNC;
        }
		else if (CurrToken == "if")
		{
			return TOKEN_IF;
		}
		else if (CurrToken == "for")
		{
			return TOKEN_FOR;
		}
		else if (CurrToken == "while")
		{
			return TOKEN_WHILE;
		}
		else if (CurrToken == "else")
		{
			return TOKEN_ELSE;
		}
        else
        {
            return TOKEN_IDENT;
        }
        break;
    case STATUS_INT:
        return TOKEN_INT;
        break;
    case STATUS_FLOAT:
        return TOKEN_FLOAT;
        break;
    case STATUS_DELIM:
		if (CurrToken == ",")
			return TOKEN_COMMA;
		else if (CurrToken == "(")
            return TOKEN_OPEN_BRACKET;
		else if (CurrToken == ")")
            return TOKEN_CLOSE_BRACKET;
		else if (CurrToken == "{")
            return TOKEN_OPEN_BRACE;
		else if (CurrToken == "}")
            return TOKEN_CLOSE_BRACE;
		else if (CurrToken == ":")
            return TOKEN_COLON;
		else if (CurrToken == ";")
			return TOKEN_SEMI;
		break;
	case STATUS_OP:
		 if (CurrToken == "=")
			return TOKEN_EQUAL;
		
		else if (CurrToken == "+")
			return TOKEN_ADD;
		else if (CurrToken == "-")
			return TOKEN_SUB;
		else if (CurrToken == "*")
			return TOKEN_MUL;
		else if (CurrToken == "/")
			return TOKEN_DIV;
		else if (CurrToken == "==")
			return TOKEN_TWO_EQUAL;
		else if (CurrToken == ">")
			return TOKEN_BIG;
		else if (CurrToken == "<")
			return TOKEN_SMALL;
		else if (CurrToken == ">=")
			return TOKEN_BIG_EQUAL;
		else if (CurrToken == "<=")
			return TOKEN_SMALL_EQUAL;
		else if (CurrToken == "!=")
			return TOKEN_NOT_EQUAL;
		else if (CurrToken == "&&")
			return TOKEN_AND;
		else if (CurrToken == "||")
			return TOKEN_OR;
		else if (CurrToken == "%")
			return TOKEN_MOD;
		break;
    }

}
bool Scanner::IsInIndentVec(string indent)
{
    if (find(IndentVec.begin(),IndentVec.end(),indent)!=IndentVec.end())
        return true;
    return false;
}
void Scanner::InitAllVec()
{
    //Delim
    DelimVec.push_back(',');
    DelimVec.push_back(':');
    DelimVec.push_back(';');
    DelimVec.push_back('(');
    DelimVec.push_back(')');
    DelimVec.push_back('[');
    DelimVec.push_back(']');
    DelimVec.push_back('{');
    DelimVec.push_back('}');

    OpVec.push_back('=');
    OpVec.push_back('+');
    OpVec.push_back('-');
    OpVec.push_back('*');
    OpVec.push_back('/');
    OpVec.push_back('>');
    OpVec.push_back('<');
    OpVec.push_back('%');
	OpVec.push_back('&');
	OpVec.push_back('|');
	OpVec.push_back('!');



    //Indent
}
char Scanner::LookAheadOneChar() //不支持自动换行
{
    char c=EatChar();
    ThrowChar();
    return c;
}
bool Scanner::IsInDelimVec(char c)
{
    if (find(DelimVec.begin(),DelimVec.end(),c)!=DelimVec.end())
        return true;
    return false;
}
bool Scanner::IsInOpVec(char c)
{
	if (find(OpVec.begin(), OpVec.end(), c) != OpVec.end())
		return true;
	return false;
}
