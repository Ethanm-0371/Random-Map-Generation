#include "RandomGenerator.h"

#include "Window.h"
#include "Render.h"

#include "Log.h"

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
	tri->isSuper = t.isSuper;

	triangles.Add(tri);
}

void RandomGenerator::GeneratePoints(int amount)
{
	uint w; uint h;
	app->win->GetWindowSize(w, h);

	for (size_t i = 0; i < amount; i++)
	{
		iPoint point = { randVal(300, w - 400), randVal(300, h - 100) };

		ListItem<iPoint*>* p;
		p = points.start;
		while (p != nullptr)
		{
			if (point.x == p->data->x && point.y == p->data->y)
			{
				point = { randVal(300, w - 400), randVal(300, h - 100) };
			}

			p = p->next;
		}

		AddPoint(point);
	}

	iPoint t1 = { 600, -360 };
	iPoint t2 = { 50, 720 };
	iPoint t3 = { 1150, 720 };

	Triangle tri = { t1,t2,t3 };
	tri.isSuper = true;
	AddTriangle(tri);
}

iPoint RandomGenerator::FindCircumcenter(Triangle* tri)
{
	float D = (tri->A.x * (tri->B.y - tri->C.y) + tri->B.x * (tri->C.y - tri->A.y) + tri->C.x * (tri->A.y - tri->B.y)) * 2;

	float x = (1 / D) * ((tri->A.x * tri->A.x + tri->A.y * tri->A.y) * (tri->B.y - tri->C.y) + (tri->B.x * tri->B.x + tri->B.y * tri->B.y) * (tri->C.y - tri->A.y) + (tri->C.x * tri->C.x + tri->C.y * tri->C.y) * (tri->A.y - tri->B.y));
	float y = (1 / D) * ((tri->A.x * tri->A.x + tri->A.y * tri->A.y) * (tri->C.x - tri->B.x) + (tri->B.x * tri->B.x + tri->B.y * tri->B.y) * (tri->A.x - tri->C.x) + (tri->C.x * tri->C.x + tri->C.y * tri->C.y) * (tri->B.x - tri->A.x));

	iPoint ret = { (int)x,(int)y };
	return ret;
}

int RandomGenerator::calculateDistance(iPoint* point, iPoint triCircumcenter)
{
	int dx = point->x - triCircumcenter.x;
	int dy = point->y - triCircumcenter.y;
	int sum = dx * dx + dy * dy;
	int distance = sqrt(sum);
	return distance;
}

bool RandomGenerator::distanceCheck(iPoint* point, iPoint triCircumcenter, iPoint vertex)
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
			iPoint circumCenter = FindCircumcenter(t->data);

			if (distanceCheck(p->data,circumCenter,t->data->A))
			{
				badTriangles.Add(t->data);
			}
			t = t->next;
		}

		if (badTriangles.Count() != 0)
		{
			ListItem<Triangle*>* badT;
			badT = badTriangles.start;
			while (badT != nullptr)
			{
				if (!badT->data->isSuper)
				{
					triangles.Del(badT);
				}

				Triangle new1;
				Triangle new2;
				Triangle new3;

				new1.A.x = p->data->x; new1.A.y = p->data->y;
				new1.B = badT->data->B;
				new1.C = badT->data->C;

				new2.A = badT->data->A;
				new2.B.x = p->data->x; new2.A.y = p->data->y;
				new2.C = badT->data->C;

				new3.A = badT->data->A;
				new3.B = badT->data->B;
				new3.C.x = p->data->x; new3.C.y = p->data->y;

				AddTriangle(new1);
				AddTriangle(new2);
				AddTriangle(new3);

				badT = badT->next;
			}
			badTriangles.Clear();
		}

		p = p->next;
	}

	LOG("Triangulation Done");
}

void RandomGenerator::DelaunayTriangulation2()
{
	ListItem<iPoint*>* p;
	p = points.start;

	while (p != nullptr)
	{
		LOG("x: %d, y: %d", p->data->x, p->data->y);

		ListItem<Triangle*>* t;
		t = triangles.start;

		while (t != nullptr)
		{
			LOG("I found a triangle");

			LOG("I will now look for the circumcenter of this triangle");
			iPoint circumCenter = FindCircumcenter(t->data);
			LOG("Circumcenter found");

			LOG("I will now check if the point is in the radius of the circumcentre");
			if (distanceCheck(p->data, circumCenter, t->data->A))
			{
				LOG("This is inside. Adding to bad triangles");
				badTriangles.Add(t->data);
			}
			else
			{
				LOG("This is not inside all good");
			}
			
			LOG("Bad triangles count: %d", badTriangles.Count());

			t = t->next;
		}

		if (badTriangles.Count() != 0)
		{
			ListItem<Triangle*>* badT;
			badT = badTriangles.start;
			while (badT != nullptr)
			{
				LOG("I am now removing a bad triangle from the triangles list");
				if (!badT->data->isSuper)
				{
					triangles.Del(badT);
					LOG("Triangle removed");
				}
				else { LOG("Oh, wait, it's the Super Triangle, nevermind"); }

				LOG("Bad triangles count: %d", badTriangles.Count());
				LOG("Triangles count: %d", triangles.Count());

				//Create new triangles and add them to triangles list

				LOG("I am now going to create the new triangles");

				Triangle new1;
				new1.A.x = p->data->x; new1.A.y = p->data->y;
				new1.B = badT->data->B;
				new1.C = badT->data->C;
				AddTriangle(new1);

				Triangle new2;
				new2.A = badT->data->A;
				new2.B.x = p->data->x; new2.A.y = p->data->y;
				new2.C = badT->data->C;
				AddTriangle(new2);

				Triangle new3;
				new3.A = badT->data->A;
				new3.B = badT->data->B;
				new3.C.x = p->data->x; new3.C.y = p->data->y;
				AddTriangle(new3);

				LOG("New triangles created.");
				LOG("Bad triangles count: %d", badTriangles.Count());
				LOG("Triangles count: %d", triangles.Count());

				badT = badT->next;
			}
		}
		

		badTriangles.Clear();
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

		iPoint circumCenter = FindCircumcenter(tri->data);
		SDL_Rect rect = { circumCenter.x,circumCenter.y,5,5 };
		app->render->DrawRectangle(rect, 255, 255, 255, 255, true, true);

		tri = tri->next;
	}
}
