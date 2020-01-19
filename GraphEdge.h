#ifndef GRAPHEDGE_H_
#define GRAPHEDGE_H_

class GraphEdge
{
private:
	unsigned long to;
	unsigned long from;
	double cost;
public:
	GraphEdge(unsigned long = 0, unsigned long = 0, double = -1);

	unsigned long getFrom() const;
	unsigned long getTo() const;
	double getCost() const;

	void setCost(double);
};

#endif /* GRAPHEDGE_H_ */
