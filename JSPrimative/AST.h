#pragma once
#include <queue>

enum class Operator {
	InvalidOperator,
	Callgrouping,
	ArrayOperator,
	Objectaccess,
	Increment,
	Decrement,
	Negation,
	LogicalNOT,
	BitwiseNOT,
	Division,
	Multiplication,
	Modulus,
	Plus,
	Subtraction,
	Bitwiserightshift,
	Bitwiseleftshift,
	Greater,
	GEq,
	Less,
	LEQ,
	Equality,
	Inequality,
	Identity,
	NIdentity,
	BitwiseAND,
	BitwiseXOR,
	BitwiseOR,
	LogicalAND,
	LogicalOR,
	Conditionalbranch,
	Assignment,
	Multipleevaluation, //Comma
	Objectkey, //:
	ObjectStart, // {
	ObjectEnd, //}
	ArrayStart,//[
	ArrayEnd ,//]
	BracketsStart,//(
	BracketsEnd //)
};



enum  SymbolType { NullType, Newline, WhiteSpace, Symbol, Keyword, NumberLiteral, StringLiteral, VarableName,FunctionLiteral,ObjectSubReferance };



enum class StateEnum { StateNormal, StateNumber, StateVarable, StateStringLiteral, StateStringLiteralDouble, Escape, Comment, MultiLineComment };


struct ASTSymbol {
	const char* Location;
	int Lenght;
	SymbolType SymbolType;
	int SymbolValue;
	int SymbolHash;
};


void DisplaySymbol(ASTSymbol symbol);
ASTSymbol ValueSymbol(int i);

Operator GetOperator(char s);


class Parser {  
private:

	char WorkSpace[100]; //Used to store current item
	char KeyWordState; //if the current part is a keyword
	int NumberState; //if the current part is a number
	StateEnum State = StateEnum::StateNormal;
	ASTSymbol WorkingSymbol;
	int WorkIndex = 0;

	SymbolType ProcessSymbolType(char letter);
	void AddToken(ASTSymbol Token, std::queue<ASTSymbol>* ASTTokenQueue);
	void AddSymbol(const char Sym, std::queue<ASTSymbol>* ASTTokenQueue);
	void FlushWorkspace(const char* Location, std::queue<ASTSymbol>* ASTTokenQueue);
	void StartWorkspace(const char* Location, SymbolType type);
	void ProcessStreamVarable(char Letter);
	void ProcessStreamNumber(char Letter);

public:   
	int ProcessStream(const char* Stream, std::queue<ASTSymbol>* TokenQueue);
};














