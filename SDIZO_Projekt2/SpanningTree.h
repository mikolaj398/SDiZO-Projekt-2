#pragma once
#include "stdafx.h"
#include "Edge.h"
#include "VertexList.h"

class SpanningTree
{
	VertexList ** T;
	Edge *e;
	int sizeT, weight, edges, index;

public:
	SpanningTree();
	SpanningTree(int verticesT, int edgeT);
	~SpanningTree();
	void AddEdge(Edge E);
	void show();
};

