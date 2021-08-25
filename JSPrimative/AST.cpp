#include "AST.h"
#include "JS.h"
#include "Keywords.h"









SymbolType Parser::ProcessSymbolType(char letter) {
	char TOP = (letter & 0x70) >> 4;
	char Bottom = letter & 0x0F;

	if (letter == 9) {//Tab
		return WhiteSpace;
	}
	else if (letter == 10 || letter == 13) {//CF/LF
		return Newline;
	}
	else if (TOP == 0 || TOP == 1) { 	//control;
		return WhiteSpace;
	}
	else if (TOP == 2 && Bottom == 0) { //Space;
		return WhiteSpace;
	}
	else if (TOP == 3 && (Bottom < 10)) { 	//Number;
		return NumberLiteral;

	}
	else if (TOP == 4 || TOP == 6 || ((Bottom < 10) && (TOP == 5 || TOP == 7))) { //Letter;
		return StringLiteral;
	}
	else {
		return Symbol;
		//Its probally a symbol
	}
}










  



void Parser::AddToken(ASTSymbol Token, std::queue<ASTSymbol>* ASTTokenQueue) {
	ASTTokenQueue->push(Token);
}


void Parser::AddSymbol(const char Sym, std::queue<ASTSymbol>* ASTTokenQueue) {
	ASTSymbol Result;
	Result.Lenght = 0;
	Result.Location = 0;
	
	Result.SymbolValue = (int)GetOperator(Sym);
	Result.SymbolType = Symbol;
	AddToken(Result, ASTTokenQueue);
}





void Parser::FlushWorkspace(const char* Location, std::queue<ASTSymbol>* ASTTokenQueue) {
	WorkingSymbol.Lenght = WorkIndex;




	if (WorkingSymbol.SymbolType == VarableName) {
		if (KeywordValid(KeyWordState)) {
			WorkingSymbol.SymbolValue = KeyWordState;
			WorkingSymbol.SymbolType = Keyword;
		}else {
			WorkingSymbol.SymbolValue = 0;

		}
	}
	else if (WorkingSymbol.SymbolType == NumberLiteral)
    {
			WorkingSymbol.SymbolValue = NumberState;
	}
	else if (WorkingSymbol.SymbolType == StringLiteral)
	{
		WorkingSymbol.SymbolValue = (int)WorkingSymbol.Location;
	}

	if (WorkIndex > 0) {
		AddToken(WorkingSymbol, ASTTokenQueue);
	}

	//resetting
	WorkIndex = 0;
	NumberState = 0;
	WorkingSymbol.SymbolValue = 0;
	WorkingSymbol.Lenght = 0;
	WorkingSymbol.SymbolValue = 0;
	WorkingSymbol.SymbolType = (SymbolType)0;
	
}


void Parser::StartWorkspace(const char* Location, SymbolType type) {
	WorkingSymbol.Location = Location;
	WorkingSymbol.Lenght = 0;
	WorkingSymbol.SymbolType = type;
	KeyWordState = 0;
	WorkIndex = 0;
}




void Parser::ProcessStreamVarable(char Letter) {
	KeyWordState =  KeywordRegex(KeyWordState, Letter);
}

void Parser::ProcessStreamNumber(char Letter) {
	NumberState = NumberState * 10 + (Letter - '0');
}




int Parser::ProcessStream(const char* Stream, std::queue<ASTSymbol>* ASTTokenQueue) {
	char letter = *Stream;
	SymbolType SType = ProcessSymbolType(letter);

	if (State == StateEnum::Comment && SType != Newline) {
		return 0;
	}

	if (SType == WhiteSpace && State != StateEnum::StateStringLiteral &&  State != StateEnum::StateStringLiteralDouble) {
		FlushWorkspace(Stream, ASTTokenQueue);
		State = StateEnum::StateNormal;
	}

	if (SType == Symbol) {
	
		if (letter == '"' ) {
			if (State == StateEnum::StateStringLiteralDouble) {
				State = StateEnum::StateNormal;
			}
			else {
				State = StateEnum::StateStringLiteralDouble;
				StartWorkspace(Stream + 1,StringLiteral);
				return 0;
			}
		} else if(letter == '\'') {
			if (State == StateEnum::StateStringLiteral) {
				State = StateEnum::StateNormal;
			}
			else {
				State = StateEnum::StateStringLiteral;
				StartWorkspace(Stream + 1, StringLiteral);
				return 0;
			}
		} else if (letter == '\\') {

		} else {

			if (State != StateEnum::StateStringLiteral && State != StateEnum::StateStringLiteralDouble) {
				FlushWorkspace(Stream, ASTTokenQueue);
				AddSymbol(*Stream, ASTTokenQueue);
				State = StateEnum::StateNormal;
			}
		}

	}

	if (State == StateEnum::StateNormal) {
		if (SType == NumberLiteral) {
			State = StateEnum::StateNumber;
			StartWorkspace(Stream, NumberLiteral);
		}
		else if (SType == StringLiteral) {
			State = StateEnum::StateVarable;
			StartWorkspace(Stream, VarableName);
		}
	}

	if (State == StateEnum::StateVarable || State == StateEnum::StateNumber || State == StateEnum::StateStringLiteral || State == StateEnum::StateStringLiteralDouble) {
		WorkSpace[WorkIndex++] = letter;

		if (State == StateEnum::StateVarable) {
			ProcessStreamVarable(letter);
		}
		else if(State == StateEnum::StateNumber)
		{
			ProcessStreamNumber(letter);
		}
	}
	


	

}


