#pragma once
#include "AST.h"
enum BuiltINObject { OUndefined, ONull, OString, OFunction,OFunctionScope, OChar, OInt, OFloat, OArray, OObject, OObjectExtention };

struct  JsObject;
struct JSObjectElement;

union JSObjectType {
	JsObject *BaseObj;
	BuiltINObject Builtin;
};




struct JSObjectElement {
	char Keylenght;
	char DataLenght;
	union
	{
		char* Key;
		int keyvalue;
	};
	JSObjectType BaseObject;
	union
	{
		JsObject* referance;
		long value;
	};
}; 



struct  JsObject {
	JSObjectType BaseObject;
	int RefCount;
	char ElementCount;
	char MaxSise;
	JSObjectElement Data;
};
void SetObjectSubReferance(JSObjectElement* Destination, ASTSymbol Value);
BuiltINObject FromSymbolType(ASTSymbol Token);
JsObject* ObjectFromToken(ASTSymbol Token);
void FindObjectValue(JsObject* target, ASTSymbol Key);
ASTSymbol GetObjectResult();
void SetObjectValue(JsObject* target, ASTSymbol Key, ASTSymbol Value);
bool ProcessObject();


