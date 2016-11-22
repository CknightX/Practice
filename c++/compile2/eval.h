#ifndef EVAL_H
#define EVAL_H
#include "global_define.h"
#include "scanner.h"
VariableNode eval(string expression, v_Variable& a, shared_ptr<v_Variable> b=nullptr);

#endif