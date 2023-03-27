#ifndef __RANDOMGENERATOR_H__
#define __RANDOMGENERATOR_H__

#include "App.h"
#include "Module.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

//TODO 1: Create the data structures needed for the algorithm



//-----------------------------------------------------------

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



//-----------------------------------------------------------
};

#endif // __RANDOMGENERATOR_H__