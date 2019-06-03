#pragma once
#include "VertexList.h"
#include "Edge.h"

class Graph
{
	int vertices, edges, density, reversedEdges, vFirst, vLast;
	VertexList *v1, *v2;
		
public:
	Edge *E, *EO;
	VertexList **NHoodList, **NDGraphList;
	int **incidentMatrix, **NDGraphMatrix;

	Graph(int v, int e);
	Graph(int v, int e, int v_f, int v_l);
	~Graph();

	int GetVertices();
	int GetEdges();
	int GetFirstVertex();
	int GetLastVertex();
	void NotDirectedGraph();
	void RandomEdges();
	void RandomGraph();
	bool isConsist();
	void Show();
	void ShowND();
	void Swap(int & p1, int & p2);

	void DijkstraList(int w);
	void DijkstraMatrix(int w);
	void PrimList();
	void PrimMatrix();
	void bellman_fordM(int w);

	double DijkstraListTest(int w);
	double DijkstraMatrixTest(int w);
	double PrimListTest();
	double PrimMatrixTest();

};

