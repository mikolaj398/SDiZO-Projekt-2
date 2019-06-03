#pragma once
#include "VertexList.h"

//W�asna klasa stosu dzia�aj�ca z VertexList
class Stack
{
	VertexList * s;
public:
	Stack();
	~Stack();
	bool Empty();
	int Top();
	void Pop();
	void Push(int temp);
};

