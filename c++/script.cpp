/*
 * C++ 命令驱动式脚本解释器
 * 语法规范:
 *
 * 变量定义:DefConst XXX (n)
 *
 */
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<string>
#include<vector>
#include<algorithm>
#include<sstream>
#include<utility>
#include<iostream>
using namespace std;
const int MAX_LINE_SIZE=512;
const string PATH = "D:\\3.txt";
class script
{
	public:
		script()
		{
			scrLine=0;
			currentLine=0;
			n_kh=0;
		}
		void loadScript(string strFileName);
		void test()
		{
			//printf("%d",scrLine);
			for_each(scrCode.begin(),scrCode.end(),[](const string str)
					{
					printf("%s",str.c_str());
					});
		}
		void runScript();
	private:
		typedef vector<string>::iterator iter;
		void countLine();
		void loadToMemory();
		void runOneLineScript(vector<string>::iterator &i);
		void error(string err,int line);
		inline bool isAlp(char c);
		string getCommand(iter);  // 获取命令
		string getSymbol(iter);  //获取标识符
		string getStr(iter); //获取字符串
		int getNum(iter); //获取数值
		int scrLine;
		int currentLine;
		int n_kh; // 大括号嵌套计数
		FILE *scrFile;
		vector<string> scrCode;
		vector<pair<string,const int>> constVb; //常量表
		vector<pair<string,int>> vb; //变量表
};
int main(int argc,char **argv)
{
	script scr;
	scr.loadScript(PATH);
	scr.runScript();
	return 0;
}
void script::loadScript(string strFileName)
{
	if (!(scrFile=fopen(strFileName.c_str(),"rb")))
	{
		printf("FILE I/0 error.\n");
		exit(0);
	}
	//countLine();
	loadToMemory();
	fclose(scrFile);
}
void script::countLine()
{
	while(!feof(scrFile))
		if (fgetc(scrFile)=='\n')
			++scrLine;
	++scrLine; // the first line
}
void script::loadToMemory()
{
	char s[MAX_LINE_SIZE];
	while(fgets(s,MAX_LINE_SIZE,scrFile))
	{
		scrCode.push_back(string(s));
	}
}
void script::runScript()
{
	printf("Script is running!\n");
	printf("------------------\n");
	for (auto i=scrCode.begin(); i!=scrCode.end(); ++i)
	{
		++currentLine;
		runOneLineScript(i);
		if (i==scrCode.end())
                        break;
	}
}
string script::getCommand(iter i)
{
	string code=*i;
	string result="";
	for (auto c=code.begin(); c!=code.end(); ++c)
	{
		if (isAlp(*c))
		{
			result+=*c;
		}
		else
		{
			return result;
		}
	}
	return result;
}
string script::getStr(iter i)
{
	string code=*i;
	int b=code.find_first_of('"')+1;
	int e=code.find_last_of('"');
	if (b==string::npos)
		return string("");
	return code.substr(b,e-b);
}
int script::getNum(iter i)
{
	stringstream tmp;
	string code=*i;
	int b=code.find_first_of('(');
	int e=code.find_first_of(')');
	//cout<<b;
	if (b==string::npos)
		return -1;
	string num= code.substr(b+1,e-b-1);
	if (num.size()!=0&&isAlp(num[0])) //开头为字母，即变量
	{
		for (auto it=constVb.begin(); it!=constVb.end(); ++it)
		{
			if ((*it).first==num)
				return (*it).second;
		}
		for (auto it=vb.begin(); it!=vb.end(); ++it)
		{
			if ((*it).first==num)
				return (*it).second;
		}
	}
	int _num;
	tmp<<num;
	tmp>>_num;
	return _num;
}
inline bool script::isAlp(char c)
{
	return ((c>='A'&&c<='Z')||(c>='a'&&c<='z'));
}
string script::getSymbol(iter it)
{
	string code=*it;
	int b=code.find_first_of(' ')+1;  //跳过命令
	int e=code.find_last_of(' ');
	if (b==string::npos)
		return string("");
	return code.substr(b,e-b);
}
void script::runOneLineScript(vector<string>::iterator &i)
{
	string command = getCommand(i);
	if (i==scrCode.end())
                exit(0);

	else if (command=="Print")
		cout<<getStr(i);
	else if  (command=="PrintLoop")
	{
		for (int j=0; j<getNum(i); ++j)
			cout<<getStr(i);
	}
	else if (command== "NewLine")
		cout<<endl;
	else if (command=="DefConst")
	{
		constVb.push_back(make_pair<string,const int>(getSymbol(i),getNum(i)));
	}
	else if (command=="DefVb")
	{
		vb.push_back(make_pair<string,int>(getSymbol(i),getNum(i)));
	}
	else if (command=="If")
	{
		int Bool=getNum(i);
		++n_kh;
		i+=2;
			int j=0;
			while(n_kh!=0) //搜索右大括号,取之间代码块
			{
			        if ((*(i+j)).substr(0,1)=="{")
                                        n_kh++;
                                if ((*(i+j)).substr(0,1)=="}")
                                        n_kh--;
				++j;
				if ((i+j)==scrCode.end())
				{
					break;
				}
			}
			--j;
			if (j==0) //代码块为空，不执行任何命令
				return;
			else
			{
				if (Bool!=0)  //表达式为True
					{
					        auto _i=i;
					        for (;i!=(_i+j);++i)
                                                {
                                                        runOneLineScript(i);  //递归执行块内代码
                                                }
                                                i+=1;
                                                if (i==scrCode.end())
                                                        return;
                                                if (getCommand(i)=="Else")
                                                {
                                                        while((*i).substr(0,1)!="}")
                                                                ++i;
                                                }
                                                else
                                                {
                                                        i-=1;
                                                        return;
                                                }
					}
				else
				{
					i+=(j+1); //跳至右大括号处
				}
			}
		}
	else if (command=="Else")
	{
		i+=2;
		++n_kh;
			int j=0;
			while(n_kh!=0) //搜索右大括号,取之间代码块
			{
			        if ((*(i+j)).substr(0,1)=="{")
                                        n_kh++;
                                if ((*(i+j)).substr(0,1)=="}")
                                        n_kh--;
				++j;
				if ((i+j)==scrCode.end())
				{
					break;
				}
			}
			--j;
			if (j==0) //代码块为空，不执行任何命令
				return;
			else
			{
			        auto _i=i;
				for (;i!=(_i+j);++i)
				{
					runOneLineScript(i);  //递归执行块内代码
				}
			}
	}
	else if (command=="")
	{
	}
	else
	{
		error("Unknown command!",currentLine);
	}
}
void script::error(string err,int line)
{
	cout<<"Line:"<<line<<"->"<<err;
	getchar();
	exit(0);
}
