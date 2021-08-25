#include "Microcode.h"
#include "ALU.h"
#include "Debug.h"




void ALUCompute(Operator Command) {
	std::cout << "[ALU] Computing:" << ARegister << "_" << BRegister << "\n";
	NumberResultBus = ALUFunction(Command, ARegister, BRegister);
	MicroinstructionRunning = false;
}


int ALUFunction(Operator Command, int A, int B) {
	switch (Command)
	{
	case(Operator::Callgrouping): return 0; //Object
	case(Operator::ArrayOperator): return 0;//Object
	case(Operator::Objectaccess): return 0;//Object
	case(Operator::Increment): return ++A;
	case(Operator::Decrement): return --A;
	case(Operator::Negation): return -A;
	case(Operator::LogicalNOT): return (int)(!(bool)A);
	case(Operator::BitwiseNOT): return ~A;
	case(Operator::Division): return B / A;
	case(Operator::Multiplication): return A * B;
	case(Operator::Modulus): return A % B;
	case(Operator::Plus): return A + B;
	case(Operator::Subtraction): return B - A;
	case(Operator::Bitwiserightshift): return A >> B;
	case(Operator::Bitwiseleftshift): return A << B;
	case(Operator::Greater): return A > B;
	case(Operator::GEq): return A >= B;
	case(Operator::Less): return A > B;
	case(Operator::LEQ): return A >= B;
	case(Operator::Equality): return A == B;
	case(Operator::Inequality): return A != B;
	case(Operator::Identity): return 0;//Object
	case(Operator::NIdentity): return 0;//Object
	case(Operator::BitwiseAND): return A & B;
	case(Operator::BitwiseXOR): return A ^ B;
	case(Operator::BitwiseOR): return A | B;
	case(Operator::LogicalAND): return (int)((bool)A && (bool)B);
	case(Operator::LogicalOR): return (int)((bool)A || (bool)B);
	case(Operator::Conditionalbranch): return 0; //Complicated
	case(Operator::Assignment): return 0;
	default:
		return 0;
	}
}