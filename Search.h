#ifndef SEARCH_H_
#define SEARCH_H_

#include "Algorithm.h"
#include "AdjacencyStructure.h"
#include "SearchingAlgorithmDispatcher.h"
#include <ostream>

class Search : public Algorithm
{
private:
	AdjacencyStructure* graphMatrix;
	AdjacencyStructure* graphList;

	LinkedList<PathSummary> matrixSummaries;
	LinkedList<PathSummary> listSummaries;

	PathSummary(*algorithm)(AdjacencyStructure*, unsigned long, unsigned long);

	virtual void OutputSummary(ostream&, PathSummary&, string);
public:
	virtual void Load(string);
	virtual void Execute();
	virtual void Execute(unsigned long, unsigned long);
	virtual void Display();
	virtual void Stats();
	virtual void Select(int);
	virtual void Save();
	virtual void Configure();

	virtual unsigned long LoadedNodes();

	virtual ~Search();
};

#endif /* SEARCH_H_ */
