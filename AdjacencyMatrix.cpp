#include "AdjacencyMatrix.h"

AdjacencyMatrix::AdjacencyMatrix(unsigned long nodes)
{
	this->nodes = nodes;
	graph = new GraphEdge*[nodes];

	for (unsigned long i = 0; i < nodes; i++)
	{
		graph[i] = new GraphEdge[nodes];

		for (unsigned long j = 0; j < nodes; j++)
		{
			graph[i][j] = GraphEdge(i, j);
		}
	}

	points = new Point3D[nodes];
}

AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix& copy)
{
	nodes = copy.nodes;
	graph = new GraphEdge*[nodes];
	points = new Point3D[nodes];

	for (unsigned long i = 0; i < nodes; i++)
	{
		graph[i] = new GraphEdge[nodes];

		for (unsigned long j = 0; j < nodes; j++)
		{
			graph[i][j] = copy.graph[i][j];
		}
	}

	for (unsigned long i = 0; i < nodes; i++)
	{
		points[i] = copy.points[i];
	}
}

AdjacencyMatrix& AdjacencyMatrix::operator=(const AdjacencyMatrix& copy)
{
	if (&copy != this)
	{
		delete[] points;

		for (unsigned long i = 0; i < nodes; i++)
		{
			delete[] graph[i];
		}

		delete[] graph;

		nodes = copy.nodes;
		graph = new GraphEdge*[nodes];
		points = new Point3D[nodes];

		for (unsigned long i = 0; i < nodes; i++)
		{
			graph[i] = new GraphEdge[nodes];

			for (unsigned long j = 0; j < nodes; j++)
			{
				graph[i][j] = copy.graph[i][j];
			}
		}

		for (unsigned long i = 0; i < nodes; i++)
		{
			points[i] = copy.points[i];
		}
	}

	return *this;
}

AdjacencyMatrix::~AdjacencyMatrix()
{
	delete[] points;

	for (unsigned long i = 0; i < nodes; i++)
	{
		delete[] graph[i];
	}

	delete[] graph;
}

unsigned long AdjacencyMatrix::totalNodes()
{
	return nodes;
}

void AdjacencyMatrix::node(unsigned long nodeId, Point3D nodePoint)
{
	points[nodeId] = nodePoint;
}

void AdjacencyMatrix::connect(unsigned long node1, unsigned long node2)
{
	graph[node1][node2] = GraphEdge(node1, node2, 0);
}

LinkedList<GraphEdge> AdjacencyMatrix::connections(unsigned long nodeId)
{
	GraphEdge* con = graph[nodeId];

	LinkedList<GraphEdge> data;

	for (unsigned long i = 0; i < nodes; i++)
	{
		if (con[i].getCost() >= 0)
		{
			data.push_back(con[i]);
		}
	}

	return data;
}

bool AdjacencyMatrix::connected(unsigned long node1, unsigned long node2)
{
	return graph[node1][node2].getCost() >= 0;
}

Point3D AdjacencyMatrix::point(unsigned long nodeId)
{
	return points[nodeId];
}

GraphEdge AdjacencyMatrix::between(unsigned long node1, unsigned long node2)
{
	return graph[node1][node2];
}

void AdjacencyMatrix::setCost(unsigned long node1, unsigned long node2, double cost)
{
	graph[node1][node2].setCost(cost);
}
