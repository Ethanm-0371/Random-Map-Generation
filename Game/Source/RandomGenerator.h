#ifndef __RANDOMGENERATOR_H__
#define __RANDOMGENERATOR_H__

#include "App.h"
#include "Module.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

//TODO 1: Create the data structures needed for the algorithm
enum RoomDoorType
{
	UDLR,
	LR,
	UD,
	UDL,
	UDR,
	ULR,
	DLR,
	UR,
	UL,
	DL,
	DR,
	U,
	D,
	L,
	R,
	NO_ROOMS
};

class Room
{
public:
	
	Room() {}
	Room(int type)
	{
		this->type = type;
		doorTop = doorBot = doorLeft = doorRight = false;
		doorsType = NO_ROOMS;
	}
	~Room() {}


private:
	int type;
	bool doorTop, doorBot, doorLeft, doorRight;
	RoomDoorType doorsType;

	friend class RandomGenerator;
};

class RandomGenerator
{
public:

	RandomGenerator();

	~RandomGenerator();

	iPoint GetWorldSize() { return worldSize; }

	void Start();

	void Update();

	int CalculateRoomChance();
	void CheckNeighbours(iPoint* position);
	void SetRoomDoors(int x, int y);
	void SetRoomDoorTypes(int x, int y);

	void PlaceCenter();
	void PlaceRooms();
	void PlaceCorridors();

	void DrawRooms();

private:

	//TODO 1: Create the data structures needed for the algorithm
	iPoint worldSize = { 9,9 };
	Room grid[9][9];
	List<iPoint*> roomsToCheck;
	int spawnChance = 50;
	int roomsPlaced = 0;
	int maximumRooms = 20;
};

#endif // __RANDOMGENERATOR_H__