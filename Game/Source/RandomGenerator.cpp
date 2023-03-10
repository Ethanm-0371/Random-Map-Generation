#include "RandomGenerator.h"

#include "Window.h"
#include "Render.h"

int randVal(int min, int max) {

	return (rand() % (max - min + 1)) + min;
}

RandomGenerator::RandomGenerator()
{
	
}

RandomGenerator::~RandomGenerator()
{}

void RandomGenerator::Update()
{
	DrawPoints();
	DrawTriangles();
}

void RandomGenerator::AddPoint(iPoint p)
{
	iPoint* point = new iPoint;
	point->x = p.x;
	point->y = p.y;

	points.Add(point);
}

void RandomGenerator::AddTriangle(Triangle t)
{
	Triangle* tri = new Triangle;
	tri->p1 = t.p1;
	tri->p2 = t.p2;
	tri->p3 = t.p3;

	triangles.Add(tri);
}

void RandomGenerator::GeneratePoints(int amount)
{
	uint w; uint h;
	app->win->GetWindowSize(w, h);

	for (size_t i = 0; i < amount; i++)
	{
		iPoint point = { randVal(300, w - 300), randVal(200, h - 200) };

		ListItem<iPoint*>* p;
		p = points.start;
		while (p != nullptr)
		{
			if (point.x == p->data->x && point.y == p->data->y)
			{
				point = { randVal(300, w - 300), randVal(200, h - 200) };
			}

			p = p->next;
		}

		AddPoint(point);
	}

	iPoint t1 = { 600, -360 };
	iPoint t2 = { 50, 720 };
	iPoint t3 = { 1150, 720 };

	AddPoint(t1);
	AddPoint(t2);
	AddPoint(t3);

	Triangle tri = { t1,t2,t3 };
	AddTriangle(tri);
}

void RandomGenerator::DrawPoints()
{
	ListItem<iPoint*>* p;
	p = points.start;

	while (p != nullptr)
	{
		SDL_Rect rect = { p->data->x,p->data->y,10,10 };

		app->render->DrawRectangle(rect, 0, 255, 0, 255, true, true);

		p = p->next;
	}
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
