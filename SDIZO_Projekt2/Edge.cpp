#include "Edge.h"

Edge::Edge()
{
}

Edge::~Edge()
{
}

bool Edge::operator()(const Edge & e1, const Edge & e2) {
	//porownywanie dwoch wag wierzcholkow
	if (e1.weight > e2.weight) return true;
	else return false;
}