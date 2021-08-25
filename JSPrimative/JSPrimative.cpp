// JSPrimative.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "MemAllocator.h"
#include "AST.h"
#include "Expression.h"
#include "Microcode.h"
#include <iostream>
#include "Scope.h"
#include "Debug.h"
#include "Core.h"
#include "StringHash.h"




void TestHash() {
	unsigned int State = 0;
	const char* Data = "Hellofgyhrhsrtyrtyrft";
	unsigned int i = 0;
	while (Data[i] != 0)
	{
		State =	HashLetter(Data[i++], State);
	}

	std::cout << Data;
	std::cout << ":"; 
	std::cout << State;
	std::cout << "\n";

}


void TestHash2() {


	unsigned int State = 1;
	for (size_t i = 0; i < 1000000; i++)
	{
		State = HashLetter(0, State);
	    std::cout << std::hex << State << std::endl;
	}
	
}

void TestHash3() {

	unsigned int State = 657;
	for (size_t a = 'a'; a < 'z'; a++)
	{

		for (size_t b = 'a'; b < 'z'; b++)
		{
			for (size_t c = 'a'; c < 'z'; c++)
			{
				for (size_t d = 'a'; d < 'z'; d++)
				{
					State = 435344534;
					State = HashLetter(a, State);
					State = HashLetter(b, State);
					State = HashLetter(c, State);
					State = HashLetter(d, State);
					std::cout << State;
					std::cout << "\n";
				}
			}
		}
	}

}


int RunCore(const char* Source, char* CodeRegion, AllocatorState Ram) {
	Core core = Core();

	return 0;
}



int Run(const char* Source,char* CodeRegion, AllocatorState Ram) {
	Parser parser = Parser();
	JsObject* ScopeObject = AllocateScope(&Ram);

	ASTSymbol Top = {};
	ASTSymbol TopOperator = {};
	TopOperator.SymbolType = SymbolType::NullType;


	strcpy_s(CodeRegion, 100, Source);


	std::cout << "\n \n";
	std::cout << CodeRegion;
	std::cout << "\n \n";


	std::queue<ASTSymbol> TokenQueue;
	std::stack<ASTSymbol> ShuntingYardStack;
	std::stack<ASTSymbol> ResultStack;

	char Letter;
	int i = 0;
	bool Running = true;
	bool EndofString = false;

	bool ExtraChar = false;


	while (Running) {

		EndofString |= (Letter = CodeRegion[i]) == 0;
		if (!EndofString) {
			parser.ProcessStream(&CodeRegion[i++], &TokenQueue);
		
		}
		else if (EndofString && !ExtraChar) {
			ExtraChar = true;
			parser.ProcessStream(&CodeRegion[i++], &TokenQueue);
		}




		bool Empty = TokenQueue.empty();
		bool ShuntEmpty = ShuntingYardStack.empty();

		bool Mrunning = MicrocodeRunning();

		if (Mrunning) {
			bool SaveResult = false;
			ASTSymbol Result;
			Operator ConsumedOperator = Operator::InvalidOperator;
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
				TopOperator.SymbolType =SymbolType::NullType;
			}


			if (MicrocodeProcess(Top, &Result, &SaveResult, &Ram,&ConsumedOperator)) {
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






		Running = !EndofString || !Empty || Mrunning || !ShuntEmpty;
	}


	std::cout << "\n \n";
	std::cout << "Program Results: \n";
	int Result = ExpressionInt(&ResultStack);
	ExpressionResults(&ResultStack);
	std::cout << "\n \n";
	std::cout << "Sisplaying Scope: \n";
	DisplayVarable(ScopeObject);


	std::cout << "\n \n";




	return Result;
}





void RunTestSet(const char** Testcases, const int* Results, const size_t Count, char* CodeRegion, AllocatorState Ram) {
	for (size_t i = 0; i < Count; i++)
	{
		std::cout << "TEST:" << i << "________________________________";
		_ASSERTE(Run(Testcases[i], CodeRegion, Ram) == Results[i]);
	}
}


void RunCompileTestSet(const char** Testcases, const size_t Count, char* CodeRegion, AllocatorState Ram) {
	for (size_t i = 0; i < Count; i++)
	{
		std::cout << "TEST:" << i << "________________________________";
		Run(Testcases[i], CodeRegion, Ram);
	}
}





void RunNumbersTests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = { "1 " ,"87 ","2*3 ","2+3 ","10+5*2 ","2*5+10 ","1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1 " ,"1*1*1*1*1*1*1*1*1+1*1*1*1*1*1*1 "};
	const int NumbersResults[] = { 1 ,87,6,5,20,20 ,1,2 };
	const int NumbersTestsCount = sizeof(NumbersResults) / sizeof(int);
	RunTestSet(NumbersTestcases, NumbersResults, NumbersTestsCount, CodeRegion, Ram);
}


void RunNumbers2Tests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = {"1 ","1*5 ","1*5+2 ","1*5+2-6 ","5+2-6 " };
	const int NumbersResults[] = { 1 ,5,7,1};
	const int NumbersTestsCount = sizeof(NumbersResults) / sizeof(int);
	RunTestSet(NumbersTestcases, NumbersResults, NumbersTestsCount, CodeRegion, Ram);
}


void RunNumbers3Tests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = { "1-5 ","5-1 " ,"4/2 " ,"10/2 "};
	const int NumbersResults[] = { -4, 4,2,5 };
	const int NumbersTestsCount = sizeof(NumbersResults) / sizeof(int);
	RunTestSet(NumbersTestcases, NumbersResults, NumbersTestsCount, CodeRegion, Ram);
}





void RunObjectsTests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = {"{A:'Hello'} ","{'a':1}.a ","{A:\"hello\"}","{A:'Hello',B'Hi'} ","{A:1,B : {C:1,D : 3,E : 4 + 7}} " };
	const int NumbersTestsCount = sizeof(NumbersTestcases) / sizeof(char*);
	RunCompileTestSet(NumbersTestcases, NumbersTestsCount, CodeRegion, Ram);
}



void RunObjects2Tests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = { "{A:'Hello',B:{A:1*5+2-6,B:{C:1*2*2*2*2,D:3+8,E:4+8}}} ","{A:1,B:{C:1,D:3,E:4+7}} ","{C:2*2*2*2*2,B:3} ","{C:2*2,B:3} " };
	const int NumbersTestsCount = sizeof(NumbersTestcases) / sizeof(char*);
	RunCompileTestSet(NumbersTestcases, NumbersTestsCount, CodeRegion, Ram);
}


void RunObjects3Tests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = { "{C:2*2,B:3}.C ","{C:2*2,B:3}.B " };
	const int NumbersResults[] = { 4 ,3 };
	const int NumbersTestsCount = sizeof(NumbersResults) / sizeof(int);
	RunTestSet(NumbersTestcases, NumbersResults, NumbersTestsCount, CodeRegion, Ram);
}



void RunObjects4Tests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = { "{A:'Hello',B:{A:1*5+2-6,B:{C:1*2*2*2*2,D:3+8,E:4+8}}}.B.A ","{A:'Hello',B:{A:1*5+2-6,B:{C:1*2*2*2*2,D:3+8,E:4+8}}}.B.B.C ","{A:'Hello',B:{A:1*5+2-6,B:{C:1*2*2*2*2,D:3+8,E:4+8}}}.B.B.D ","{A:'Hello',B:{A:1*5+2-6,B:{C:1*2*2*2*2,D:3+8,E:4+8}}}.B.B.E " };
	const int NumbersResults[] = {1 ,16,11,12 };
	const int NumbersTestsCount = sizeof(NumbersResults) / sizeof(int);
	RunTestSet(NumbersTestcases, NumbersResults, NumbersTestsCount, CodeRegion, Ram);
}



void RunObjects5Tests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = { "{}.A = 1 ","{A:1} ","{A:1}.A = 1 ","{A:'Hello'}.A = 1 ","A = 1 " };
	const int NumbersTestsCount = sizeof(NumbersTestcases) / sizeof(char*);
	RunCompileTestSet(NumbersTestcases, NumbersTestsCount, CodeRegion, Ram);
}

void RunObjects6Tests(char* CodeRegion, AllocatorState Ram) {
	const char* NumbersTestcases[] = { "{}.A = 1 ","a = 1 ","{}.A = 'hello'","{}.A = {}","{C:2*2,B:3}.C = 1 ","A = {A:1} " };
	const int NumbersTestsCount = sizeof(NumbersTestcases) / sizeof(char*);
	RunCompileTestSet(NumbersTestcases, NumbersTestsCount, CodeRegion, Ram);
}








int main()
{

	//TestHash();
	//TestHash2();

	//Startup
	char* pBuffer = new char[1024 * 16];
	AllocatorState Ram = PrepareRegion(pBuffer);
	char* CodeRegion = (char*)AllocateMem(&Ram, 512);


	if (true) {
		RunNumbersTests(CodeRegion, Ram);
		RunNumbers2Tests(CodeRegion, Ram);
		RunNumbers3Tests(CodeRegion, Ram);
		RunObjectsTests(CodeRegion, Ram);
		RunObjects2Tests(CodeRegion, Ram);
		RunObjects3Tests(CodeRegion, Ram);
		RunObjects4Tests(CodeRegion, Ram);
		RunObjects5Tests(CodeRegion, Ram);
		RunObjects6Tests(CodeRegion, Ram);
	}

	
	Run("{}.A = {}", CodeRegion, Ram);
	//Run("{}.A = {} ", CodeRegion, Ram);

	//const char* Source ="2*5+10 " ;// "{A:'Hello',B:{A:1*5+2-6,B:{C:1*2*2*2*2,D:3+8,E:4+7}}}.B.B ";//"{A:1,B:{C:1,D:3,E:4+7}} "

	//todo fix array ordering
	
	//Run("1 ", CodeRegion, Ram);
	// Run("{A:{B:1}}.A.B ", CodeRegion, Ram);

	DisplayRamStats();


	std::cout << "Running________________________________";









	










}



void MainLoop() {

	// ProcessStream(&Hello[i]);

}











// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
