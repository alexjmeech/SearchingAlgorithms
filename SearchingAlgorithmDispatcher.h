#ifndef SEARCHINGALGORITHMDISPATCHER_H_
#define SEARCHINGALGORITHMDISPATCHER_H_

#include <string>
#include <unordered_map>
#include "PathSummary.h"
#include "AdjacencyStructure.h"
using namespace std;

enum SearchingAlgorithmType
{
	BFS_RECURSIVE,
	BFS_ITERATIVE,
	DFS_RECURSIVE,
	DFS_ITERATIVE,
	A_STAR,
	DIJKSTRA,
	END_SEARCHES
};

typedef PathSummary(*AlgorithmFunction)(AdjacencyStructure*, unsigned long, unsigned long);

typedef void(LinkedList<unsigned long>::*PushFunction)(unsigned long);
typedef unsigned long(LinkedList<unsigned long>::*PopFunction)();

typedef double(*WeightFunction)(AdjacencyStructure*, unordered_map<unsigned long, double>&, unsigned long, unsigned long, unsigned long);

class PriorityWeightComparator
{
private:
	AdjacencyStructure* graph;
	WeightFunction calc;
	unsigned long end;
	unordered_map<unsigned long, double>* cumulative;
public:
	PriorityWeightComparator();
	PriorityWeightComparator(AdjacencyStructure*, WeightFunction, unsigned long, unordered_map<unsigned long, double>&);

	bool operator()(const GraphEdge&, const GraphEdge&) const;
};

class SearchingAlgorithmDispatcher
{
private:
	static double DijkstraWeight(AdjacencyStructure*, unordered_map<unsigned long, double>&, unsigned long, unsigned long, unsigned long);
	static double AStarWeight(AdjacencyStructure*, unordered_map<unsigned long, double>&, unsigned long, unsigned long, unsigned long);

	static unsigned long ProgressXFS(AdjacencyStructure*, unsigned long, unordered_map<unsigned long, unsigned long>&, LinkedList<unsigned long>&, PushFunction, PopFunction);
	static unordered_map<unsigned long, unsigned long> BestPath(AdjacencyStructure*, unsigned long, unsigned long, WeightFunction);

	static void DoRecursiveXFS(AdjacencyStructure*, unsigned long, unordered_map<unsigned long, unsigned long>&, LinkedList<unsigned long>, PushFunction, PopFunction);

	static PathSummary RecursiveBFS(AdjacencyStructure*, unsigned long, unsigned long);
	static PathSummary IterativeBFS(AdjacencyStructure*, unsigned long, unsigned long);
	static PathSummary RecursiveDFS(AdjacencyStructure*, unsigned long, unsigned long);
	static PathSummary IterativeDFS(AdjacencyStructure*, unsigned long, unsigned long);
	static PathSummary Dijkstra(AdjacencyStructure*, unsigned long, unsigned long);
	static PathSummary AStar(AdjacencyStructure*, unsigned long, unsigned long);
public:
	static AlgorithmFunction getAlgorithm(SearchingAlgorithmType);
};

#endif /* SEARCHINGALGORITHMDISPATCHER_H_ */
