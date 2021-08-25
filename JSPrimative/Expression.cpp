#include "Expression.h"
#include "Debug.h"


int ALUFunction(Operator Command, int A, int B);



//http://www.scriptingmaster.com/javascript/operator-precedence.asp



enum class ValueState{
	None,
	Value,
	ValueDemand,
	ObjectValueDemand,
	ObjectKeyDemand,
	Invalid,
};




ValueState ExpressionStateTransition(ValueState PastState, ASTSymbol token) {
	return ValueState::None;
}


OperatorInputType GetOperatorNumricType(Operator Token) {
	if (Token < Operator::Objectaccess) {
		return OperatorInputType::None;
	}
	else	if (Token == Operator::Objectaccess) {
		return  OperatorInputType::Object;
	}
	else if (Token < Operator::Division)
	{
		return  OperatorInputType::Number;
	}
	else if (Token < Operator::Conditionalbranch)
	{
		return  OperatorInputType::Number;
	}
	else if (Token == Operator::Conditionalbranch)
	{
		return OperatorInputType::None;
	}
	else if (Token == Operator::Assignment)
	{
		return OperatorInputType::Both;
	}
	else
	{
		return OperatorInputType::None;
	}
	return OperatorInputType::None;

}



char GetOperatorInputs(Operator Token) {
	if (Token < Operator::Objectaccess) {
		return -1;
	}else	if (Token == Operator::Objectaccess) {
		return 2;
	}
	else if(Token < Operator::Division)
	{
		return 1;
	}
	else if (Token < Operator::Conditionalbranch)
	{
		return 2;
	}
	else if (Token == Operator::Conditionalbranch)
	{
		return -1;
	}
	else if (Token == Operator::Assignment)
	{
		return 2;
	}
	else
	{
		return -1;
	}
	return -1;
}













     char OperatorPrecedence(Operator Token) {
	if (Token < Operator::Increment) {
		return 1;
	}
	else if(Token < Operator::Division)
	{
		return 2;
	}
	else if (Token < Operator::Plus) {
		return 3;
	}
	else if (Token < Operator::Bitwiserightshift) {
		return 4;
	}
	else if (Token < Operator::Greater) {
		return 5;
	}
	else if (Token < Operator::Equality) {
		return 6;
	}
	else if (Token < Operator::BitwiseAND) {
		return 7;
	}
	else if (Token < Operator::BitwiseXOR) {
		return 8;
	}
	else if (Token == Operator::BitwiseXOR) {
		return 9;
	}
	else if (Token == Operator::BitwiseOR) {
		return 10;
	}
	else if (Token == Operator::LogicalAND) {
		return 11;
	}
	else if (Token == Operator::LogicalOR) {
		return 12;
	}
	else if (Token == Operator::Conditionalbranch) {
		return 13;
	}
	else if (Token == Operator::Assignment) {
		return 15;
	}
	else if (Token == Operator::Multipleevaluation) {
		return 15;
	}
	else if (Token == Operator::Objectkey) {
		return 0;
	}
	else if (Token == Operator::ObjectEnd) {
		return 15;
	}
	else if (Token == Operator::ObjectStart) {
		return 15;
	}
	else {
		return 0;
	}
}



char OperatorAssociativityLR(Operator Token) {
	if (Token == Operator::Objectaccess) {
		return true;
	}else
	if (Token < Operator::Increment) {
		return false;
	}
	else if(Token < Operator::Division) {
		return false;
	}
	else if (Token < Operator::Assignment) {
		return true;
	}
	else if (Token < Operator::Multipleevaluation) {
		return false;
	}
	else if (Token == Operator::Multipleevaluation) {
		return true;
	}
	else if (Token == Operator::ObjectEnd) {
		return true;
	}
	else if (Token == Operator::ArrayEnd) {
		return true;
	}
	else if (Token == Operator::ObjectStart) {
		return true;
	}
	
	return false;
}


char OperatorPrecedence(ASTSymbol Token) {
	if (Token.SymbolType == Keyword) {
		return 2;
	}
	else if (Token.SymbolType == Symbol) {
		return  OperatorPrecedence((Operator)Token.SymbolValue);
	}
	else {
		return 0;
	}
}

bool OperatorAssociativityLR(ASTSymbol Token) {

	if (Token.SymbolType == Keyword) {
		return false;
	}
	else if (Token.SymbolType == Symbol) {
		return OperatorAssociativityLR((Operator)Token.SymbolValue);
	}
	else {
		return false;
	}
}





void ReduceSymbol(ASTSymbol peek) {
	std::cout << "[Espression] Reducing Stack ";
	DisplaySymbol(peek);
	StartOPCode(peek);
}

void ReduceStack(std::stack<ASTSymbol>* ShuntingYardStack) {

	ASTSymbol peek = (ASTSymbol)ShuntingYardStack->top();

	ShuntingYardStack->pop();
	ReduceSymbol(peek);
}











bool ParseExpression(const ASTSymbol Token, std::stack<ASTSymbol>* ShuntingYardStack, std::stack<ASTSymbol>* ResultStack) { //, std::stack<int>* ArrayStack

	DisplaySymbol(Token);

	bool TokenAssociativity = !OperatorAssociativityLR(Token);
	char TokenPrecedence = OperatorPrecedence(Token);



	bool IsLiteral = Token.SymbolType == Keyword || Token.SymbolType == NumberLiteral || Token.SymbolType == StringLiteral || Token.SymbolType == VarableName;
	bool IsStartToken = Token.SymbolType == Symbol && ((Operator)Token.SymbolValue == Operator::ObjectStart || (Operator)Token.SymbolValue == Operator::ArrayStart);


	bool ISResult = IsLiteral || IsStartToken;
	bool ISOperator = !ISResult;

	bool PreventPush = Token.SymbolType == Symbol && ((Operator)Token.SymbolValue == Operator::Multipleevaluation || (Operator)Token.SymbolValue == Operator::Objectkey);
	bool PushOnly = ShuntingYardStack->empty();


	bool NOPrecedence = true;

	if (!PushOnly && ISOperator) {
		ASTSymbol peek = ShuntingYardStack->top();
		char StackPrecedence = OperatorPrecedence(peek);
		bool IsLastStartToken = peek.SymbolType == Symbol && ((Operator)peek.SymbolValue == Operator::ObjectStart || (Operator)peek.SymbolValue == Operator::ArrayStart);
		PushOnly |= IsLastStartToken;


		if ((peek.SymbolType == Symbol && ((Operator)peek.SymbolValue == Operator::ObjectEnd || (Operator)peek.SymbolValue == Operator::ArrayEnd))) {
			StackPrecedence = 0;
		}
		else {
		}

  		NOPrecedence = (StackPrecedence != 0 && (TokenPrecedence < StackPrecedence || TokenPrecedence == StackPrecedence && TokenAssociativity));
	}

	if (ISResult) {
		ResultStack->push(Token);
		if (IsStartToken) {
			ShuntingYardStack->push(Token);
		}
		return true;
	}
	else if (PreventPush && PushOnly) {

		
	}else if (!PreventPush && (PushOnly || NOPrecedence)) {
		ShuntingYardStack->push(Token);
		return true;

	}
	else if (!NOPrecedence) {
		ReduceStack(ShuntingYardStack);
		return false;
	}
}





bool FinishExpression(std::stack<ASTSymbol>* ShuntingYardStack, std::stack<ASTSymbol>* ResultStack) {
	if (ShuntingYardStack->empty()) {
		return false;
	}
	else {
		ReduceStack(ShuntingYardStack);
		return true;
	}
}



void ExpressionResults(std::stack<ASTSymbol>* ResultStack) {
	while (!ResultStack->empty())
	{

		DisplaySymbol(ResultStack->top());
		ResultStack->pop();
	}
}

int ExpressionInt(std::stack<ASTSymbol>* ResultStack) {
	if(!ResultStack->empty())
	{
		return MathmaticalValueDebug(ResultStack->top());
	}
	return 0;
}


