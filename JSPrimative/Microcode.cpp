#include "Microcode.h"
#include "Debug.h"

#include "ALU.h"



int ARegister;
int BRegister;
ASTSymbol CREgister;
ASTSymbol DREgister;

int NumberResultBus;
ASTSymbol ObjectResultBus;
ASTSymbol HeldSymbol;

JsObject* ObjectRefBus;


int Step;
int SubStep;


enum class CommandState
{
	LoadState,Evaluate,Store
};




bool MicroinstructionRunning;

Operator command;











bool running = false;

bool MicrocodeRunning() {
	return running;
}




void PrepareObject(int Step) {

}


void StartOPCode(ASTSymbol Command) {
	command = (Operator)Command.SymbolValue;
	Step = 0;
	SubStep = 0;
	running = true;
	if (Command.SymbolType == Symbol) {
		if ((Operator)Command.SymbolValue == Operator::ObjectEnd) {
			Step = 4;
		}else if((Operator)Command.SymbolValue == Operator::ArrayEnd) {
			Step =10;
		}
		else if ((Operator)Command.SymbolValue == Operator::Objectaccess) {
			Step = 16;
		}
		else if ((Operator)Command.SymbolValue == Operator::ObjectStart || (Operator)Command.SymbolValue == Operator::ArrayStart) {
			Step = 100;
		}
	}
}












bool MathmaticalValue(ASTSymbol symbol, int* Accumulator, int Step) {
	*Accumulator = MathmaticalValueDebug(symbol);
	return true;
}






// true to consume token
bool LoadState(ASTSymbol Token,bool* Consume) {

	OperatorInputType NumricType = GetOperatorNumricType(command);
	if (NumricType == OperatorInputType::Number) {
		int* ResultTarget;
		if (Step == 0) {
			ResultTarget = &ARegister;
		}
		else {
			ResultTarget = &BRegister;
		}
		if (!MathmaticalValue(Token, ResultTarget, SubStep++)) {
			return false;
		}
	}
	else {
		if (Step == 0) {
			CREgister = Token;
		}
		else {
			DREgister = Token;
		}
	}

	if (GetOperatorInputs(command)  == Step + 1) {
		SubStep = 0;
		*Consume = true;
		return true;
	}
	else {
		*Consume = true;
		SubStep = 0;
		Step++;
		return false;
	}

}




//Not used
bool ProcessALU(ASTSymbol Token, ASTSymbol* Result, bool* ResultReady, AllocatorState* RamState) {

	ASTSymbol ResultObj;
	bool Consume = false;
	switch (Step)
	{
	case(0): //Load Data for ALU operaton
	case(1):
		Consume = false;
		if (LoadState(Token, &Consume)) {
			Step = 2;
			return true;
		}
		return Consume;
	case(2): //Compute ALU operaton
		ALUCompute(command);
		*Result = ValueSymbol(NumberResultBus);
		*ResultReady = true;
		running = false;
		return false;
	}
}






// true to consume token
bool MicrocodeProcess(ASTSymbol Token, ASTSymbol* Result, bool* ResultReady, AllocatorState* RamState, Operator* ConsumeOperand) {
	ASTSymbol ResultObj;
	bool Consume = false;
	OperatorInputType NumricType;

	switch (Step)
	{
	case(0): //Load Data for ALU operaton
	case(1):
		Consume = false;
		if (LoadState(Token,&Consume)) {
			Step = 2;
			return true;
		}
		return Consume;
	
		
	case(2): //Compute ALU operaton

		NumricType = GetOperatorNumricType(command);
		if (NumricType == OperatorInputType::Number) {
			ALUCompute(command);
			*Result = ValueSymbol(NumberResultBus);
			*ResultReady = true;
			running = false;
			return false;
		}
		else {
			RunObjectOperator(command, DREgister , CREgister);
		}
	case(3): //Save Result
		NumricType = GetOperatorNumricType(command);
		if (NumricType == OperatorInputType::Number) {
			*Result = ValueSymbol(NumberResultBus);
			*ResultReady = true;
			running = false;
			return false;
		}
		else {
			if (ProcessObject()) {
				*Result = ObjectResultBus;
				*ResultReady = true;
				running = false;
			}
			return false;
		}


		//Everything below is broken


	case(4): //Request Object
		RequestObject(RamState, 10);
		Step = 5;
		return false;
		
	case(5): //Collect Object Memory
		if (0 != (ObjectRefBus = (JsObject*)MemoryResult(RamState))){
			Step = 6;
		}
		return false;
	case(6)://Collect ObjectKey
		if (Token.SymbolType == Symbol && (Operator)Token.SymbolValue == Operator::ObjectStart) {
			Step = 9;
			*ConsumeOperand = Operator::ObjectStart;
		}
		else {
			HeldSymbol = Token;
			Step = 7;
		}
		return true;
	case(7): //Collect Object parts
		if (Token.SymbolType == Symbol && (Operator)Token.SymbolValue == Operator::ObjectStart) {
			Step = 9;
			*ConsumeOperand = Operator::ObjectStart;
			return true;
		}
		else {
			SetObjectValue(ObjectRefBus, Token , HeldSymbol);
			Step = 8;
			return true;
		}
	
	case(8): //Process Object
		if (ProcessObject()) {
			Step = 6;
		}
		return false;
	case(9): //Return Result
	
		ResultObj.SymbolType = VarableName;
		ResultObj.SymbolValue = (int)ObjectRefBus;
		*Result = ResultObj;
		*ResultReady = true;
		running = false;
		return false;
	case(10): //Array
		HeldSymbol = Token;
		Step = 11;
		ARegister = 0;
		return true;
	case(11): //Array Branch
		if (Token.SymbolType == Symbol && (Operator)Token.SymbolValue == Operator::ArrayOperator) { //P
			//Step = 7;
			return true;
		}
		else {
			RequestObject(RamState, 10);
			Step = 12;
		}
		return false;
	case(12): //Array Allocate
		if (0 != (ObjectRefBus = (JsObject*)MemoryResult(RamState))) {
			SetObjectValue(ObjectRefBus, ValueSymbol(ARegister++), HeldSymbol);
			Step = 14;
		}
		return false;
	case(13):
		
		if (Token.SymbolType == Symbol && (Operator)Token.SymbolValue == Operator::ArrayStart) { //P
			Step = 15;
			*ConsumeOperand = Operator::ArrayStart;
			return false;
		}
		else {
			SetObjectValue(ObjectRefBus, ValueSymbol(ARegister++), Token);
			Step = 14;
			return true;
		}
	case(14):
		if (ProcessObject()) {
			Step = 13;
		}
		return false;
	case(15): //Return Result
		
		ResultObj.SymbolType = VarableName;
		ResultObj.SymbolValue = (int)ObjectRefBus;
		*Result = ResultObj;
		*ResultReady = true;
		running = false;
		return false;
	case(16):
		HeldSymbol = Token;
		Step = 17;
		return true;
	case(17):
		ObjectRefBus = ObjectFromToken(Token);
		FindObjectValue(ObjectRefBus, HeldSymbol);
		Step = 18;
		return true;
	case(18):
		if (ProcessObject()) {
			*Result = GetObjectResult();
			*ResultReady = true;
			running = false;
			
		}
		return false;
	case(19):
		


	case(100):
		return true;


	default:
		break;
	}
	return true;
}
