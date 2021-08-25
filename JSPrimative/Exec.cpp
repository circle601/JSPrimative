#include "Exec.h"
#include "AST.h"

#include <stack>          // std::stack



enum ExecStateEnum
{
	ENone,
	EValue,
	EKeyword,
	EUnitary,
	EBinaryMul,
	EBinaryAddative,
	EBinaryPost,
	EGroup,
};

struct ExecState {
	ExecStateEnum State;
	ASTSymbol Symbol;
};



//Current Symbol
//Look ahead



ExecStateEnum SymbolClass(ASTSymbol Token) {
	if (Token.SymbolType == Symbol) {
		switch (Token.SymbolValue)
		{
		case('+'):
			return EBinaryAddative;
		case('*'):
			return EBinaryMul;
		default:
			break;
		}
	}else if (Token.SymbolType == Keyword) {
		return EUnitary;
	}
}

int GetPrecedence(ExecStateEnum StateType) {
	switch (StateType)
	{
	case(ENone): return 0;
	case(EValue): return 0;
	case(EKeyword): return 0;
	case(EUnitary): return 3;
	case(EBinaryMul): return 2;
	case(EBinaryAddative): return 1;
	default: return 1;
	}
}

bool StatePush(ExecStateEnum StateType) {
	switch (StateType)
	{
	case(ENone): return false;
	case(EValue): return true;
	case(EKeyword): return 0;
	case(EUnitary): return 3;
	case(EBinaryMul): return 2;
	case(EBinaryAddative): return 1;
	default: return 1;
	}
}









