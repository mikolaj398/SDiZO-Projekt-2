#include "stdafx.h"
#include "SpanningTree.h"

SpanningTree::SpanningTree()
{
}

SpanningTree::SpanningTree(int verticeT, int edgeT)
{
	T = new VertexList *[verticeT];
	for (int i = 0; i < verticeT; i++) {
		T[i] = NULL;
	}
	sizeT = verticeT - 1;
	this->weight = 0;
	this->edges = edgeT;
	e = new Edge[edgeT];
	index = 0;
}

SpanningTree::~SpanningTree()
{
	VertexList *v1, *v2;
	for (int i = 0; i <= sizeT; i++) {
		v1 = T[i];
		while (v1)
		{
			v2 = v1;
			v1 = v1->next;
			delete v2;
		}
	}
	delete[] T;
}

void SpanningTree::AddEdge(Edge E) {

	VertexList *v;
	weight += E.weight;
	v = new VertexList;
	v->v = E.vl;
	v->weight = E.weight;
	v->next = T[E.vf];
	T[E.vf] = v;

	v = new VertexList;
	v->v = E.vf;
	v->weight = E.weight;
	v->next = T[E.vl];
	T[E.vl] = v;

	e[index] = E;
	index++;
}

void SpanningTree::show() {

	std::cout << std::endl;
	for (int i = 0; i < index; i++) {
		std::cout << "Krawedz " << std::setw(2) << i + 1 << ":";
		std::cout << "  [" << e[i].vf << "," << std::setw(2) << e[i].vl << "] waga:" << std::setw(2) << e[i].weight << std::endl;
	}
	std::cout << "Waga minimalnego drzewa rozpinajacego: " << weight << std::endl;
}