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
	tri->A = t.A;
	tri->B = t.B;
	tri->C = t.C;
	tri->incomplete = true;

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

iPoint RandomGenerator::FindCircumcenter(Triangle tri)
{
	float D = (tri.A.x * (tri.B.y - tri.C.y) + tri.B.x * (tri.C.y - tri.A.y) + tri.C.x * (tri.A.y - tri.B.y)) * 2;

	float x = (1 / D) * ((tri.A.x * tri.A.x + tri.A.y * tri.A.y) * (tri.B.y - tri.C.y) + (tri.B.x * tri.B.x + tri.B.y * tri.B.y) * (tri.C.y - tri.A.y) + (tri.C.x * tri.C.x + tri.C.y * tri.C.y) * (tri.A.y - tri.B.y));
	float y = (1 / D) * ((tri.A.x * tri.A.x + tri.A.y * tri.A.y) * (tri.C.x - tri.B.x) + (tri.B.x * tri.B.x + tri.B.y * tri.B.y) * (tri.A.x - tri.C.x) + (tri.C.x * tri.C.x + tri.C.y * tri.C.y) * (tri.B.x - tri.A.x));

	iPoint ret = { (int)x,(int)y };
	return ret;
}

int RandomGenerator::calculateDistance(iPoint point, iPoint triCircumcenter)
{
	int dx = point.x - triCircumcenter.x;
	int dy = point.y - triCircumcenter.y;
	int sum = dx * dx + dy * dy;
	int distance = sqrt(sum);
	return distance;
}

bool RandomGenerator::distanceCheck(iPoint point, iPoint triCircumcenter, iPoint vertex)
{
	int distance = calculateDistance(point, triCircumcenter);
	int radius = calculateDistance(point, vertex);

	if (distance < radius)
	{
		return true;
	}

	return false;
}

void RandomGenerator::DelaunayTriangulation()
{
	ListItem<iPoint*>* p;
	p = points.start;

	while (p != nullptr)
	{
		ListItem<Triangle*>* t;
		t = triangles.start;

		while (t != nullptr)
		{
			Triangle currentT; currentT.A = t->data->A; currentT.B = t->data->B; currentT.C = t->data->C; currentT.incomplete = t->data->incomplete;
			iPoint circumCenter = FindCircumcenter(currentT);

			if (distanceCheck(p->data,circumCenter,t->data->A))
			{
				//delaunay video minute 3:00
			}

			t = t->next;
		}

		p = p->next;
	}
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
		app->render->DrawLine(tri->data->A.x, tri->data->A.y, tri->data->B.x, tri->data->B.y, 0, 255, 0, 255, true);
		app->render->DrawLine(tri->data->B.x, tri->data->B.y, tri->data->C.x, tri->data->C.y, 0, 255, 0, 255, true);
		app->render->DrawLine(tri->data->C.x, tri->data->C.y, tri->data->A.x, tri->data->A.y, 0, 255, 0, 255, true);

		Triangle t; t.A = tri->data->A; t.B = tri->data->B; t.C = tri->data->C; t.incomplete = tri->data->incomplete;
		iPoint circumCenter = FindCircumcenter(t);
		SDL_Rect rect = { circumCenter.x,circumCenter.y,5,5 };
		app->render->DrawRectangle(rect, 255, 255, 255, 255, true, true);

		tri = tri->next;
	}
}
