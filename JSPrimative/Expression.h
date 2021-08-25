#pragma once

#include "AST.h"

#include <stack>       // std::stack
#include "Microcode.h"   


enum class OperatorInputType {
	None = 0,
	Number = 1,
	Object = 2,
	Both = 3
};

enum class ArrayType {
	ObjectArrayType,
	ArrayArrayType,
	CallArrayType,
};

struct ArrayStackElement {
	char Lenght;
	ArrayType Atype;
};

//returns true to consume token
bool ParseExpression(const ASTSymbol Token, std::stack<ASTSymbol>* ShuntingYardStack, std::stack<ASTSymbol>* ResultStack);
bool FinishExpression(std::stack<ASTSymbol>* ShuntingYardStack, std::stack<ASTSymbol>* ResultStack);
void ExpressionResults(std::stack<ASTSymbol>* ResultStack);
char GetOperatorInputs(Operator Token);
OperatorInputType GetOperatorNumricType(Operator Token);
int ExpressionInt(std::stack<ASTSymbol>* ResultStack);
