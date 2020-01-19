#ifndef ADJACENCYSTRUCTURE_H_
#define ADJACENCYSTRUCTURE_H_

#include "Point3D.h"
#include "LinkedList.h"
#include "GraphEdge.h"

class AdjacencyStructure
{
public:
	virtual ~AdjacencyStructure() {};

	virtual unsigned long totalNodes() = 0;

	virtual void node(unsigned long, Point3D) = 0;
	virtual void connect(unsigned long, unsigned long) = 0;
	virtual LinkedList<GraphEdge> connections(unsigned long) = 0;

	virtual bool connected(unsigned long, unsigned long) = 0;

	virtual Point3D point(unsigned long) = 0;
	virtual GraphEdge between(unsigned long, unsigned long) = 0;
	virtual void setCost(unsigned long, unsigned long, double) = 0;
};

#endif /* ADJACENCYSTRUCTURE_H_ */
