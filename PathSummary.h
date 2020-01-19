#ifndef PATHSUMMARY_H_
#define PATHSUMMARY_H_

#include <string>
#include "LinkedList.h"
using namespace std;

class PathSummary
{
private:
	string algorithmName;
	LinkedList<unsigned long> path;
	double distance;
	double cost;
	unsigned long explored;
	long execution;
public:
	PathSummary();
	PathSummary(string);

	string getAlgorithm();

	LinkedList<unsigned long> getPath();

	double getPathDistance();

	double getPathCost();

	unsigned long getTotalExplored();

	long getExecutionTime();

	void addStep(unsigned long, double = 0, double = 0);
	void setExplored(unsigned long);
	void setExecutionTime(long);
};

#endif /* PATHSUMMARY_H_ */
