#include "JS.h"
#include "AST.h"
#include "Microcode.h"
#include "ALU.h"
//JsObject* CurrentObject;
//extern JsObject* ObjectBus;


JsObject* CurrentObject;
int MaxCount;
int CurrentPos;

ASTSymbol CompareToken;
ASTSymbol NewValue;



bool SaveMode;
int StringCheck;



enum ObjectState
{
	DoneState,
	LoadObjectState,
	FindState,
	SetValueState,
	ExtendObjectState,
	GetValueState,
	ClearPastValue,
};
ObjectState state;



void RunObjectOperator(Operator Command, ASTSymbol A,ASTSymbol B) {
	switch (Command)
	{
		case(Operator::Objectaccess): FindObjectValue(ObjectFromToken(A),B);
		case(Operator::Assignment): DOAssignment(A,B);
	}
}




void DOAssignment(ASTSymbol target,  ASTSymbol Value) {
	state = DoneState;
	if (target.SymbolType == VarableName) {
		if (target.SymbolHash != -1) {
			JsObject* Object = ObjectFromToken(target);
		}
	}
	else if (target.SymbolType == ObjectSubReferance ){
		JSObjectElement* Element = (JSObjectElement*)target.SymbolValue;
		SetObjectSubReferance(Element, Value);
	}
	ObjectResultBus = target;
}







void SetObjectValue(JsObject* target,ASTSymbol Key, ASTSymbol Value) {
	CurrentObject = target;
	NewValue = Value;
	SaveMode = true;
	CompareToken = Key;
	state = LoadObjectState;
	StringCheck = 0;
}


void FindObjectValue(JsObject* target, ASTSymbol Key) {

	CurrentObject = target;
	SaveMode = false;
	CompareToken = Key;
	state = LoadObjectState;
	StringCheck = 0;
}



ASTSymbol GetObjectResult() {
	ASTSymbol Result;
	Result.SymbolType = ObjectSubReferance;
	JSObjectElement* Element = &(&CurrentObject->Data)[CurrentPos];
	Result.SymbolValue = (int)Element;
	return Result;
}


void LoadObject() {
	 MaxCount = CurrentObject->MaxSise;
	 CurrentPos = 0;
}


bool FindObjectPart() {
	JSObjectElement Element = (&CurrentObject->Data)[CurrentPos];

	int keyLenght = CompareToken.Lenght;

	char* StringLocation = (char*)CompareToken.Location;
	if (CompareToken.SymbolType == NumberLiteral) keyLenght = 0;

	if (CurrentPos == (MaxCount - 1)) {
		state = ExtendObjectState;
		return false;
	}
	else if (Element.BaseObject.Builtin == OUndefined) {
		state = SetValueState;
		
		return true;
	}else if (Element.Keylenght == keyLenght) {
		if (Element.Keylenght == 0) {
			if (Element.keyvalue == CompareToken.SymbolValue) {
				if (SaveMode) {
					state = SetValueState;
				}
				else {
					state = GetValueState;
				}
				return true;
			}
			
		}
		else {
			if (StringCheck == keyLenght) {
				if (SaveMode) {
					state = SetValueState;
				}
				else {
					state = GetValueState;
				}
				return true;
			}
			if (Element.Key[StringCheck] == (StringLocation)[StringCheck]) {
				StringCheck++;
				return false;
			}
			

		}
	}else if(Element.BaseObject.Builtin == OObjectExtention) {
		CurrentObject = (JsObject*)Element.value;
		CurrentPos = -1;
		state = LoadObjectState;
		return false;
	}

	StringCheck = 0;
	CurrentPos++;
	return false;
	
}

void Prevousvalue() {
	JSObjectElement* Element = &(&CurrentObject->Data)[CurrentPos];
	if (Element->BaseObject.Builtin == OObject) {
		Element->referance->RefCount--;

	}
}




void Setvalue() {

	JSObjectElement* Element = &(&CurrentObject->Data)[CurrentPos];
	CurrentObject->ElementCount++;

	if (CompareToken.SymbolType == StringLiteral || CompareToken.SymbolType == VarableName) {
		Element->Keylenght = CompareToken.Lenght;
		Element->Key = (char*)CompareToken.Location;
			//todo add a lenght check;
	}else if (CompareToken.SymbolType == NumberLiteral) {
		Element->Keylenght = 0;
		Element->keyvalue = CompareToken.SymbolValue;
	} 

	if (!SaveMode) return;
		
		
	if (NewValue.SymbolType == VarableName) {
	
		Element->BaseObject.Builtin = OObject;
		Element->referance = (JsObject*)NewValue.SymbolValue;
		Element->referance->RefCount++;
	}
	else if(NewValue.SymbolType == StringLiteral) {
		Element->BaseObject.Builtin = OString;
		Element->DataLenght = NewValue.Lenght;
		Element->value = (int)NewValue.Location;
	}
	else if (NewValue.SymbolType == NumberLiteral) {
		
		Element->BaseObject.Builtin = OInt;
		Element->value = (int)NewValue.SymbolValue;
	}
}



void ExtendObject() {


}





bool ProcessObject() {
	switch (state)
	{
	case LoadObjectState:
		LoadObject();
		state = FindState;
		break;
	case FindState:
		FindObjectPart();
		break;
	case SetValueState:
		Setvalue();
		return true;
		break;
	case GetValueState:
		return true;
		break;
	case ClearPastValue:
		break;
	case ExtendObjectState:
		return true;
		break;
	case DoneState:
		return true;
	default:
		break;
	}
	return false;
}