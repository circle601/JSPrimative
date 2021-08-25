#pragma once

#include <stack>   
#include "AST.h"
#include "MemAllocator.h"
#include "JS.h"
#include "Expression.h"

bool MicrocodeProcess(ASTSymbol Token, ASTSymbol* Result, bool* ResultReady, AllocatorState* RamState, Operator* ConsumeOperand);
void StartOPCode(ASTSymbol command);
bool MicrocodeRunning();
int MathmaticalValueDebug(ASTSymbol command);

extern int NumberResultBus;
extern ASTSymbol ObjectResultBus;
extern JsObject* ObjectReferanceBus;

extern int ARegister;
extern int BRegister;
extern ASTSymbol CREgister;
extern ASTSymbol DREgister;
extern bool MicroinstructionRunning;