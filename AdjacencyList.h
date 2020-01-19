#ifndef ADJACENCYLIST_H_
#define ADJACENCYLIST_H_

#include "AdjacencyStructure.h"

class AdjacencyList: public AdjacencyStructure
{
private:
	LinkedList<GraphEdge>* graph;
	unsigned long nodes;
	Point3D* points;
public:
	AdjacencyList(unsigned long);
	AdjacencyList(const AdjacencyList&);
	AdjacencyList& operator=(const AdjacencyList&);
	virtual ~AdjacencyList();

	virtual unsigned long totalNodes();

	virtual void node(unsigned long, Point3D);
	virtual void connect(unsigned long, unsigned long);
	virtual LinkedList<GraphEdge> connections(unsigned long);

	virtual bool connected(unsigned long, unsigned long);

	virtual Point3D point(unsigned long);
	virtual GraphEdge between(unsigned long, unsigned long);
	virtual void setCost(unsigned long, unsigned long, double);
};

#endif /* ADJACENCYLIST_H_ */
