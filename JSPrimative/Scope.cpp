
#include "Scope.h"





JsObject* AllocateScope(AllocatorState* State) {
	JsObject* Result = AllocateObject(State, 20); 

	return Result;
}







struct ScopeTraveralState {
	char* Position;
	
};



enum ScopeType {
	ScopeStart, Scopelet, ScopeVar,ScopeFunctionWall,ScopeRedirect
};

enum TriState {
	TriStateWorking, TriStateTrue, TriStateFalse
};

struct ScopeObject
{
	char String;
	char Lenght;
	ScopeType stype;
};






TriState CheckScope(char* Target, int Length, ScopeTraveralState state) {

	int StringLenght = *state.Position;
	if (Length != StringLenght) {
		return TriStateFalse;
	}
	
}

int CheckWord(char* A,char* B,int Letter) {
	if (A[Letter] != B[Letter]) return TriStateFalse;
	if (A[Letter] == 0 && B[Letter] == 0) return TriStateTrue;
	if (A[Letter] != 0 || B[Letter] != 0) return TriStateFalse;
	return TriStateWorking;

}