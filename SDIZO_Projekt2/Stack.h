#pragma once
#include "VertexList.h"

//W³asna klasa stosu dzia³aj¹ca z VertexList
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

