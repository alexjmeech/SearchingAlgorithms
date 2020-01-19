#include "Search.h"
#include "SearchingAlgorithmDispatcher.h"
#include "FileIOUtil.hpp"
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include <iostream>
#include <fstream>
#include <cstring>

void Search::Load(string directory)
{
	if (graphMatrix != nullptr)
	{
		delete graphMatrix;
	}

	if (graphList != nullptr)
	{
		delete graphList;
	}

	string* lines = nullptr;
	unsigned long total = 0;

	FileIOUtil<string>::readData(directory, "graph.txt", lines, total);

	graphMatrix = new AdjacencyMatrix(total);
	graphList = new AdjacencyList(total);

	for (unsigned long i = 0; i < total; i++)
	{
		string line = lines[i];

		if (line.empty())
		{
			continue;
		}

		const char* cs = line.c_str();

		unsigned long node = 0;
		unsigned long lastComma = 0;

		unsigned long comma;

		while ((comma = line.find_first_of(',', lastComma)) != string::npos)
		{
			if (lastComma == 0)
			{
				node = strtoul(cs + lastComma, NULL, 10) - 1;
			}
			else
			{
				unsigned long target = strtoul(cs + lastComma, NULL, 10) - 1;
				graphMatrix->connect(node, target);
				graphList->connect(node, target);
			}

			lastComma = comma + 1;
		}

		{
			unsigned long target = strtoul(cs + lastComma, NULL, 10) - 1;
			graphMatrix->connect(node, target);
			graphList->connect(node, target);
		}
	}

	delete[] lines;
	FileIOUtil<string>::readData(directory, "positions.txt", lines, total);

	for (unsigned long i = 0; i < total; i++)
	{
		string line = lines[i];

		if (line.empty())
		{
			continue;
		}

		const char* cs = line.c_str();

		unsigned long node = 0;
		double x = 0;
		double y = 0;
		double z = 0;
		unsigned long lastComma = 0;

		for (unsigned long j = 0; j < 3; j++)
		{
			unsigned long comma = line.find_first_of(',', lastComma);

			if (j == 0)
			{
				node = strtoul(cs + lastComma, NULL, 10) - 1;
			}
			else if (j == 1)
			{
				x = strtod(cs + lastComma, NULL);
			}
			else if (j == 2)
			{
				y = strtod(cs + lastComma, NULL);
			}

			lastComma = comma + 1;
		}

		z = strtod(cs + lastComma, NULL);

		graphMatrix->node(node, Point3D(x, y, z));
		graphList->node(node, Point3D(x, y, z));
	}

	delete[] lines;
	FileIOUtil<string>::readData(directory, "weights.txt", lines, total);

	for (unsigned long i = 0; i < total; i++)
	{
		string line = lines[i];

		if (line.empty() || line.at(0) < 0)
		{
			continue;
		}

		const char* cs = line.c_str();

		unsigned long from;
		unsigned long to;
		double cost;

		unsigned long lastComma = 0;
		unsigned long comma = line.find_first_of(',', lastComma);

		from = strtoul(cs + lastComma, NULL, 10) - 1;
		lastComma = comma + 1;

		comma = line.find_first_of(',', lastComma);

		to = strtoul(cs + lastComma, NULL, 10) - 1;
		lastComma = comma + 1;

		cost = strtod(cs + lastComma, NULL);

		graphMatrix->setCost(from, to, cost);
		graphList->setCost(from, to, cost);
	}

	delete[] lines;
}

void Search::Execute()
{
	Execute(rand() % graphMatrix->totalNodes(), rand() % graphMatrix->totalNodes());
}

void Search::Execute(unsigned long start, unsigned long end)
{
	if (this->algorithm == nullptr)
	{
		return;
	}

	matrixSummaries.push_back((*algorithm)(graphMatrix, start, end));
	listSummaries.push_back((*algorithm)(graphList, start, end));
}

void Search::Display() {}

void Search::Stats()
{
	OutputSummary(cout, matrixSummaries.peek_back(), "Adjacency Matrix");
	OutputSummary(cout, listSummaries.peek_back(), "Adjacency List");
	cout.flush();
}

void Search::Select(int algorithm)
{
	if (algorithm < 0 || algorithm >= SearchingAlgorithmType::END_SEARCHES)
	{
		return;
	}

	matrixSummaries.clear();
	listSummaries.clear();
	this->algorithm = SearchingAlgorithmDispatcher::getAlgorithm((SearchingAlgorithmType)algorithm);
}

void Search::Save()
{
	FileIOUtil<string>::ensureDirectory("output");
	ofstream outputMatrix("output/Matrix " + matrixSummaries.peek_back().getAlgorithm() + ".txt");

	outputMatrix << "Averaged Data:" << '\n' << '\n';

	{
		unsigned long total = 0;
		unsigned long avgPath = 0;
		unsigned long avgExplored = 0;
		long avgTime = 0;
		double avgDist = 0;
		double avgCost = 0;

		for (PathSummary& summary : matrixSummaries)
		{
			total++;
			avgPath += summary.getPath().size();
			avgExplored += summary.getTotalExplored();
			avgTime += summary.getExecutionTime();
			avgDist += summary.getPathDistance();
			avgCost += summary.getPathCost();
		}

		avgPath /= total;
		avgExplored /= total;
		avgTime /= total;
		avgDist /= total;
		avgCost /= total;

		outputMatrix << "Average Path Length: " << avgPath << '\n';
		outputMatrix << "Average Nodes Explored: " << avgExplored << '\n';
		outputMatrix << "Average Execution Time: " << avgTime << '\n';
		outputMatrix << "Average Path Distance: " << avgDist << '\n';
		outputMatrix << "Average Path Cost: " << avgCost << '\n' << '\n';
	}

	outputMatrix << "Raw Data:" << '\n' << '\n';

	for (PathSummary& summary : matrixSummaries)
	{
		OutputSummary(outputMatrix, summary, "Adjacency Matrix");
	}

	outputMatrix.close();

	ofstream outputList("output/List " + listSummaries.peek_back().getAlgorithm() + ".txt");

	outputList << "Averaged Data:" << '\n' << '\n';

	{
		unsigned long total = 0;
		unsigned long avgPath = 0;
		unsigned long avgExplored = 0;
		long avgTime = 0;
		double avgDist = 0;
		double avgCost = 0;

		for (PathSummary& summary : listSummaries)
		{
			total++;
			avgPath += summary.getPath().size();
			avgExplored += summary.getTotalExplored();
			avgTime += summary.getExecutionTime();
			avgDist += summary.getPathDistance();
			avgCost += summary.getPathCost();
		}

		avgPath /= total;
		avgExplored /= total;
		avgTime /= total;
		avgDist /= total;
		avgCost /= total;

		outputList << "Average Path Length: " << avgPath << '\n';
		outputList << "Average Nodes Explored: " << avgExplored << '\n';
		outputList << "Average Execution Time: " << avgTime << '\n';
		outputList << "Average Path Distance: " << avgDist << '\n';
		outputList << "Average Path Cost: " << avgCost << '\n' << '\n';
	}

	outputList << "Raw Data:" << '\n' << '\n';

	for (PathSummary& summary : listSummaries)
	{
		OutputSummary(outputList, summary, "Adjacency List");
	}

	outputList.close();
}

void Search::Configure() {}

unsigned long Search::LoadedNodes()
{
	if (graphMatrix == nullptr)
	{
		return 0;
	}

	return graphMatrix->totalNodes();
}

void Search::OutputSummary(ostream& output, PathSummary& summary, string graphType)
{
	output << "Graph Type:" << '\n';
	output << graphType << '\n' << '\n';

	output << "Algorithm:" << '\n';
	output << summary.getAlgorithm() << '\n' << '\n';

	output << "Path:" << '\n';
	LinkedList<unsigned long> listPath = summary.getPath();
	output << (listPath.pop_front() + 1);

	for (unsigned long node : listPath)
	{
		output << " -> " << (node + 1);
	}

	output << '\n' << '\n';

	output << "Nodes in Path:" << '\n';
	output << summary.getPath().size() << '\n' << '\n';

	output << "Cost of Path:" << '\n';
	output << summary.getPathCost() << '\n' << '\n';

	output << "Distance of Path:" << '\n';
	output << summary.getPathDistance() << '\n' << '\n';

	output << "Nodes Explored:" << '\n';
	output << summary.getTotalExplored() << '\n' << '\n';

	output << "Execution Time:" << '\n';
	output << summary.getExecutionTime() << " nanoseconds" << '\n' << '\n' << '\n';
}

Search::~Search()
{
	if (graphMatrix != nullptr)
	{
		delete graphMatrix;
	}

	if (graphList != nullptr)
	{
		delete graphList;
	}
}
