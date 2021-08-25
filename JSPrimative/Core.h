#pragma once

#include "AST.h"
#include "MemAllocator.h"
#include "JS.h"
#include <queue>
#include <stack>
#include "Expression.h"
#include "Scope.h"

class Core {
	//CoreParts
	Parser parser;

	ASTSymbol* CodeSource;
	int CurrentIndex;
	bool EndofString;
	std::deque<JsObject*> CallStack;
	std::queue<ASTSymbol> TokenQueue;
	std::stack<ASTSymbol> ShuntingYardStack;
	std::stack<ASTSymbol> ResultStack;
	void DoParse();
	void DoExpression(AllocatorState* RamState);
	void RunSymbol(AllocatorState* RamState);
public:
	void RunCore(AllocatorState* RamState);
};
