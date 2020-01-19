#include "GraphEdge.h"

GraphEdge::GraphEdge(unsigned long from, unsigned long to, double cost)
{
	this->from = from;
	this->to = to;
	this->cost = cost;
}

unsigned long GraphEdge::getFrom() const
{
	return from;
}

unsigned long GraphEdge::getTo() const
{
	return to;
}

double GraphEdge::getCost() const
{
	return cost;
}

void GraphEdge::setCost(double newCost)
{
	cost = newCost;
}
