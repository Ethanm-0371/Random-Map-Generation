#include "RandomGenerator.h"

#include "Window.h"
#include "Render.h"
#include "Map.h"

#include "Log.h"

float randVal0_100() {

	float randomVal = (rand() % (1000 - 0 + 1)) + 0;

	float ret = randomVal / 10;

	return ret;
}

RandomGenerator::RandomGenerator()
{
}

RandomGenerator::~RandomGenerator()
{}

void RandomGenerator::Start()
{
	//TODO 2: Populate the map and call the main methods




	//-----------------------------------------------------------
}

void RandomGenerator::Update()
{
	DrawRooms();
}

int RandomGenerator::CalculateRoomChance()
{
	//This function would be used to generate a value and compare it to the chance a room has of spawning.
	//This generated value can be tweaked at will to adjust the dungeon generation.

	int ret = round(randVal0_100());

	return ret;
}

void RandomGenerator::CheckNeighbours(iPoint* position)
{
	iPoint up;		up.x = position->x;			up.y = position->y - 1;
	iPoint down;	down.x = position->x;		down.y = position->y + 1;
	iPoint right;	right.x = position->x + 1;	right.y = position->y;
	iPoint left;	left.x = position->x - 1;	left.y = position->y;

	//TODO 5: Check if the rooms in all directions are free, and if so and the chance is good, change the room type



	//-----------------------------------------------------------
}

void RandomGenerator::SetRoomDoors(int x, int y)
{
	iPoint up;		up.x = x;			up.y = y - 1;
	iPoint down;	down.x = x;			down.y = y + 1;
	iPoint right;	right.x = x + 1;	right.y = y;
	iPoint left;	left.x = x - 1;		left.y = y;

	//TODO 7: Check if there are rooms in every direction and assign the corresponding door variables



	//-----------------------------------------------------------
}

void RandomGenerator::SetRoomDoorTypes(int x, int y)
{
	//TODO 8: Depending on the door variables, assign one RoomDoor type or another



	//-----------------------------------------------------------
}

void RandomGenerator::PlaceCenter()
{
	//TODO 3: Place the center room and add it to the list of rooms to check



	//-----------------------------------------------------------
}

void RandomGenerator::PlaceRooms()
{
	//TODO 4: Iterate the list of rooms to check and check their neighbours



	//-----------------------------------------------------------

	LOG("Rooms created");
}

void RandomGenerator::PlaceCorridors()
{
	//TODO 6: Set the rooms' doors and all of their according types



	//-----------------------------------------------------------
}

void RandomGenerator::DrawRooms()
{
	//TODO 13: Iterate the map and draw the according chunk depending on its type in the correct position



	//-----------------------------------------------------------
}
