#include "AdjacencyList.h"

AdjacencyList::AdjacencyList(unsigned long nodes)
{
	this->nodes = nodes;
	graph = new LinkedList<GraphEdge>[nodes];
	points = new Point3D[nodes];
}

AdjacencyList::AdjacencyList(const AdjacencyList& copy)
{
	nodes = copy.nodes;
	graph = new LinkedList<GraphEdge>[nodes];
	points = new Point3D[nodes];

	for (unsigned long i = 0; i < nodes; i++)
	{
		graph[i] = copy.graph[i];
	}

	for (unsigned long i = 0; i < nodes; i++)
	{
		points[i] = copy.points[i];
	}
}

AdjacencyList& AdjacencyList::operator=(const AdjacencyList& copy)
{
	if (&copy != this)
	{
		delete[] points;
		delete[] graph;

		nodes = copy.nodes;
		graph = new LinkedList<GraphEdge>[nodes];
		points = new Point3D[nodes];

		for (unsigned long i = 0; i < nodes; i++)
		{
			graph[i] = copy.graph[i];
		}

		for (unsigned long i = 0; i < nodes; i++)
		{
			points[i] = copy.points[i];
		}
	}

	return *this;
}

AdjacencyList::~AdjacencyList()
{
	delete[] points;
	delete[] graph;
}

unsigned long AdjacencyList::totalNodes()
{
	return nodes;
}

void AdjacencyList::node(unsigned long nodeId, Point3D nodePoint)
{
	points[nodeId] = nodePoint;
}

void AdjacencyList::connect(unsigned long node1, unsigned long node2)
{
	graph[node1].push_back(GraphEdge(node1, node2, 0));
}

LinkedList<GraphEdge> AdjacencyList::connections(unsigned long nodeId)
{
	return graph[nodeId];
}

bool AdjacencyList::connected(unsigned long node1, unsigned long node2)
{
	for (GraphEdge& edge : graph[node1])
	{
		if (edge.getTo() == node2)
		{
			return true;
		}
	}

	return false;
}

Point3D AdjacencyList::point(unsigned long nodeId)
{
	return points[nodeId];
}

GraphEdge AdjacencyList::between(unsigned long node1, unsigned long node2)
{
	for (GraphEdge& edge : graph[node1])
	{
		if (edge.getTo() == node2)
		{
			return edge;
		}
	}

	return GraphEdge(node1, node2);
}

void AdjacencyList::setCost(unsigned long node1, unsigned long node2, double cost)
{
	for (GraphEdge& edge : graph[node1])
	{
		if (edge.getTo() == node2)
		{
			edge.setCost(cost);
			return;
		}
	}
}
