#include "JS.h"
#include "AST.h"


void SetObjectSubReferance(JSObjectElement* Destination, ASTSymbol Value) {
	BuiltINObject SourceType = FromSymbolType(Value);

	if (Value.SymbolType == ObjectSubReferance) {
		JSObjectElement* Source = (JSObjectElement*)Value.SymbolValue;
		*Destination = *Source;
	}
	else {
		Destination->value = Value.SymbolValue;
		Destination->DataLenght = Value.Lenght;
		Destination->BaseObject.Builtin = SourceType;
	}

}


JsObject* ObjectFromToken(ASTSymbol Token) {
	if (Token.SymbolType == VarableName) {
		return (JsObject*)Token.SymbolValue;
	}
	else if (Token.SymbolType == ObjectSubReferance) {
		JSObjectElement* Element = (JSObjectElement*)Token.SymbolValue;
		if (Element->BaseObject.Builtin == OObject) {
			return (JsObject*)Element->value;
		}
	}
	return (JsObject*)0;
}




BuiltINObject FromSymbolType(ASTSymbol Token) {
	switch (Token.SymbolType) {
	case(NumberLiteral):
		return OFloat;
	case(StringLiteral):
		return OString;
	case(VarableName):
		return OObject;
	case(FunctionLiteral):
		return OFunction;
	default: return OUndefined;
	}
}











