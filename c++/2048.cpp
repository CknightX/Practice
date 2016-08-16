#include<iostream>
#include<windows.h>
#include<iomanip>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<conio.h>
using std::cout;
using std::endl;
using std::cin;
using std::setw;
using std::vector;
class game
{
public:
    game();
    void gameStart();
    void drawMap();
    void randBlock();
    void randTwoBlock();
    static int randNum(int a);
    bool isEnd();
private:
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
    bool removeLeft();
    bool removeRight();
    bool removeUp();
    bool removeDown();
    void returnForward();
    void copyMap();
    int score;
    int map[4][4];
    int forwardmap[4][4];
    int forwardscore;
};
int main()
{
    game a;
    a.gameStart();
    return 0;
}
bool game::isEnd()
{
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            if (map[i][j]==0)
                return false;
    for (int i=0; i<3; ++i)
    {
        for (int j=0; j<=3; ++j)
            if (map[j][i]==map[j][i+1]||map[i][j]==map[i+1][j])
                return false;
    }
    return true;
}
int game::randNum(int a)
{
    return (rand()%a);
}
void game::randTwoBlock()
{
    randBlock();
    randBlock();
}
void game::randBlock()
{
    vector<int> unused;
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            if (map[i][j]==0)
                unused.push_back(10*i+j);
    int k,l,r;
    if (unused.size()==0)
        return;
    r=randNum(unused.size());
    k=unused[r]/10;
    l=unused[r]%10;
    map[k][l]=2;
}
game::game():score(0)
{
    srand(time(0));
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            map[i][j]=0;
    randTwoBlock();
    copyMap();

}
void game::drawMap()
{
    system("cls");
    cout<<endl<<endl;
    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
            if (map[i][j]==0)
                cout<<setw(16)<<'*';
            else
                cout<<setw(16)<<map[i][j];
        cout<<endl<<endl<<endl<<endl<<endl;
    }
    cout<<endl<<"Score:"<<score<<endl<<"Please use direction key:";
}
bool game::moveLeft()
{
    copyMap();
    bool flag=false;
    if(removeLeft())
        flag=true;
    for (int i=0; i<4; ++i)
        for (int j=0; j<3; ++j)
            if (map[i][j]==map[i][j+1]&&map[i][j]!=0)
            {
                flag=true;
                map[i][j]*=2;
                map[i][j+1]=0;
                score+=map[i][j];
                removeLeft();
            }
    return flag;
}
bool game::moveRight()
{
    copyMap();
    bool flag=false;
    if(removeRight())
        flag=true;
    for (int i=0; i<4; ++i)
        for (int j=3; j>0; --j)
            if (map[i][j]==map[i][j-1]&&map[i][j]!=0)
            {
                flag=true;
                map[i][j]*=2;
                map[i][j-1]=0;
                score+=map[i][j];
                removeRight();
            }
    return flag;
}
bool game::moveUp()
{
    copyMap();
    bool flag=false;
    if(removeUp())
        flag=true;
    for (int j=0; j<4; ++j)
        for (int i=0; i<3; ++i)
            if (map[i][j]==map[i+1][j]&&map[i][j]!=0)
            {
                flag=true;
                map[i][j]*=2;
                map[i+1][j]=0;
                score+=map[i][j];
                removeUp();
            }
    return flag;
}
bool game::moveDown()
{
    copyMap();
    bool flag=false;
    if(removeDown())
        flag=true;
    for (int j=0; j<4; ++j)
        for (int i=3; i>0; --i)
            if (map[i][j]==map[i-1][j]&&map[i][j]!=0)
            {
                flag=true;
                map[i][j]*=2;
                map[i-1][j]=0;
                score+=map[i][j];
                removeDown();
            }
    return flag;
}
bool game::removeLeft()
{
    bool flag=false;
    for (int i=0; i<4; ++i)
        for (int j=0; j<3; ++j)
            if (map[i][j]==0)
            {
                for (int k=j; k<=3; ++k)
                    if (map[i][k]!=0)
                    {
                        flag=true;
                        map[i][j]=map[i][k];
                        map[i][k]=0;
                        break;
                    }
            }
    return flag;
}
bool game::removeRight()
{
    bool flag=false;
    for (int i=0; i<4; ++i)
        for (int j=3; j>0; --j)
            if (map[i][j]==0)
            {
                for (int k=j; k>=0; --k)
                    if (map[i][k]!=0)
                    {
                        flag=true;
                        map[i][j]=map[i][k];
                        map[i][k]=0;
                        break;
                    }
            }
    return flag;
}
bool game::removeUp()
{
    bool flag=false;
    for (int j=0; j<4; ++j)
        for (int i=0; i<3; ++i)
            if (map[i][j]==0)
            {
                for (int k=i; k<=3; ++k)
                    if (map[k][j]!=0)
                    {
                        flag=true;
                        map[i][j]=map[k][j];
                        map[k][j]=0;
                        break;
                    }
            }
    return flag;
}
bool game::removeDown()
{
    bool flag=false;
    for (int j=0; j<4; ++j)
        for (int i=3; i>0; --i)
            if (map[i][j]==0)
            {
                for (int k=i; k>=0; --k)
                    if (map[k][j]!=0)
                    {
                        flag=true;
                        map[i][j]=map[k][j];
                        map[k][j]=0;
                        break;
                    }
            }
    return flag;
}
void game::gameStart()
{
    int i;

    drawMap();
    while(i=getch())
    {
        if (isEnd())
        {
            cout<<endl<<"Game Over.";
            cin.get();
            break;
        }
        switch(i)
        {
        case 72:
            if(moveUp())
                randBlock();
            drawMap();

            break;
        case 80:
            if(moveDown())
                randBlock();
            drawMap();
            break;
        case 75:
            if(moveLeft())
                randBlock();
            drawMap();
            break;
        case 77:
            if(moveRight())
                randBlock();
            drawMap();
            break;
        case 32:
            returnForward();
            drawMap();
        default:
            break;
        }
    }
}
void game::returnForward()
{
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            map[i][j]=forwardmap[i][j];
    score=forwardscore;
}
void game::copyMap()
{
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            forwardmap[i][j]=map[i][j];
    forwardscore=score;
}
