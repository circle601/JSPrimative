#include "Debug.h"

JsObject* LastObject;


ASTSymbol ValueSymbol(int i) {
	ASTSymbol WorkingSymbol;
	WorkingSymbol.SymbolValue = i;
	WorkingSymbol.Lenght = 0;
	WorkingSymbol.SymbolType = NumberLiteral;
	return WorkingSymbol;
}





void DisplayObjectPart(JSObjectElement* ObjectPart) {

	if (ObjectPart->BaseObject.Builtin == OObject) {
		DisplayVarable(ObjectPart->referance);
	}
	else if (ObjectPart->BaseObject.Builtin == ONull) {
		std::cout << "Null";
	}
	else if (ObjectPart->BaseObject.Builtin == OUndefined) {
		std::cout << "Undefined";
	}
	else if (ObjectPart->BaseObject.Builtin == OInt) {
		std::cout << (int)ObjectPart->value;
	}
	else if (ObjectPart->BaseObject.Builtin == OString) {
		int Lenght = ObjectPart->DataLenght;
		std::cout << "'";
		for (size_t i = 0; i < Lenght; i++)
		{
			std::cout << (char)((char*)ObjectPart->value)[i];
		}
		std::cout << "'";
	}
	else {
		std::cout << (int)ObjectPart->value;
	}

}



void DisplayVarable(JsObject* Token) {
	if (Token == 0) {
		std::cout << "Null";
		return;
	}
	if (Token->BaseObject.Builtin == OObject) {
		std::cout << "{";
		int Count = Token->ElementCount;
		bool first = true;
		if(Count > 0){
		for (size_t i = 0; i < Count; i++)
		//for (size_t i = Count ; i > 0; i--)
		{
			if (!first) {
				std::cout << ",";
			}
			first = false;
			JSObjectElement* Element = &(&Token->Data)[i];

			unsigned char StrLenght;
			if ((StrLenght = (unsigned char)Element->Keylenght) == 0) {
				std::cout << (int)Element->keyvalue;
			}
			else {
					for (size_t x = 0; x < StrLenght; x++)
					{
						std::cout << (char)Element->Key[x];
					}
				
			}

			std::cout << ":";

			DisplayObjectPart(Element);



		}
		}

		std::cout << "}";
	}
}


const char* OperatorNames[] =
{
	"InvalidOperator",
	"Callgrouping",
	"ArrayOperator",
	"Objectaccess",
	"Increment",
	"Decrement",
	"Negation",
	"LogicalNOT",
	"BitwiseNOT",
	"Division",
	"Multiplication",
	"Modulus",
	"Plus",
	"Subtraction",
	"Bitwiserightshift",
	"Bitwiseleftshift",
	"Greater",
	"GEq",
	"Less",
	"LEQ",
	"Equality",
	"Inequality",
	"Identity",
	"NIdentity",
	"BitwiseAND",
	"BitwiseXOR",
	"BitwiseOR",
	"LogicalAND",
	"LogicalOR",
	"Conditionalbranch",
	"Assignment",
	"Multipleevaluation", //Comma
	"Objectkey",
	"ObjectStart",
	"ObjectEnd",
	"ArrayStart",//[
	"ArrayEnd",//]
	"BracketsStart",
	"BracketsEnd"
};



void DisplayRamStats() {
	std::cout << MemoryObjectsAllocated << " Objects Allocated \n";
	std::cout  << "Last Object Allocated \n";
	DisplayVarable(LastObject);
	std::cout << "\n";
}

void ObjectAllocated(JsObject* object) {
	LastObject = object;
	std::cout << "[Memory] Object Allocated \n";
}



int MathmaticalValueDebug(ASTSymbol symbol) {
	if (symbol.SymbolType == NumberLiteral) {
		return symbol.SymbolValue;
	}
	else if (symbol.SymbolType == ObjectSubReferance) {
		JSObjectElement* Element = (JSObjectElement*)symbol.SymbolValue;
		if (Element->BaseObject.Builtin == OInt) {
			return (int)Element->value;
		}
	}
	return true;
}





const char* Names[] = { "Null", "Newline", "WhiteSpace","Symbol","Keyword", "Number", "String", "Object","Function","ObjectSubReferance" };







void DisplaySymbol(ASTSymbol Token) {
	std::cout << Names[Token.SymbolType];
	std::cout << "{";


	if (Token.Lenght != 0) {
		for (unsigned char i = 0; Token.Location != 0 && i < Token.Lenght; i++) { // str[i] tested here
			std::cout << Token.Location[i]; // but it has no assigned value until here.
		}
	}

	if (Token.SymbolType == NumberLiteral) {
		std::cout << ":";
		std::cout << Token.SymbolValue;
	}


	if (Token.SymbolType == Symbol) {
		if (Token.SymbolValue >= (sizeof(OperatorNames) / sizeof(char*))) {
			std::cout << "Unknown operator";
		}
		else {
			std::cout << OperatorNames[Token.SymbolValue];
		}
	}


	if (Token.SymbolType == VarableName) {
		if ((JsObject*)Token.SymbolValue != 0) {
			std::cout << " ";
			DisplayVarable((JsObject*)Token.SymbolValue);
		}

	}
	else if (Token.SymbolType == ObjectSubReferance) {
		DisplayObjectPart((JSObjectElement*)Token.SymbolValue);

	}

	std::cout << "} \n";

}


