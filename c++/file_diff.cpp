/*本程序用来检测某目录下，若干文件操作后文件的差异*/
#include<iostream>
#include<windows.h>
#include<string>
#include<vector>
#include<stdlib.h>
using namespace std;
struct FileData_
{
	DWORD nFileSizeHigh, nFileSizeLow, dwFileAttributes;
	string cFileName;
	void operator=(const WIN32_FIND_DATA& tmp)
	{
		cFileName = string(tmp.cFileName);
		nFileSizeHigh = tmp.nFileSizeHigh;
		nFileSizeLow = tmp.nFileSizeLow;
		dwFileAttributes = tmp.dwFileAttributes;
	}
	FileData_(const WIN32_FIND_DATA& tmp)
	{
		cFileName = string(tmp.cFileName);
		nFileSizeHigh = tmp.nFileSizeHigh;
		nFileSizeLow = tmp.nFileSizeLow;
		dwFileAttributes = tmp.dwFileAttributes;
	}
	FileData_(){}
};
struct Node
{
	vector<Node*> child;
	FileData_ FileData;
};
class FileTree 
{
public:
	void Build(const string& path)
	{
		RootPath = path;
		_Build(path,Root);
	}
	void Check()
	{
		_Build(RootPath, CheckRoot);
		_Check(Root,CheckRoot,""); //比较两棵树
	}
	~FileTree()
	{
		destruction(Root);
		destruction(CheckRoot);
	}
private:
	Node *Root;
	Node *CheckRoot;
	string RootPath;
	void destruction(Node *root)
	{
		for (auto i : root->child)
		{
			if (i->FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				destruction(i);
			else
				delete i;
		}
	}
	void _Check(Node *root,Node *check_root,string curr_path) //同文件夹比较
	{
		curr_path += root->FileData.cFileName;
		curr_path += "/";
		for (auto i : check_root->child)  //新增文件
		{
			int flag = 0;
			Node *p = nullptr;
			for (auto j : root->child)
			{
				p = j;
				if (j->FileData.cFileName==i->FileData.cFileName)
				{
					if (j->FileData.nFileSizeHigh == i->FileData.nFileSizeHigh&&j->FileData.nFileSizeLow == i->FileData.nFileSizeLow)
						flag = 1;
					else
						flag = 2;
					break;
				}
			}
			if (flag == 0)
				cout << "new file:\t./" << curr_path << i->FileData.cFileName << endl;
			else if (flag == 2)
			{
				cout << "file change:\t./" << curr_path << i->FileData.cFileName << endl;
			}
			else if (flag == 1)
			{
				if (i->FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					_Check(p,i,curr_path);
			}

		}
		for (auto i : root->child)
		{
			bool flag = false;
			for (auto j : check_root->child)
			{
				if (j->FileData.cFileName==i->FileData.cFileName)
				{
						flag = true;
						break;
				}
			}
				if (!flag)
					cout << "lose file:\t./" << curr_path << i->FileData.cFileName << endl;
		}

	}
	void _Build(string path,Node *&root)
	{
		root = new Node;
		WIN32_FIND_DATA data;
		HANDLE hFind;
		hFind = FindFirstFile((path).c_str(), &data);
		root->FileData = data;
		hFind = FindFirstFile((path + "\\*.*").c_str(), &data);
		do
		{
			if (string(data.cFileName) == "." || string(data.cFileName) == "..")
			{
				continue;
			}
			Node *tmp;
			if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) //文件夹
			{
				_Build(path + "\\" + data.cFileName, tmp);
			}
			else
				tmp = new Node;
			tmp->FileData = data;
			root->child.push_back(tmp);
		} while (FindNextFile(hFind, &data));
	}
};
int main()
{
	FileTree a;
	a.Build("D:\\Temp");
	cin.get();
	a.Check();
	cin.get();
}