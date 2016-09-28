#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED
#include "parser.h"
struct ScriptHeader  //脚本头数据
{
    int StackSize; //要求堆栈的大小
    int GlobalDataSize;
    bool IsMainPresent; //全局数据的大小
    bool MainFuncIndex;
};


#endif // ASM_H_INCLUDED
