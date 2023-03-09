#ifndef __RANDOMGENERATOR_H__
#define __RANDOMGENERATOR_H__

#include "App.h"
#include "Module.h"
#include "Point.h"

struct Triangle
{
	iPoint p1;
	iPoint p2;
	iPoint p3;
};

class RandomGenerator
{
public:

	RandomGenerator();

	~RandomGenerator();

	void Update();

	void DrawTriangle(Triangle t);

private:

	iPoint t1 = {5 * 100,5 * 100 };
	iPoint t2 = {3 * 100,1 * 100 };
	iPoint t3 = {2 * 100,4 * 100 };

	Triangle tri = {t1,t2,t3};
	List<iPoint*> points;
};

#endif // __RANDOMGENERATOR_H__