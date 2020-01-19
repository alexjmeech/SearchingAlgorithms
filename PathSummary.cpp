#include "PathSummary.h"

PathSummary::PathSummary()
{
	algorithmName = "Blank";
	distance = 0;
	cost = 0;
	explored = 0;
	execution = -1;
}

PathSummary::PathSummary(string algorithm)
{
	algorithmName = algorithm;
	distance = 0;
	cost = 0;
	explored = 0;
	execution = -1;
}

string PathSummary::getAlgorithm()
{
	return algorithmName;
}

LinkedList<unsigned long> PathSummary::getPath()
{
	return path;
}

double PathSummary::getPathDistance()
{
	return distance;
}

double PathSummary::getPathCost()
{
	return cost;
}

unsigned long PathSummary::getTotalExplored()
{
	return explored;
}

long PathSummary::getExecutionTime()
{
	return execution;
}

void PathSummary::addStep(unsigned long node, double stepCost, double stepDistance)
{
	path.push_back(node);
	cost += stepCost;
	distance += stepDistance;
}

void PathSummary::setExplored(unsigned long total)
{
	explored = total;
}

void PathSummary::setExecutionTime(long nanos)
{
	execution = nanos;
}
