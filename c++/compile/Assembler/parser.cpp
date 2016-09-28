#include "parser.h"
Parser::Parser(string path)
:scan(path)
{
    InitInstrTable();
}
int Parser::AddInstrLookup(string name,int code,int cunt)
{
    shared_ptr<InstrLookup> tmp=make_shared<InstrLookup>();
    tmp->Name=name;
    tmp->Opcode=code;
    tmp->OpCount=cunt;
    tmp->OpList=shared_ptr<int>(new int[cunt]);
    InstrTable.push_back(tmp);
    return InstrTable.size()-1;
}
void Parser::SetOpType(int index,int opdex,int types)
{
    *((*InstrTable[index]).OpList.get()+index)=types;
}
void Parser::InitInstrTable()
{
    // Create a temporary index to use with each instruction

    int iInstrIndex;

    // The following code makes repeated calls to AddInstrLookup () to add a hardcoded
    // instruction set to the assembler's vocabulary. Each AddInstrLookup () call is
    // followed by zero or more calls to SetOpType (), whcih set the supported types of
    // a specific operand. The instructions are grouped by family.

    // ---- Main

    // Mov          Destination, Source

    iInstrIndex = AddInstrLookup ( "Mov", INSTR_MOV, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // ---- Arithmetic

    // Add         Destination, Source

    iInstrIndex = AddInstrLookup ( "Add", INSTR_ADD, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Sub          Destination, Source

    iInstrIndex = AddInstrLookup ( "Sub", INSTR_SUB, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Mul          Destination, Source

    iInstrIndex = AddInstrLookup ( "Mul", INSTR_MUL, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Div          Destination, Source

    iInstrIndex = AddInstrLookup ( "Div", INSTR_DIV, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Mod          Destination, Source

    iInstrIndex = AddInstrLookup ( "Mod", INSTR_MOD, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Exp          Destination, Source

    iInstrIndex = AddInstrLookup ( "Exp", INSTR_EXP, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Neg          Destination

    iInstrIndex = AddInstrLookup ( "Neg", INSTR_NEG, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Inc          Destination

    iInstrIndex = AddInstrLookup ( "Inc", INSTR_INC, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Dec          Destination

    iInstrIndex = AddInstrLookup ( "Dec", INSTR_DEC, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // ---- Bitwise

    // And          Destination, Source

    iInstrIndex = AddInstrLookup ( "And", INSTR_AND, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Or           Destination, Source

    iInstrIndex = AddInstrLookup ( "Or", INSTR_OR, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // XOr          Destination, Source

    iInstrIndex = AddInstrLookup ( "XOr", INSTR_XOR, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Not          Destination

    iInstrIndex = AddInstrLookup ( "Not", INSTR_NOT, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // ShL          Destination, Source

    iInstrIndex = AddInstrLookup ( "ShL", INSTR_SHL, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // ShR          Destination, Source

    iInstrIndex = AddInstrLookup ( "ShR", INSTR_SHR, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // ---- String Manipulation

    // Concat       String0, String1

    iInstrIndex = AddInstrLookup ( "Concat", INSTR_CONCAT, 2 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG |
                OP_FLAG_TYPE_STRING );

    // GetChar      Destination, Source, Index

    iInstrIndex = AddInstrLookup ( "GetChar", INSTR_GETCHAR, 3 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG |
                OP_FLAG_TYPE_STRING );
    SetOpType ( iInstrIndex, 2, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG |
                OP_FLAG_TYPE_INT );

    // SetChar      Destination, Index, Source

    iInstrIndex = AddInstrLookup ( "SetChar", INSTR_SETCHAR, 3 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG |
                OP_FLAG_TYPE_INT );
    SetOpType ( iInstrIndex, 2, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG |
                OP_FLAG_TYPE_STRING );

    // ---- Conditional Branching

    // Jmp          Label

    iInstrIndex = AddInstrLookup ( "Jmp", INSTR_JMP, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_LINE_LABEL );

    // JE           Op0, Op1, Label

    iInstrIndex = AddInstrLookup ( "JE", INSTR_JE, 3 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

    // JNE          Op0, Op1, Label

    iInstrIndex = AddInstrLookup ( "JNE", INSTR_JNE, 3 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

    // JG           Op0, Op1, Label

    iInstrIndex = AddInstrLookup ( "JG", INSTR_JG, 3 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

    // JL           Op0, Op1, Label

    iInstrIndex = AddInstrLookup ( "JL", INSTR_JL, 3 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

    // JGE          Op0, Op1, Label

    iInstrIndex = AddInstrLookup ( "JGE", INSTR_JGE, 3 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

    // JLE           Op0, Op1, Label

    iInstrIndex = AddInstrLookup ( "JLE", INSTR_JLE, 3 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 1, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
    SetOpType ( iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

    // ---- The Stack Interface

    // Push          Source

    iInstrIndex = AddInstrLookup ( "Push", INSTR_PUSH, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Pop           Destination

    iInstrIndex = AddInstrLookup ( "Pop", INSTR_POP, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // ---- The Function Interface

    // Call          FunctionName

    iInstrIndex = AddInstrLookup ( "Call", INSTR_CALL, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_FUNC_NAME );

    // Ret

    iInstrIndex = AddInstrLookup ( "Ret", INSTR_RET, 0 );

    // CallHost      FunctionName

    iInstrIndex = AddInstrLookup ( "CallHost", INSTR_CALLHOST, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_HOST_API_CALL );

    // ---- Miscellaneous

    // Pause        Duration

    iInstrIndex = AddInstrLookup ( "Pause", INSTR_PAUSE, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );

    // Exit         Code

    iInstrIndex = AddInstrLookup ( "Exit", INSTR_EXIT, 1 );
    SetOpType ( iInstrIndex, 0, OP_FLAG_TYPE_INT |
                OP_FLAG_TYPE_FLOAT |
                OP_FLAG_TYPE_STRING |
                OP_FLAG_TYPE_MEM_REF |
                OP_FLAG_TYPE_REG );
}

