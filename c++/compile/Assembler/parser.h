#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "scanner.h"
#include<memory>

 // ---- Operand Type Bitfield Flags ---------------------------------------------------

            // The following constants are used as flags into an operand type bit field, hence
            // their values being increasing powers of 2.

            #define OP_FLAG_TYPE_INT        1           // Integer literal value
            #define OP_FLAG_TYPE_FLOAT      2           // Floating-point literal value
            #define OP_FLAG_TYPE_STRING     4           // Integer literal value
            #define OP_FLAG_TYPE_MEM_REF    8           // Memory reference (variable or array
                                                        // index, both absolute and relative)
            #define OP_FLAG_TYPE_LINE_LABEL 16          // Line label (used for jumps)
            #define OP_FLAG_TYPE_FUNC_NAME  32          // Function table index (used for Call)
            #define OP_FLAG_TYPE_HOST_API_CALL  64      // Host API Call table index (used for
                                                        // CallHost)
            #define OP_FLAG_TYPE_REG        128         // Register

// ---- Instruction Opcodes -----------------------------------------------------------

            #define INSTR_MOV               0

            #define INSTR_ADD               1
            #define INSTR_SUB               2
            #define INSTR_MUL               3
            #define INSTR_DIV               4
            #define INSTR_MOD               5
            #define INSTR_EXP               6
            #define INSTR_NEG               7
            #define INSTR_INC               8
            #define INSTR_DEC               9

            #define INSTR_AND               10
            #define INSTR_OR                11
            #define INSTR_XOR               12
            #define INSTR_NOT               13
            #define INSTR_SHL               14
            #define INSTR_SHR               15

            #define INSTR_CONCAT            16
            #define INSTR_GETCHAR           17
            #define INSTR_SETCHAR           18

            #define INSTR_JMP               19
            #define INSTR_JE                20
            #define INSTR_JNE               21
            #define INSTR_JG                22
            #define INSTR_JL                23
            #define INSTR_JGE               24
            #define INSTR_JLE               25

            #define INSTR_PUSH              26
            #define INSTR_POP               27

            #define INSTR_CALL              28
            #define INSTR_RET               29
            #define INSTR_CALLHOST          30

            #define INSTR_PAUSE             31
            #define INSTR_EXIT              32


struct FuncNode
{
    int Index;
    string FuncName;
    int EntryPoint;  //��ڵ�
    int ParamCount;  //��������
    int LocalDataSize; //�ֲ����ݴ�С
};
struct SymbolNode
{
    int Index;
    string SymbolName;
    int Size;
    int StackIndex; //���ŵĶ�ջ����
    int FuncIndex; //�������ڵĺ�������
};
struct InstrLookup //ָ�����
{
    string Name;
    int Opcode; //������
    int OpCount; //����������
    shared_ptr<int> OpList; //�������б�ָ��
};
class Parser
{
public:
    Parser(string path);
private:
    void InitInstrTable(); //��ʼ��ָ����ұ�

    void AddFunc(string name,int entrypoint);
    int AddInstrLookup(string name,int code,int cunt);
    int AddSymbol(string name,int size,int sindex,int findex);
    int AddLable(string name,int tindex,int findex);
    void SetOpType(int index,int opdex,int types);
    void SetFuncInfo(string name,int paramcount,int localdatasize);

    vector<InstrLookup>::iterator GetInstrByName(string name); //ͨ�����ƻ�ȡ��Ӧָ��
    int GetStackIndexByIdent(string name,int funcindex);
    int GetSizeByIdent(string name,int funcindex);

    Scanner scan; //�ʷ�������
    vector<string> StrTable; //�ַ�����
    vector<shared_ptr<FuncNode>> FuncTable; //������
    vector<shared_ptr<SymbolNode>> SymbolTable; //���ű�

    //Ԥ���
    vector<shared_ptr<InstrLookup>> InstrTable; //ָ����ұ�
};


#endif // PARSER_H_INCLUDED
