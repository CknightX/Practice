#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED
#include "parser.h"
struct ScriptHeader  //�ű�ͷ����
{
    int StackSize; //Ҫ���ջ�Ĵ�С
    int GlobalDataSize;
    bool IsMainPresent; //ȫ�����ݵĴ�С
    bool MainFuncIndex;
};


#endif // ASM_H_INCLUDED
