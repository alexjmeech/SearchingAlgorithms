#include "Search.h"
#include "SearchingAlgorithmDispatcher.h"
#include <iostream>

int main(int argv, char** argc)
{
	srand(time(NULL));

	Algorithm* a = new Search();
	a->Load("input/");

	if (argv == 2)
	{
		cout << "Compiling Report Data" << endl;

		unsigned long starts[100];
		unsigned long ends[100];

		for (unsigned long i = 0; i < 100; i++)
		{
			starts[i] = rand() % ((Search*)a)->LoadedNodes();
			ends[i] = rand() % ((Search*)a)->LoadedNodes();
		}

		for (int i = SearchingAlgorithmType::BFS_RECURSIVE; i < SearchingAlgorithmType::END_SEARCHES; i++)
		{
			a->Select(i);
			cout << "Reporting on Algorithm " << i << endl;

			for (unsigned long j = 0; j < 100; j++)
			{
				((Search*)a)->Execute(starts[j], ends[j]);
			}

			a->Save();
		}

		cout << "Report Data Compiled" << endl;

		delete a;
		return 0;
	}

	for (int i = SearchingAlgorithmType::BFS_RECURSIVE; i < SearchingAlgorithmType::END_SEARCHES; i++)
	{
		a->Select(i);

		if (argv > 2)
		{
			unsigned long start = strtoul(argc[1], NULL, 10) - 1;
			unsigned long end = strtoul(argc[2], NULL, 10) - 1;
			((Search*)a)->Execute(start, end);
			a->Stats();
			cout << endl;
		}
		else
		{
			a->Execute();
			a->Stats();
			cout << endl;
		}

		a->Stats();
		cout << endl;
	}

	delete a;

	return 0;
}
