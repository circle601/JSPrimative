#include "Core.h"








void Core::RunCore(AllocatorState* RamState) {
	Core::DoParse();
	Core::DoExpression(RamState);
}



void Core::RunSymbol(ASTSymbol* CodeSource, AllocatorState* RamState) {
	JsObject* ScopeObject = AllocateScope(RamState);
	
	 CurrentIndex = 0;
	 bool EndofString = false;

	std::deque<JsObject*> CallStack;
	std::queue<ASTSymbol> TokenQueue;
	std::stack<ASTSymbol> ShuntingYardStack;
	std::stack<ASTSymbol> ResultStack;


}


void Core::DoParse() {
	//Todo need to add extra Char again
	bool AtEnd;
	if (CurrentIndex >= CodeSource->Lenght) {
		AtEnd = true;
	}
	const char* Letter = &(CodeSource->Location[CurrentIndex]);
	if (*Letter == 0) AtEnd = true;
	if (EndofString && AtEnd) return;
	parser.ProcessStream(Letter, &TokenQueue);
	if (!AtEnd) {
		CurrentIndex++;
	}
	else {
		EndofString = true;
	}
}





void  Core::DoExpression(AllocatorState* RamState) {

	bool Empty = TokenQueue.empty();
	bool ShuntEmpty = ShuntingYardStack.empty();

	bool Mrunning = MicrocodeRunning();

	if (Mrunning) {
		bool SaveResult = false;
		ASTSymbol Result;
		ASTSymbol Top;
		Operator ConsumedOperator = Operator::InvalidOperator;
		ASTSymbol TopOperator;

		if (!ResultStack.empty()) {
			Top = ResultStack.top();
		}
		else {
			Top.SymbolType = SymbolType::NullType;
		}

		if (!ShuntingYardStack.empty()) {
			TopOperator = ShuntingYardStack.top();
		}
		else {
			TopOperator.SymbolType = SymbolType::NullType;
		}


		if (MicrocodeProcess(Top, &Result, &SaveResult, RamState, &ConsumedOperator)) {
			ResultStack.pop();
		}
		if (ConsumedOperator == (Operator)TopOperator.SymbolValue && TopOperator.SymbolType == Symbol) {
			ShuntingYardStack.pop();
		}


		if (SaveResult) {
			ResultStack.push(Result);
		}
	}
	else {

		if (!Empty) {
			ASTSymbol token = TokenQueue.front();
			if (ParseExpression(token, &ShuntingYardStack, &ResultStack)) {
				TokenQueue.pop();
			}
		}
		else if (!ShuntEmpty && EndofString) {
			FinishExpression(&ShuntingYardStack, &ResultStack);
		}

	}


}