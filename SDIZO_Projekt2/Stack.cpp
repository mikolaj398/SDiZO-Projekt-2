#include "stdafx.h"
#include "Stack.h"



Stack::Stack() {
	s = NULL;
}

Stack::~Stack() {
	while (s) Pop();
}

bool Stack::Empty() {
	return !s;
}

int Stack::Top() {
	return s->v;
}

void Stack::Pop(){
	if (s) {
		VertexList *v = s;
		s = s->next;
		delete v;
	}
}

void Stack::Push(int temp) {
	VertexList * w = new VertexList;
	w->v = temp;
	w->next = s;
	s = w;
}