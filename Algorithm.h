#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <string>
using namespace std;

class Algorithm
{
public:
	virtual void Load(string) = 0;
	virtual void Execute() = 0;
	virtual void Display() = 0;
	virtual void Stats() = 0;
	virtual void Select(int) = 0;
	virtual void Save() = 0;
	virtual void Configure() = 0;

	virtual ~Algorithm() {}
};

#endif /* ALGORITHM_H_ */
