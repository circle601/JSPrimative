#include "AST.h"

Operator GetOperator(char s) {
	switch (s)
	{
	case('+'): return Operator::Plus;
	case('-'): return Operator::Subtraction;
	case('*'): return Operator::Multiplication;
	case('='): return Operator::Assignment;
	case('/'): return Operator::Division;
	case('.'): return Operator::Objectaccess;
	case(','): return Operator::Multipleevaluation;
	case(':'): return Operator::Objectkey;
	case('{'): return Operator::ObjectStart;
	case('}'): return Operator::ObjectEnd;
	case('['): return Operator::ArrayStart;
	case(']'): return Operator::ArrayEnd;
	case('('): return Operator::BracketsStart;
	case(')'): return Operator::BracketsEnd;
	default:
		return Operator::InvalidOperator;
	}
}