#include "RandomGenerator.h"

#include "Render.h"

RandomGenerator::RandomGenerator()
{
	
}

RandomGenerator::~RandomGenerator()
{}

void RandomGenerator::Update()
{
	DrawTriangle(tri);
}

void RandomGenerator::DrawTriangle(Triangle t)
{
	app->render->DrawLine(t.p1.x, t.p1.y, t.p2.x, t.p2.y, 0, 255, 0, 255, true);
	app->render->DrawLine(t.p2.x, t.p2.y, t.p3.x, t.p3.y, 0, 255, 0, 255, true);
	app->render->DrawLine(t.p3.x, t.p3.y, t.p1.x, t.p1.y, 0, 255, 0, 255, true);
}
