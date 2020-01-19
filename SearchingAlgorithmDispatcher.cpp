#include "SearchingAlgorithmDispatcher.h"
#include <vector>
#include <queue>
#include <limits>
#include <chrono>

using namespace std;

PriorityWeightComparator::PriorityWeightComparator()
{
	graph = nullptr;
	calc = nullptr;
	end = 0;
	cumulative = nullptr;
}

PriorityWeightComparator::PriorityWeightComparator(AdjacencyStructure* backing, WeightFunction weightCalc, unsigned long target, unordered_map<unsigned long, double>& calculated)
{
	graph = backing;
	calc = weightCalc;
	end = target;
	cumulative = &calculated;
}

bool PriorityWeightComparator::operator()(const GraphEdge& e1, const GraphEdge& e2) const
{
	if (graph == nullptr)
	{
		return e1.getCost() > e2.getCost();
	}

	double w1 = (*calc)(graph, *cumulative, e1.getFrom(), e1.getTo(), end);
	double w2 = (*calc)(graph, *cumulative, e2.getFrom(), e2.getTo(), end);

	return w1 > w2;
}

double SearchingAlgorithmDispatcher::DijkstraWeight(AdjacencyStructure* graph, unordered_map<unsigned long, double>& cumulative, unsigned long from, unsigned long to, unsigned long end)
{
	if (graph->connected(from, to))
	{
		GraphEdge edge = graph->between(from, to);

		return cumulative.at(edge.getFrom()) + graph->point(edge.getFrom()).cost(graph->point(edge.getTo()));
	}

	return std::numeric_limits<double>::max();
}

double SearchingAlgorithmDispatcher::AStarWeight(AdjacencyStructure* graph, unordered_map<unsigned long, double>& cumulative, unsigned long from, unsigned long to, unsigned long end)
{
	if (graph->connected(from, to))
	{
		GraphEdge edge = graph->between(from, to);

		double base = graph->point(edge.getFrom()).cost(graph->point(edge.getTo())) * (1 + edge.getCost());

		base += graph->point(edge.getTo()).cost(graph->point(end)) * (1 + edge.getCost());

		return cumulative.at(edge.getFrom()) + base;
	}

	return std::numeric_limits<double>::max();
}

unsigned long SearchingAlgorithmDispatcher::ProgressXFS(AdjacencyStructure* graph, unsigned long end, unordered_map<unsigned long, unsigned long>& previous, LinkedList<unsigned long>& storage, PushFunction pushTo, PopFunction popFrom)
{
	unsigned long current = (storage.*(popFrom))();

	for (GraphEdge& edge : graph->connections(current))
	{
		if (previous.count(edge.getTo()) == 0)
		{
			(storage.*(pushTo))(edge.getTo());
			previous.insert({edge.getTo(), current});
		}
	}

	return current;
}

unordered_map<unsigned long, unsigned long> SearchingAlgorithmDispatcher::BestPath(AdjacencyStructure* graph, unsigned long start, unsigned long end, WeightFunction calculateWeight)
{
	unordered_map<unsigned long, unsigned long> previous;
	unordered_map<unsigned long, double> calculatedWeights;
	priority_queue<GraphEdge, vector<GraphEdge>, PriorityWeightComparator> select(PriorityWeightComparator(graph, calculateWeight, end, calculatedWeights));

	previous.insert({start, start});
	calculatedWeights.insert({start, 0});

	if (start == end)
	{
		return previous;
	}

	for (GraphEdge& edge : graph->connections(start))
	{
		previous.insert({edge.getTo(), start});
		calculatedWeights.insert({edge.getTo(), (*calculateWeight)(graph, calculatedWeights, start, edge.getTo(), end)});
		select.push(edge);
	}

	while (!select.empty())
	{
		GraphEdge curr = select.top();
		select.pop();

		if (curr.getTo() == end)
		{
			break;
		}

		for (GraphEdge& edge : graph->connections(curr.getTo()))
		{
			double calculated = (*calculateWeight)(graph, calculatedWeights, edge.getFrom(), edge.getTo(), end);

			if (previous.count(edge.getTo()) > 0)
			{
				if (previous.at(edge.getTo()) == edge.getFrom())
				{
					continue;
				}

				if (calculatedWeights.at(edge.getTo()) < calculated)
				{
					continue;
				}

				for (auto& p : previous)
				{
					if (p.second == edge.getTo())
					{
						calculatedWeights[p.first] -= calculatedWeights.at(edge.getTo());
						calculatedWeights[p.first] += calculated;
					}
				}

				previous[edge.getTo()] = edge.getFrom();
				calculatedWeights[edge.getTo()] = calculated;
				select.push(edge);
			}
			else
			{
				previous.insert({edge.getTo(), edge.getFrom()});
				calculatedWeights.insert({edge.getTo(), calculated});
				select.push(edge);
			}
		}
	}

	return previous;
}

void SearchingAlgorithmDispatcher::DoRecursiveXFS(AdjacencyStructure* graph, unsigned long target, unordered_map<unsigned long, unsigned long>& previous, LinkedList<unsigned long> storage, PushFunction pushFunction, PopFunction popFunction)
{
	if (storage.isEmpty())
	{
		return;
	}

	unsigned long n = ProgressXFS(graph, target, previous, storage, pushFunction, popFunction);

	if (n == target)
	{
		return;
	}

	DoRecursiveXFS(graph, target, previous, storage, pushFunction, popFunction);
}

PathSummary SearchingAlgorithmDispatcher::RecursiveBFS(AdjacencyStructure* graph, unsigned long start, unsigned long end)
{
	PathSummary summary("Recursive BFS");

	LinkedList<unsigned long> storage;
	unordered_map<unsigned long, unsigned long> previous;

	PushFunction pushFunction = &LinkedList<unsigned long>::push_back;
	PopFunction popFunction = &LinkedList<unsigned long>::pop_front;

	auto startTime = std::chrono::high_resolution_clock::now();
	storage.push_back(start);
	previous.insert({start, start});
	DoRecursiveXFS(graph, end, previous, storage, pushFunction, popFunction);
	auto finishTime = std::chrono::high_resolution_clock::now();

	LinkedList<unsigned long> computed;
	unsigned long cur = end;

	while (cur != start)
	{
		computed.push_front(cur);
		cur = previous.at(cur);
	}

	computed.push_front(start);

	for (unsigned long& n : computed)
	{
		summary.addStep(n, n == start ? 0 : graph->between(previous.at(n), n).getCost(), n == start ? 0 : graph->point(previous.at(n)).distance(graph->point(n)));
	}

	summary.setExecutionTime((finishTime - startTime).count());
	summary.setExplored(previous.size());

	return summary;
}

PathSummary SearchingAlgorithmDispatcher::IterativeBFS(AdjacencyStructure* graph, unsigned long start, unsigned long end)
{
	PathSummary summary("Iterative BFS");

	LinkedList<unsigned long> storage;
	unordered_map<unsigned long, unsigned long> previous;

	PushFunction pushFunction = &LinkedList<unsigned long>::push_back;
	PopFunction popFunction = &LinkedList<unsigned long>::pop_front;

	auto startTime = std::chrono::high_resolution_clock::now();
	storage.push_back(start);
	previous.insert({start, start});

	while (!storage.isEmpty())
	{
		unsigned long n = ProgressXFS(graph, end, previous, storage, pushFunction, popFunction);

		if (n == end)
		{
			break;
		}
	}

	auto finishTime = std::chrono::high_resolution_clock::now();

	LinkedList<unsigned long> computed;
	unsigned long cur = end;

	while (cur != start)
	{
		computed.push_front(cur);
		cur = previous.at(cur);
	}

	computed.push_front(start);

	for (unsigned long& n : computed)
	{
		summary.addStep(n, n == start ? 0 : graph->between(previous.at(n), n).getCost(), n == start ? 0 : graph->point(previous.at(n)).distance(graph->point(n)));
	}

	summary.setExecutionTime((finishTime - startTime).count());
	summary.setExplored(previous.size());

	return summary;
}

PathSummary SearchingAlgorithmDispatcher::RecursiveDFS(AdjacencyStructure* graph, unsigned long start, unsigned long end)
{
	PathSummary summary("Recursive DFS");

	LinkedList<unsigned long> storage;
	unordered_map<unsigned long, unsigned long> previous;

	PushFunction pushFunction = &LinkedList<unsigned long>::push_front;
	PopFunction popFunction = &LinkedList<unsigned long>::pop_front;

	auto startTime = std::chrono::high_resolution_clock::now();
	storage.push_back(start);
	previous.insert({start, start});
	DoRecursiveXFS(graph, end, previous, storage, pushFunction, popFunction);
	auto finishTime = std::chrono::high_resolution_clock::now();

	LinkedList<unsigned long> computed;
	unsigned long cur = end;

	while (cur != start)
	{
		computed.push_front(cur);
		cur = previous.at(cur);
	}

	computed.push_front(start);

	for (unsigned long& n : computed)
	{
		summary.addStep(n, n == start ? 0 : graph->between(previous.at(n), n).getCost(), n == start ? 0 : graph->point(previous.at(n)).distance(graph->point(n)));
	}

	summary.setExecutionTime((finishTime - startTime).count());
	summary.setExplored(previous.size());

	return summary;
}

PathSummary SearchingAlgorithmDispatcher::IterativeDFS(AdjacencyStructure* graph, unsigned long start, unsigned long end)
{
	PathSummary summary("Iterative DFS");

	LinkedList<unsigned long> storage;
	unordered_map<unsigned long, unsigned long> previous;

	PushFunction pushFunction = &LinkedList<unsigned long>::push_front;
	PopFunction popFunction = &LinkedList<unsigned long>::pop_front;

	auto startTime = std::chrono::high_resolution_clock::now();
	storage.push_back(start);
	previous.insert({start, start});

	while (!storage.isEmpty())
	{
		unsigned long n = ProgressXFS(graph, end, previous, storage, pushFunction, popFunction);

		if (n == end)
		{
			break;
		}
	}

	auto finishTime = std::chrono::high_resolution_clock::now();

	LinkedList<unsigned long> computed;
	unsigned long cur = end;

	while (cur != start)
	{
		computed.push_front(cur);
		cur = previous.at(cur);
	}

	computed.push_front(start);

	for (unsigned long& n : computed)
	{
		summary.addStep(n, n == start ? 0 : graph->between(previous.at(n), n).getCost(), n == start ? 0 : graph->point(previous.at(n)).distance(graph->point(n)));
	}

	summary.setExecutionTime((finishTime - startTime).count());
	summary.setExplored(previous.size());

	return summary;
}

PathSummary SearchingAlgorithmDispatcher::Dijkstra(AdjacencyStructure* graph, unsigned long start, unsigned long end)
{
	PathSummary summary("Dijkstra");

	auto startTime = std::chrono::high_resolution_clock::now();
	unordered_map<unsigned long, unsigned long> previous = BestPath(graph, start, end, &(SearchingAlgorithmDispatcher::DijkstraWeight));
	auto finishTime = std::chrono::high_resolution_clock::now();

	LinkedList<unsigned long> computed;
	unsigned long cur = end;

	while (cur != start)
	{
		computed.push_front(cur);
		cur = previous.at(cur);
	}

	computed.push_front(start);

	for (unsigned long& n : computed)
	{
		summary.addStep(n, n == start ? 0 : graph->between(previous.at(n), n).getCost(), n == start ? 0 : graph->point(previous.at(n)).distance(graph->point(n)));
	}

	summary.setExecutionTime((finishTime - startTime).count());
	summary.setExplored(previous.size());

	return summary;
}

PathSummary SearchingAlgorithmDispatcher::AStar(AdjacencyStructure* graph, unsigned long start, unsigned long end)
{
	PathSummary summary("A*");

	auto startTime = std::chrono::high_resolution_clock::now();
	unordered_map<unsigned long, unsigned long> previous = BestPath(graph, start, end, &(SearchingAlgorithmDispatcher::AStarWeight));
	auto finishTime = std::chrono::high_resolution_clock::now();

	LinkedList<unsigned long> computed;
	unsigned long cur = end;

	while (cur != start)
	{
		computed.push_front(cur);
		cur = previous.at(cur);
	}

	computed.push_front(start);

	for (unsigned long& n : computed)
	{
		summary.addStep(n, n == start ? 0 : graph->between(previous.at(n), n).getCost(), n == start ? 0 : graph->point(previous.at(n)).distance(graph->point(n)));
	}

	summary.setExecutionTime((finishTime - startTime).count());
	summary.setExplored(previous.size());

	return summary;
}

AlgorithmFunction SearchingAlgorithmDispatcher::getAlgorithm(SearchingAlgorithmType type)
{
	switch (type)
	{
	case BFS_RECURSIVE:
		return &(SearchingAlgorithmDispatcher::RecursiveBFS);
	case BFS_ITERATIVE:
			return &(SearchingAlgorithmDispatcher::IterativeBFS);
	case DFS_RECURSIVE:
			return &(SearchingAlgorithmDispatcher::RecursiveDFS);
	case DFS_ITERATIVE:
			return &(SearchingAlgorithmDispatcher::IterativeDFS);
	case DIJKSTRA:
			return &(SearchingAlgorithmDispatcher::Dijkstra);
	case A_STAR:
			return &(SearchingAlgorithmDispatcher::AStar);
	default:
		return nullptr;
	}
}
