#pragma once

#include "Expression.h"
#include <iostream>
#include "AST.h"
int MathmaticalValueDebug(ASTSymbol symbol);
void DisplayVarable(JsObject* Token);
void DisplaySymbol(ASTSymbol Token);
void DisplayRamStats();

extern int MemoryObjectsAllocated;
void ObjectAllocated(JsObject* object);