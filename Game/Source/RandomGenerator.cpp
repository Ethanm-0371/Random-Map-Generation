#include "RandomGenerator.h"

#include "Render.h"

RandomGenerator::RandomGenerator()
{
	
}

RandomGenerator::~RandomGenerator()
{}

void RandomGenerator::Update()
{
	DrawTriangles();
}

void RandomGenerator::AddTriangle(Triangle t)
{
	Triangle* tri = new Triangle;
	tri->p1 = t.p1;
	tri->p2 = t.p2;
	tri->p3 = t.p3;

	triangles.Add(tri);
}

void RandomGenerator::DrawTriangles()
{
	ListItem<Triangle*>* tri;
	tri = triangles.start;

	while (tri != nullptr)
	{
		app->render->DrawLine(tri->data->p1.x, tri->data->p1.y, tri->data->p2.x, tri->data->p2.y, 0, 255, 0, 255, true);
		app->render->DrawLine(tri->data->p2.x, tri->data->p2.y, tri->data->p3.x, tri->data->p3.y, 0, 255, 0, 255, true);
		app->render->DrawLine(tri->data->p3.x, tri->data->p3.y, tri->data->p1.x, tri->data->p1.y, 0, 255, 0, 255, true);

		tri = tri->next;
	}
}
