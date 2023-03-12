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
	bool isSuper = false;
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

	iPoint FindCircumcenter(Triangle* tri);

	int calculateDistance(iPoint* point, iPoint triCircumcenter);
	bool distanceCheck(iPoint* point, iPoint triCircumcenter, iPoint vertex);

	void DelaunayTriangulation();

	void DrawPoints();
	void DrawTriangles();

private:

	List<iPoint*> points;
	List<Triangle*> triangles;
	List<Triangle*> badTriangles;
};

#endif // __RANDOMGENERATOR_H__