#ifndef __RANDOMGENERATOR_H__
#define __RANDOMGENERATOR_H__

#include "App.h"
#include "Module.h"
#include "Point.h"

struct Triangle
{
	iPoint A;
	iPoint B;
	iPoint C;
	bool incomplete;
};

class RandomGenerator
{
public:

	RandomGenerator();

	~RandomGenerator();

	void Update();

	void AddPoint(iPoint p);
	void AddTriangle(Triangle t);

	void GeneratePoints(int amount);

	iPoint FindCircumcenter(Triangle tri);

	void DrawPoints();
	void DrawTriangles();

private:

	List<iPoint*> points;
	List<Triangle*> triangles;
};

#endif // __RANDOMGENERATOR_H__