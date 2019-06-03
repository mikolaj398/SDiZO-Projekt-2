#pragma once
class Edge
{
public:
	int vf, vl, weight; // kolejno wierzcholek poczatkowy, w. koncowy. waga

	Edge();
	~Edge();
	bool operator () (const Edge & e1, const Edge & e2);
};

