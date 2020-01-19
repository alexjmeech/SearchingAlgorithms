#ifndef ADJACENCYMATRIX_H_
#define ADJACENCYMATRIX_H_

#include "AdjacencyStructure.h"

class AdjacencyMatrix: public AdjacencyStructure
{
private:
	GraphEdge** graph;
	unsigned long nodes;
	Point3D* points;
public:
	AdjacencyMatrix(unsigned long);
	AdjacencyMatrix(const AdjacencyMatrix&);
	AdjacencyMatrix& operator=(const AdjacencyMatrix&);
	virtual ~AdjacencyMatrix();

	virtual unsigned long totalNodes();

	virtual void node(unsigned long, Point3D);
	virtual void connect(unsigned long, unsigned long);
	virtual LinkedList<GraphEdge> connections(unsigned long);

	virtual bool connected(unsigned long, unsigned long);

	virtual Point3D point(unsigned long);
	virtual GraphEdge between(unsigned long, unsigned long);
	virtual void setCost(unsigned long, unsigned long, double);
};

#endif /* ADJACENCYMATRIX_H_ */
