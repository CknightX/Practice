#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
long sum=0;
int Map[9][9];
bool use_num[10];
vector<vector<int>> usable_num;
void setNum();
int countNum(int x,int y);
void printMap();
void emptyUse();
void solve();
bool isSolved();
void solveGus(vector<vector<int>>::iterator a);
bool islegal(int x,int y);
//void solveGus(int x,int y);
void usable();
int main()
{
    setNum();
    solve();
    return 0;
}
bool islegal(int x,int y)
{
int numof[10]={0};
                // vertical
                for (int i=0; i<9; ++i)
                    numof[Map[i][y]]++;
                // horizon
                for (int i=0; i<9; ++i)
                    numof[Map[x][i]]++;
                int _x,_y;   //9 cube
                _x=(x/3)*3;
                _y=(y/3)*3;
                for (int i=_x; i<_x+3; ++i)
                    for (int j=_y; j<_y+3; ++j)
                        numof[Map[i][j]]++;
           if (numof[Map[x][y]]==3)
                return true;
           return false;
}
void solveGus(vector<vector<int>>::iterator a)
{
        if (a==usable_num.end())
        {
                //printMap();
                ++sum;
                if (sum%10000==0)
                        cout<<sum<<endl;
                        return;
        }
        for (auto b=(*a).begin()+1;b!=(*a).end();++b)
        {
                int c=*((*a).begin())/10,d=*((*a).begin())%10;
                Map[c][d]=*b;
                if (islegal(c,d))
                {
                        solveGus(a+1);
                }
                Map[c][d]=0;
        }
}
void setNum()
{
    int tmp[9][9]=
    {
        0,0,0,0,6,0,0,0,0,
        0,2,0,0,0,0,0,0,0,
        0,9,0,0,0,0,0,0,0,
        0,1,0,0,0,6,0,2,5,
        4,0,0,0,0,0,9,1,0,
        0,0,0,0,0,0,0,0,4,
        0,0,0,6,4,0,0,0,0,
        0,0,0,0,0,0,8,0,0,
        5,0,1,0,0,0,0,0,2
    };
    for (int i=0; i<9; ++i)
        for (int j=0; j<9; ++j)
            Map[i][j]=tmp[i][j];
}
int countNum(int x,int y)
{
    emptyUse();
    // vertical
    for (int i=0; i<9; ++i)
        use_num[Map[i][y]]=true;
    // horizon
    for (int i=0; i<9; ++i)
        use_num[Map[x][i]]=true;
    int _x,_y;   //9 cube
    _x=(x/3)*3;
    _y=(y/3)*3;
    for (int i=_x; i<_x+3; ++i)
        for (int j=_y; j<_y+3; ++j)
            use_num[Map[i][j]]=true;
    int numof=0,unuse_num=-1;
    for (int i=1; i<10; ++i)
    {
        numof+=static_cast<int>(use_num[i]);
        if (use_num[i]==false)
            unuse_num=i;
    }
    if (numof==8)
        return unuse_num;
    return 0;
}
void emptyUse()
{
    for (int i=0; i<10; ++i)
        use_num[i]=false;
}
void solve()
{

        for (int j=0; j<9; ++j)
            for (int k=0; k<9; ++k)
                if (Map[j][k]==0)
                {
                    int tmp=countNum(j,k);
                    Map[j][k]=tmp;
                }
    if (!isSolved())
    {
        solveGus(usable_num.begin());
    }
    else
    {
            printMap();
    }


}
void printMap()
{
    for (int i=0; i<9; ++i)
    {
        for (int j=0; j<9; ++j)
            if (Map[i][j]==0)
                cout<<'*'<<' ';
            else
                cout<<Map[i][j]<<' ';
        cout<<endl;
    }
    cout<<endl;
}
bool isSolved()
{
    for (int x=0; x<9; ++x)
        for (int y=0; y<9; ++y)
            if (Map[x][y]==0)
            {
                emptyUse();
                // vertical
                for (int i=0; i<9; ++i)
                    use_num[Map[i][y]]=true;
                // horizon
                for (int i=0; i<9; ++i)
                    use_num[Map[x][i]]=true;
                int _x,_y;   //9 cube
                _x=(x/3)*3;
                _y=(y/3)*3;
                for (int i=_x; i<_x+3; ++i)
                    for (int j=_y; j<_y+3; ++j)
                        use_num[Map[i][j]]=true;
                vector<int> a;
                a.push_back(10*x+y);
                for (int i=1; i<10; ++i)
                {
                    if (use_num[i]==false)
                        a.push_back(i);
                }
                usable_num.push_back(a);
            }
    if (usable_num.empty())
        return true;
    return false;
}

