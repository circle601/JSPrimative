#include "MemAllocator.h"
#include "JS.h"
#include "Debug.h"
//this has way way too many problems
int MemoryObjectsAllocated = 0;

//Buddy memory allocation
AllocatorState PrepareRegion(char* Region) {
	AllocatorState result;
	result.Region = Region;
	result.Last = 0;
	return result;
}


void* AllocateMem(AllocatorState* State, size_t sise) {
	//Should eventually use the Buddy memory allocation
	int Start = State->Last;
	State->Region[Start] = sise;
	State->Last += sise + 2;
	State->Inuse = false;
	return &State->Region[Start + 1];
}

void Freemem(void* Region, AllocatorState* State) {
	//Pretend too free memory but its not supported at the moment
}

int ObjectSise(int Length) {
	return sizeof(JsObject) + sizeof(JSObjectElement) * (Length - 1);
}


JsObject* AllocateObject(AllocatorState* State, size_t Length) {

	MemoryObjectsAllocated++;
	int Sise = ObjectSise(Length - 1);
	JsObject*  Result = (JsObject*)AllocateMem(State, Sise);
	Result->ElementCount = 0;
	Result->RefCount = 0;
	Result->BaseObject.Builtin = OObject;
	Result->MaxSise = Length;
	for (size_t i = 0; i < Length - 1; i++)
	{
		JSObjectElement* Element = &(&Result->Data)[i];
		Element->BaseObject.Builtin = OUndefined;
	}
	ObjectAllocated(Result);
	return Result;
}


void RequestArray(AllocatorState* State, size_t Length) {
	State->MemoryResultStore = 0;
	State->MemoryResultStore = (void*)AllocateObject(State, Length);
}


void RequestObject(AllocatorState* State, size_t Length) {
	State->MemoryResultStore = 0;
	State->MemoryResultStore = (void*)AllocateObject(State,Length);
}

void RequestMemory(AllocatorState* State, size_t Length) {
	State->MemoryResultStore = 0;
	State->MemoryResultStore = AllocateMem(State, Length);
}

void* MemoryResult(AllocatorState* State) {
	return State->MemoryResultStore;
}


bool MemoryInUse(AllocatorState* State) {
	return State->Inuse;
}

