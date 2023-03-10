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

	void AddTriangle(Triangle t);

	void DrawTriangles();

private:

	List<iPoint*> points;
	List<Triangle*> triangles;
};

#endif // __RANDOMGENERATOR_H__