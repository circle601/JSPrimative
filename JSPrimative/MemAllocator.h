#pragma once
#include "JS.h"


struct AllocatorState
{
	char* Region;
	int Last;
	void* MemoryResultStore;
	bool Inuse;
};


//Buddy memory allocation
AllocatorState PrepareRegion(char* Region);
void* AllocateMem(AllocatorState* State, size_t sise);
void Freemem(void* Region, AllocatorState* State);
JsObject* AllocateObject(AllocatorState* State, size_t Length);

void RequestObject(AllocatorState* State, size_t Length);

void RequestMemory(AllocatorState* State, size_t Length);

void* MemoryResult(AllocatorState* State);


bool MemoryInUse(AllocatorState* State);