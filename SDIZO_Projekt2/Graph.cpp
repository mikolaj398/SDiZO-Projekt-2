#include "stdafx.h"
#include "Graph.h"
#include "VertexList.h"
#include "Stack.h"
#include "SpanningTree.h"
#include <list>
#include <vector>
using namespace std;
Graph::Graph(int v, int e)
{
	vertices = v;
	edges = e;
	incidentMatrix = new int *[v];
	NDGraphMatrix = new int *[v];
	NHoodList = new VertexList *[v];
	NDGraphList = new VertexList *[v];
	E = new Edge[e];
	EO = new Edge[e];
	reversedEdges = 0;
}


Graph::Graph(int v, int e, int v_f, int v_l)
{
	vertices = v;
	edges = e;
	vFirst = v_f;
	vLast = v_l;
	incidentMatrix = new int *[v];
	NDGraphMatrix = new int *[v];
	NHoodList = new VertexList *[v];
	NDGraphList = new VertexList *[v];
	E = new Edge[e];
	EO = new Edge[e];
	reversedEdges = 0;
}

Graph::~Graph()
{
	for (int i = 0; i < vertices; i++) {
		v1 = NHoodList[i];
		while (v1) {
			v2 = v1;
			v1 = v1->next;
			delete v2;
		}
	}
	delete[] NHoodList;

	for (int i = 0; i < vertices; i++)
		delete[] incidentMatrix[i];
	delete[] incidentMatrix;

	//Ogarnąć czy nie trzeba pousuwać reszty
}

//zwracanie wartości prywatnych
int Graph::GetVertices() {
	return vertices;
}

int Graph::GetEdges() {
	return edges;
}

int Graph::GetFirstVertex() {
	return vFirst;
}

int Graph::GetLastVertex() {
	return vLast;
}

void Graph::NotDirectedGraph() { //graf nieskierowany
	//int j;
	//tworzenie macierzy
	for (int i = 0; i < vertices; i++)
		NDGraphMatrix[i] = new int[edges];

	for (int i = 0; i < vertices; i++)
		for (int j = 0; j < edges; j++)
			NDGraphMatrix[i][j] = 0;

	for (int i = 0; i < vertices; i++)
		NDGraphList[i] = NULL;

	for (int i = 0; i < edges; i++) {
		int j = 0;
		while (j < this->reversedEdges) {
			if (EO[j].vf == E[i].vf && EO[j].vl == E[i].vl)
				break;
			else j++;
		}
		if (j == reversedEdges) {
			EO[reversedEdges].vf = E[i].vl;
			EO[reversedEdges].vl = E[i].vf;
			EO[reversedEdges].weight = E[i].weight;
			reversedEdges++;
		}
	}

	int vf, vl, weight;
	for (int i = 0; i < reversedEdges; i++) {
		vf = EO[i].vf;
		vl = EO[i].vl;
		weight = EO[i].weight;

		v1 = new VertexList;
		v1->v = vl;
		v1->weight = weight;
		v1->next = NDGraphList[vf];
		NDGraphList[vf] = v1;

		v2 = new VertexList;
		vf = EO[i].vl;
		vl = EO[i].vf;
		weight = EO[i].weight;

		v2->v = vl;
		v2->weight = weight;
		v2->next = NDGraphList[vf];
		NDGraphList[vf] = v2;

		NDGraphMatrix[EO[i].vf][i] = 1;
		NDGraphMatrix[EO[i].vl][i] = 1;
	}
}


void Graph::RandomEdges() { //losowanie krawędzi

	int c = 0, calcST = 0, a = this->vertices, b, toAdd1, toAdd2, i, *inTree, *notInTree;

	inTree = new int[a];
	notInTree = new int[a];
	for (int i = 0; i < a; i++)
		notInTree[i] = i;

	b = rand() % a;
	toAdd1 = notInTree[b];
	notInTree[b] = notInTree[a - 1];
	a--;
	inTree[c] = toAdd1;
	c++;

	for (int i = 0; i < this->vertices - 1; i++) {
		b = rand() % c;
		toAdd1 = inTree[b];

		b = rand() % a;
		toAdd2 = notInTree[b];
		notInTree[b] = notInTree[a - 1];
		a--;
		inTree[c] = toAdd2;
		c++;
		E[i].vf = toAdd1;
		E[i].vl = toAdd2;
	}

	for (int i = 0; i < (vertices - 1); i++) {
		b = rand() % vertices;
		Swap(E[b].vl, E[b].vf);
	}

	for (int i = vertices - 1; i < edges; i++) {

		a = vertices;
		int *T = new int[a];

		for (int k = 0; k < a; k++)
			T[k] = k;

		b = rand() % a;
		toAdd1 = T[b];
		T[b] = T[a - 1];
		a--;

		while (true) {
			calcST = 0;
			for (int j = 0; j < i; j++) {
				if (E[j].vf == toAdd1)
					calcST++;
				if (calcST == vertices - 1)
					break;
			}

			if (calcST == vertices - 1) {
				b = rand() % a;
				toAdd1 = T[b];
				T[b] = T[a - 1];
				a--;
			}
			else break;
		}

		a = vertices;
		for (int k = 0; k < a; k++)
			T[k] = k;

		T[toAdd1] = T[a - 1];
		a--;

		b = rand() % a;
		toAdd2 = T[b];
		T[b] = T[a - 1];
		a--;

		for (int j = 0; j < i; j++) {
			while (toAdd1 == E[j].vf && toAdd2 == E[j].vl) {
				b = rand() % a;
				toAdd2 = T[b];
				T[b] = T[a - 1];
				a--;
				j = 0;
			}
		}
		E[i].vf = toAdd1;
		E[i].vl = toAdd2;
		delete[]T;
	}

	for (i = 0; i < this->edges; i++)
		E[i].weight = (rand() % 9) + 1;

	delete[] notInTree;
	delete[] inTree;
}


void Graph::RandomGraph() {

	for (int i = 0; i < vertices; i++)
		incidentMatrix[i] = new int[edges];
	for (int i = 0; i < vertices; i++)
		for (int j = 0; j < edges; j++)
			incidentMatrix[i][j] = 0;

	for (int i = 0; i < vertices; i++)
		NHoodList[i] = NULL;

	RandomEdges();
	NotDirectedGraph();

	for (int i = 0; i < edges; i++) {
		int vf = E[i].vf;
		int vl = E[i].vl;
		int weight = E[i].weight;
		v1 = new VertexList;
		v1->v = vl;
		v1->weight = weight;
		v1->next = NHoodList[vf];
		NHoodList[vf] = v1;

		incidentMatrix[vf][i] = 1;
		incidentMatrix[vl][i] = -1;
	}
}

bool Graph::isConsist() // sprawdzanie czy graf jest spójny
{
	Stack st;
	int w, u, count;
	count = 0;
	bool *visited = new bool[vertices];
	for (int i = 0; i < vertices; i++)
		visited[i] = false;

	st.Push(0);
	visited[0] = true;

	while (!st.Empty()) {
		w = st.Top();
		st.Pop();
		count++;
		for (v1 = NDGraphList[w]; v1; v1 = v1->next) {
			u = v1->v;
			if (!visited[u]) {
				visited[u] = true;
				st.Push(u);
			}
		}
	}
	delete[] visited;
	if (count == vertices)
		return true;
	else return false;
}


void Graph::Show() { //wyświetlenie grafu macierzowo i listowo

	std::cout << std::endl << "Liczba wierzcholkow:" <<vertices << std::endl;
	std::cout << "Liczba krawedzi:" << edges << std::endl;

	std::cout << std::endl << "Macierz Incydencji:" << std::endl << "     ";
	for (int i = 0; i < edges; i++)
		std::cout << std::setw(3) << i;

	std::cout << std::endl << std::endl;
	for (int i = 0; i < vertices; i++)
	{
		std::cout << std::setw(3) << i << "  ";
		for (int j = 0; j < edges; j++)
			std::cout << std::setw(3) << incidentMatrix[i][j];
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "wagi:";
	for (int i = 0; i < edges; i++)
		std::cout << std::setw(3) << E[i].weight;

	std::cout << std::endl << std::endl << std::endl << "Lista Sasiedztwa:" << std::endl;

	std::cout << std::endl;
	for (int i = 0; i < vertices; i++)
	{
		std::cout << "LS[" << i << "]=";
		v1 = NHoodList[i];
		while (v1)
		{
			std::cout << "\t" << v1->v << "(" << v1->weight << ") ";
			v1 = v1->next;
		}
		std::cout << std::endl;
	}
}


void Graph::ShowND() { //wyświetlenie grafu nieskierowanego macierzowo i listowo

	std::cout << std::endl << "Liczba wierzcholkow:" << vertices << std::endl;
	std::cout << "Liczba krawedzi:" << edges << std::endl;

	std::cout << std::endl << "Macierz Incydencji (nieskierowana):" << std::endl << "    ";

	for (int i = 0; i < reversedEdges; i++)
	{
		std::cout << std::setw(3) << i;

	}
	std::cout << std::endl << std::endl;
	for (int i = 0; i < vertices; i++)
	{
		std::cout << std::setw(4) << i;
		for (int j = 0; j < reversedEdges; j++)
			std::cout << std::setw(3) << NDGraphMatrix[i][j];
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "wagi";
	for (int i = 0; i < reversedEdges; i++)
	{
		std::cout << std::setw(3) << EO[i].weight;
	}
	std::cout << std::endl << std::endl << std::endl << "Lista Sasiedztwa (nieskierowana):" << std::endl;

	for (int i = 0; i < vertices; i++)
	{
		std::cout << "LS[" << i << "] =";
		v1 = NDGraphList[i];
		while (v1)
		{
			std::cout << std::setw(3) << v1->v << "(" << v1->weight << ") ";
			v1 = v1->next;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	////////////////////////////////////
	
}


void Graph::Swap(int & p1, int & p2) { //zamiana dwoch liczb - potrzebna przy sortowaniu kopca

	int temp = p1;
	p1 = p2;
	p2 = temp;
}


void  Graph::DijkstraList(int w) { //algorytm dijkstry dla listy sasiedztwa // w - wierzcholek poczatkowy
	int root, node, heapSize, parent, lChild, rChild, minChild, minCost, *costs, *prev, *heap, *posHeap;
	bool *visited;
	Stack stack;

	costs = new int[vertices];
	prev = new int[vertices];
	heap = new int[vertices];
	posHeap = new int[vertices];
	visited = new bool[vertices];

	for (int i = 0; i < vertices; i++) {
		costs[i] = MAXINT;
		prev[i] = -1;
		visited[i] = false;
		heap[i] = i;
		posHeap[i] = i;
	}
	heapSize = vertices;
	//costs[0] = 0;
	costs[w] = 0;
	node = heap[0];
	heap[0] = heap[w];
	heap[w] = node;
	posHeap[w] = 0;
	posHeap[0] = w;

	// time start
	std::chrono::time_point<std::chrono::system_clock> start, end; //pomiar czasu
	start = std::chrono::system_clock::now();

	for (int i = 0; i < vertices; i++) { //dla kazdego wierzcholka
		root = heap[0];
		heap[0] = heap[--heapSize];
		posHeap[heap[0]] = 0;
		parent = 0;
		while (true) { //ustalenie najlepszego potomka dla danego wierzcholka w zaleznosci od kosztu (struktura kopca)
			lChild = parent + parent + 1;
			rChild = lChild + 1;
			if (lChild >= heapSize)
				break;
			minCost = costs[heap[lChild]];
			minChild = lChild;
			if ((rChild < heapSize) && (minCost > costs[heap[rChild]])) {
				minCost = costs[heap[rChild]];
				minChild = rChild;
			}
			if (costs[heap[parent]] <= minCost)
				break;
			node = heap[parent];
			heap[parent] = heap[minChild];
			heap[minChild] = node;
			posHeap[heap[parent]] = parent;
			posHeap[heap[minChild]] = minChild;
			parent = minChild;
		}

		visited[root] = true;
		if (NHoodList[root] != NULL)
			for (v1 = NHoodList[root]; v1; v1 = v1->next)
				if (!visited[v1->v] && (costs[v1->v] > costs[root] + v1->weight)) {
					costs[v1->v] = costs[root] + v1->weight;
					prev[v1->v] = root;
					for (int child = posHeap[v1->v]; child; child = parent) { //petla po dzieciach
						parent = child / 2;
						if (costs[heap[parent]] <= costs[heap[child]])
							break;
						node = heap[parent];
						heap[parent] = heap[child];
						heap[child] = node;
						posHeap[heap[parent]] = parent;
						posHeap[heap[child]] = child;
					}
				}
	}
	// mierzenie czasu koniec
	end = std::chrono::system_clock::now(); //koniec pomiaru
	std::chrono::duration<double> elapsed_seconds = end - start;

	//wypisywanie na ekran
	std::cout << "\n\nAlgorytm Dijkstry - Lista\n ";
	std::cout << "\nWierzcholek startowy: " << w << std::endl << std::endl;
	std::cout << "w. koncowy | koszt cal. | drogra\n";
	for (int i = 0; i < vertices; i++) {
		int width = -2;

		std::cout << std::setw(6) << i << "     | ";


		if (costs[i] == MAXINT)// || koszta[i]<0)
			std::cout << "Brak sciezki" << std::endl;
		else {
			std::cout << std::setw(6) << costs[i] << "     |   ";
			for (int j = i; j > -1; j = prev[j]) {
				stack.Push(j);
				width = width + 2;
			}
			bool was = false;
			while (!stack.Empty()) {
				if (!was){
					std::cout << stack.Top();
					was = true;
				}
				else
					std::cout << " -> " << stack.Top();
				stack.Pop();
			}
			std::cout << std::endl;
		}
	}

	std::cout << "\nCzas wykonywania: " << elapsed_seconds.count() << " [s]\n";
	std::cout << std::endl << std::endl;
	// czyszczenie
	delete visited;
	delete posHeap;
	delete heap;
	delete prev;
	delete costs;
}


void  Graph::DijkstraMatrix(int w) {
	int root, node, heapSize, parent, lChild, rChild, minChild, minCost, *costs, *prev, *heap, *posHeap;
	bool *visited;
	Stack stack;

	costs = new int[vertices];
	prev = new int[vertices];
	heap = new int[vertices];
	posHeap = new int[vertices];
	visited = new bool[vertices];

	for (int i = 0; i < vertices; i++) {
		costs[i] = MAXINT;
		prev[i] = -1;
		visited[i] = false;
		heap[i] = i;
		posHeap[i] = i;
	}
	heapSize = vertices;
	costs[w] = 0;
	node = heap[0];
	heap[0] = heap[w];
	heap[w] = node;
	posHeap[w] = 0;
	posHeap[0] = w;

	std::chrono::time_point<std::chrono::system_clock> start, end; //pomiar czasu
	start = std::chrono::system_clock::now();

	for (int i = 0; i < vertices; i++) {
		root = heap[0];
		heap[0] = heap[--heapSize];
		posHeap[heap[0]] = 0;
		parent = 0;
		while (true) {
			lChild = parent + parent + 1;
			rChild = lChild + 1;
			if (lChild >= heapSize)
				break;
			minCost = costs[heap[lChild]];
			minChild = lChild;
			if ((rChild < heapSize) && (minCost > costs[heap[rChild]])) {
				minCost = costs[heap[rChild]];
				minChild = rChild;
			}
			if (costs[heap[parent]] <= minCost)
				break;
			node = heap[parent];
			heap[parent] = heap[minChild];
			heap[minChild] = node;
			posHeap[heap[parent]] = parent;
			posHeap[heap[minChild]] = minChild;
			parent = minChild;
		}
		visited[root] = true;

		for (int j = 0; j < edges; j++) {
			if (incidentMatrix[root][j] != 0)
				for (int k = 0; k < vertices; k++)
					if (k != root && incidentMatrix[k][j] == -1 && !visited[k] && (costs[k] > costs[root] + E[j].weight)) {
						costs[k] = costs[root] + E[j].weight;
						prev[k] = root;
						for (int child = posHeap[k]; child; child = parent) {
							parent = child / 2;
							if (costs[heap[parent]] <= costs[heap[child]])
								break;
							node = heap[parent];
							heap[parent] = heap[child];
							heap[child] = node;
							posHeap[heap[parent]] = parent;
							posHeap[heap[child]] = child;
						}
					}
		}
	}
	//stop czas
	end = std::chrono::system_clock::now(); //koniec pomiaru
	std::chrono::duration<double> elapsed_seconds = end - start;

	//wypisywanie na ekran
	std::cout << "\n\nAlgorytm Dijkstry - Macierz\n ";
	std::cout << "\nWierzcholek startowy: " << w << std::endl << std::endl;
	std::cout << "w. koncowy | koszt cal. | drogra\n";
	for (int i = 0; i < vertices; i++) {
		int width = -2;

		std::cout << std::setw(6) << i << "     | ";


		if (costs[i] == MAXINT)
			std::cout << "Brak sciezki" << std::endl;
		else {
			std::cout << std::setw(6) << costs[i] << "     |   ";
			for (int j = i; j > -1; j = prev[j]) {
				stack.Push(j);
				width = width + 2;
			}
			bool was = false;
			while (!stack.Empty()) {
				if (!was) {
					std::cout << stack.Top();
					was = true;
				}
				else
					std::cout << " -> " << stack.Top();
				stack.Pop();
			}
			std::cout << std::endl;
		}
	}
	std::cout << "\nCzas wykonywania: " << elapsed_seconds.count() << " [s]\n";
	std::cout << std::endl << std::endl;

	//czyszczenie
	delete visited;
	delete posHeap;
	delete heap;
	delete prev;
	delete costs;
}

void Graph::PrimList() {
	int w = 0;
	bool *visited = new bool[vertices];
	Edge e;
	std::priority_queue<Edge, std::vector<Edge>, Edge> queue;
	SpanningTree *tree = new SpanningTree(vertices, edges);
	for (int i = 0; i < vertices; i++)
		visited[i] = false;


	visited[w] = true;

	std::chrono::time_point<std::chrono::system_clock> start, end; //pomiar czasu
	start = std::chrono::system_clock::now();

	for (int i = 1; i < vertices; i++) {
		for (v1 = this->NDGraphList[w]; v1; v1 = v1->next) {
			if (!visited[v1->v]) {
				e.vf = w;
				e.vl = v1->v;
				e.weight = v1->weight;
				queue.push(e);
			}
		}
		do {
			e = queue.top();
			queue.pop();
		} while (visited[e.vl]);
		tree->AddEdge(e);
		visited[e.vl] = true;
		w = e.vl;
	}

	end = std::chrono::system_clock::now(); //koniec pomiaru
	std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "\nMinimalne drzewo rozpinajace:\n";
	tree->show();

	std::cout << "\nCzas wykonywania: " << elapsed_seconds.count() << " [s]\n";
	//
	delete visited;
	delete tree;
}

void Graph::PrimMatrix() {
	int w = 0;
	bool *visited = new bool[vertices];
	Edge e;
	std::priority_queue<Edge, std::vector<Edge>, Edge> queue;
	SpanningTree *tree = new SpanningTree(vertices, edges);
	for (int i = 0; i < vertices; i++)
		visited[i] = false;

	visited[0] = true;
	
	std::chrono::time_point<std::chrono::system_clock> start, end; //pomiar czasu
	start = std::chrono::system_clock::now();

	for (int i = 1; i < vertices; i++) {
		for (int g = 0; g < reversedEdges; g++) {
			if (NDGraphMatrix[w][g] != 0)
				for (int j = 0; j < vertices; j++)
					if (j != w && NDGraphMatrix[j][g] != 0 && !visited[j]) {
						e.vf = w;
						e.vl = j;
						e.weight = EO[g].weight;
						queue.push(e);
					}
		}
		do {
			e = queue.top();
			queue.pop();
		} while (visited[e.vl]);

		tree->AddEdge(e);
		visited[e.vl] = true;
		w = e.vl;
	}

	end = std::chrono::system_clock::now(); //koniec pomiaru
	std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "\nMinimalne drzewo rozpinajace:\n";
	tree->show();

	std::cout << "\nCzas wykonywania: " << elapsed_seconds.count() << " [s]\n";
	//
	delete tree;
	delete visited;
}


void Graph::bellman_fordM(int w) {
	int *dist= new int[vertices];             //tablica odległości między wierzchołkami
	int *trace= new int[vertices];            //dodatkowa tabela poprzednio odwiedzonych wierzchołków dla danego(do odtworzenia ścieżki, a nie tylko wag)

	for (int i = 0; i < vertices; i++) {
		dist[i] = 147483647;
		trace[i] = w;
	}

	dist[w] = 0;                      //wierzchołek startowy ma odległość 0

	for (int x = 1; x < vertices; x++) {    //i-punkt startowy(x), i2-punkt docelowy(y)     x-wysokość,rząd y-szerokość,kolumna
		//relaksacja
		for (int i = 0; i < vertices; i++) {
			for (int i2 = 0; i2 < vertices; i2++) {
				if (i2 == i || NDGraphMatrix[i][i2] == 147483647)           //omija siebie i brak krawędzi
					continue;
				if (dist[i2] > dist[i] + NDGraphMatrix[i][i2]) { //jeśli odległość od i2 jest większa niż suma odległości do i oraz między i i2, to
					dist[i2] = dist[i] + NDGraphMatrix[i][i2]; //zmień odległość i2 na właśnie tą sumę(czyli, że znaleziono do danego punktu krótszą drogę)
					trace[i2] = i;                    //no i poprzednik i2 to teraz i, bo to od niego tu przyszliśmy
				}
			}
		}
	}
	std::cout << "\n\nAlgorytm Dijkstry - Macierz\n ";
	std::cout << "\nWierzcholek startowy: " << w << std::endl << std::endl;
	std::cout << "w. koncowy | koszt cal. | drogra\n";
	int x = (int)147483647 / 2;

	if (vertices > 50) {                //alternatywny sposób wyświetlania dla większych grafów
		cout << endl;
		for (int i = 0; i < vertices; i++) {
			if (dist[i] > x)
				cout << i << "(-)[-] ";
			else
				cout << i << "(" << dist[i] << ")[" << trace[i] << "] ";
		}
		cout << endl;
		return;
	}

	cout << "         ";
	for (int i = 0; i < vertices; i++) {
		cout << i << " ";
	}
	cout << endl;
	cout << "dystans: ";
	for (int i = 0; i < vertices; i++) {
		if (dist[i] > x)
			cout << "- ";         //niech nie wyświetla, jeśli gdzieś nie da się dojść
		else
			cout << dist[i] << " ";
	}
	cout << endl;
	cout << "trace:   ";
	for (int i = 0; i < vertices; i++) {
		cout << trace[i] << " ";
	}
	cout << endl;
	return;
}
//Funkcje przygotowane pod testy czasowe


double  Graph::DijkstraListTest(int w) { //algorytm dijkstry dla listy sasiedztwa // w - wierzcholek poczatkowy
	int root, node, heapSize, parent, lChild, rChild, minChild, minCost, *costs, *prev, *heap, *posHeap;
	bool *visited;
	Stack stack;

	costs = new int[vertices];
	prev = new int[vertices];
	heap = new int[vertices];
	posHeap = new int[vertices];
	visited = new bool[vertices];

	for (int i = 0; i < vertices; i++) {
		costs[i] = MAXINT;
		prev[i] = -1;
		visited[i] = false;
		heap[i] = i;
		posHeap[i] = i;
	}
	heapSize = vertices;
	//costs[0] = 0;
	costs[w] = 0;
	node = heap[0];
	heap[0] = heap[w];
	heap[w] = node;
	posHeap[w] = 0;
	posHeap[0] = w;

	// time start
	std::chrono::time_point<std::chrono::system_clock> start, end; //pomiar czasu
	start = std::chrono::system_clock::now();

	for (int i = 0; i < vertices; i++) { //dla kazdego wierzcholka
		root = heap[0];
		heap[0] = heap[--heapSize];
		posHeap[heap[0]] = 0;
		parent = 0;
		while (true) { //ustalenie najlepszego potomka dla danego wierzcholka w zaleznosci od kosztu (struktura kopca)
			lChild = parent + parent + 1;
			rChild = lChild + 1;
			if (lChild >= heapSize)
				break;
			minCost = costs[heap[lChild]];
			minChild = lChild;
			if ((rChild < heapSize) && (minCost > costs[heap[rChild]])) {
				minCost = costs[heap[rChild]];
				minChild = rChild;
			}
			if (costs[heap[parent]] <= minCost)
				break;
			node = heap[parent];
			heap[parent] = heap[minChild];
			heap[minChild] = node;
			posHeap[heap[parent]] = parent;
			posHeap[heap[minChild]] = minChild;
			parent = minChild;
		}

		visited[root] = true;
		if (NHoodList[root] != NULL)
			for (v1 = NHoodList[root]; v1; v1 = v1->next)
				if (!visited[v1->v] && (costs[v1->v] > costs[root] + v1->weight)) {
					costs[v1->v] = costs[root] + v1->weight;
					prev[v1->v] = root;
					for (int child = posHeap[v1->v]; child; child = parent) { //petla po dzieciach
						parent = child / 2;
						if (costs[heap[parent]] <= costs[heap[child]])
							break;
						node = heap[parent];
						heap[parent] = heap[child];
						heap[child] = node;
						posHeap[heap[parent]] = parent;
						posHeap[heap[child]] = child;
					}
				}
	}
	// mierzenie czasu koniec
	end = std::chrono::system_clock::now(); //koniec pomiaru
	std::chrono::duration<double> elapsed_seconds = end - start;

	// czyszczenie
	delete visited;
	delete posHeap;
	delete heap;
	delete prev;
	delete costs;

	return elapsed_seconds.count();
}


double  Graph::DijkstraMatrixTest(int w) {
	int root, node, heapSize, parent, lChild, rChild, minChild, minCost, *costs, *prev, *heap, *posHeap;
	bool *visited;
	Stack stack;

	costs = new int[vertices];
	prev = new int[vertices];
	heap = new int[vertices];
	posHeap = new int[vertices];
	visited = new bool[vertices];

	for (int i = 0; i < vertices; i++) {
		costs[i] = MAXINT;
		prev[i] = -1;
		visited[i] = false;
		heap[i] = i;
		posHeap[i] = i;
	}
	heapSize = vertices;
	costs[w] = 0;
	node = heap[0];
	heap[0] = heap[w];
	heap[w] = node;
	posHeap[w] = 0;
	posHeap[0] = w;

	std::chrono::time_point<std::chrono::system_clock> start, end; //pomiar czasu
	start = std::chrono::system_clock::now();

	for (int i = 0; i < vertices; i++) {
		root = heap[0];
		heap[0] = heap[--heapSize];
		posHeap[heap[0]] = 0;
		parent = 0;
		while (true) {
			lChild = parent + parent + 1;
			rChild = lChild + 1;
			if (lChild >= heapSize)
				break;
			minCost = costs[heap[lChild]];
			minChild = lChild;
			if ((rChild < heapSize) && (minCost > costs[heap[rChild]])) {
				minCost = costs[heap[rChild]];
				minChild = rChild;
			}
			if (costs[heap[parent]] <= minCost)
				break;
			node = heap[parent];
			heap[parent] = heap[minChild];
			heap[minChild] = node;
			posHeap[heap[parent]] = parent;
			posHeap[heap[minChild]] = minChild;
			parent = minChild;
		}
		visited[root] = true;

		for (int j = 0; j < edges; j++) {
			if (incidentMatrix[root][j] != 0)
				for (int k = 0; k < vertices; k++)
					if (k != root && incidentMatrix[k][j] == -1 && !visited[k] && (costs[k] > costs[root] + E[j].weight)) {
						costs[k] = costs[root] + E[j].weight;
						prev[k] = root;
						for (int child = posHeap[k]; child; child = parent) {
							parent = child / 2;
							if (costs[heap[parent]] <= costs[heap[child]])
								break;
							node = heap[parent];
							heap[parent] = heap[child];
							heap[child] = node;
							posHeap[heap[parent]] = parent;
							posHeap[heap[child]] = child;
						}
					}
		}
	}
	//stop czas
	end = std::chrono::system_clock::now(); //koniec pomiaru
	std::chrono::duration<double> elapsed_seconds = end - start;

	//czyszczenie
	delete visited;
	delete posHeap;
	delete heap;
	delete prev;
	delete costs;

	return elapsed_seconds.count();
}

double Graph::PrimListTest() {
	int w = 0;
	bool *visited = new bool[vertices];
	Edge e;
	std::priority_queue<Edge, std::vector<Edge>, Edge> queue;
	SpanningTree *tree = new SpanningTree(vertices, edges);
	for (int i = 0; i < vertices; i++)
		visited[i] = false;


	visited[w] = true;

	std::chrono::time_point<std::chrono::system_clock> start, end; //pomiar czasu
	start = std::chrono::system_clock::now();

	for (int i = 1; i < vertices; i++) {
		for (v1 = this->NDGraphList[w]; v1; v1 = v1->next) {
			if (!visited[v1->v]) {
				e.vf = w;
				e.vl = v1->v;
				e.weight = v1->weight;
				queue.push(e);
			}
		}
		do {
			e = queue.top();
			queue.pop();
		} while (visited[e.vl]);
		tree->AddEdge(e);
		visited[e.vl] = true;
		w = e.vl;
	}
	end = std::chrono::system_clock::now(); //koniec pomiaru
	std::chrono::duration<double> elapsed_seconds = end - start;

	//
	delete visited;
	delete tree;

	return elapsed_seconds.count();
}

double Graph::PrimMatrixTest() {
	int w = 0;
	bool *visited = new bool[vertices];
	Edge e;
	std::priority_queue<Edge, std::vector<Edge>, Edge> queue;
	SpanningTree *tree = new SpanningTree(vertices, edges);
	for (int i = 0; i < vertices; i++)
		visited[i] = false;

	visited[0] = true;

	std::chrono::time_point<std::chrono::system_clock> start, end; //pomiar czasu
	start = std::chrono::system_clock::now();

	for (int i = 1; i < vertices; i++) {
		for (int g = 0; g < reversedEdges; g++) {
			if (NDGraphMatrix[w][g] != 0)
				for (int j = 0; j < vertices; j++)
					if (j != w && NDGraphMatrix[j][g] != 0 && !visited[j]) {
						e.vf = w;
						e.vl = j;
						e.weight = EO[g].weight;
						queue.push(e);
					}
		}
		do {
			e = queue.top();
			queue.pop();
		} while (visited[e.vl]);

		tree->AddEdge(e);
		visited[e.vl] = true;
		w = e.vl;
	}

	end = std::chrono::system_clock::now(); //koniec pomiaru
	std::chrono::duration<double> elapsed_seconds = end - start;
	//
	delete tree;
	delete visited;

	return elapsed_seconds.count();
}