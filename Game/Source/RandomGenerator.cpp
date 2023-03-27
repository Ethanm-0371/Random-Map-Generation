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

	if (maximumRooms >= worldSize.x * worldSize.y)
	{
		maximumRooms = worldSize.x * worldSize.y;
	}

	for (int x = 0; x < worldSize.x; x++)
	{
		for (int y = 0; y < worldSize.y; y++)
		{
			iPoint pos(x, y);

			grid[x][y] = Room(-1);
		}
	}

	PlaceCenter();

	PlaceRooms();
	PlaceCorridors();
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

	if (up.y >= 0 && grid[up.x][up.y].type == -1)
	{
		if (CalculateRoomChance() < spawnChance)
		{
			grid[up.x][up.y].type = 1;
			roomsPlaced++;

			iPoint* upRoom = new iPoint(up);
			roomsToCheck.Add(upRoom);
		}
	}
	if (down.y < worldSize.y && grid[down.x][down.y].type == -1)
	{
		if (CalculateRoomChance() < spawnChance)
		{
			grid[down.x][down.y].type = 1;
			roomsPlaced++;

			iPoint* downRoom = new iPoint(down);
			roomsToCheck.Add(downRoom);
		}
	}
	if (right.x < worldSize.x && grid[right.x][right.y].type == -1)
	{
		if (CalculateRoomChance() < spawnChance)
		{
			grid[right.x][right.y].type = 1;
			roomsPlaced++;

			iPoint* rightRoom = new iPoint(right);
			roomsToCheck.Add(rightRoom);
		}
	}
	if (left.x >= 0 && grid[left.x][left.y].type == -1)
	{
		if (CalculateRoomChance() < spawnChance)
		{
			grid[left.x][left.y].type = 1;
			roomsPlaced++;

			iPoint* leftRoom = new iPoint(left);
			roomsToCheck.Add(leftRoom);
		}
	}
}

void RandomGenerator::SetRoomDoors(int x, int y)
{
	iPoint up;		up.x = x;			up.y = y - 1;
	iPoint down;	down.x = x;			down.y = y + 1;
	iPoint right;	right.x = x + 1;	right.y = y;
	iPoint left;	left.x = x - 1;		left.y = y;

	//TODO 7: Check if there are rooms in every direction and assign the corresponding door variables

	if (up.y >= 0 && grid[up.x][up.y].type != -1)
	{
		grid[x][y].doorTop = true;
	}
	if (down.y < worldSize.y && grid[down.x][down.y].type != -1)
	{
		grid[x][y].doorBot = true;
	}
	if (right.x < worldSize.x && grid[right.x][right.y].type != -1)
	{
		grid[x][y].doorRight = true;
	}
	if (left.x >= 0 && grid[left.x][left.y].type != -1)
	{
		grid[x][y].doorLeft = true;
	}
}

void RandomGenerator::SetRoomDoorTypes(int x, int y)
{
	//TODO 8: Depending on the door variables, assign one RoomDoor type or another

	if (grid[x][y].type == 1)
	{
		//UDLR
		if (grid[x][y].doorTop && grid[x][y].doorBot && grid[x][y].doorLeft && grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::UDLR;
		}


		//UDL
		else if (grid[x][y].doorTop && grid[x][y].doorBot && grid[x][y].doorLeft && !grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::UDL;
		}
		//UDR
		else if (grid[x][y].doorTop && grid[x][y].doorBot && !grid[x][y].doorLeft && grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::UDR;
		}
		//ULR
		else if (grid[x][y].doorTop && !grid[x][y].doorBot && grid[x][y].doorLeft && grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::ULR;
		}
		//DLR
		else if (!grid[x][y].doorTop && grid[x][y].doorBot && grid[x][y].doorLeft && grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::DLR;
		}


		//UL
		else if (grid[x][y].doorTop && !grid[x][y].doorBot && grid[x][y].doorLeft && !grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::UL;
		}
		//UR
		else if (grid[x][y].doorTop && !grid[x][y].doorBot && !grid[x][y].doorLeft && grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::UR;
		}
		//DL
		else if (!grid[x][y].doorTop && grid[x][y].doorBot && grid[x][y].doorLeft && !grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::DL;
		}
		//DR
		else if (!grid[x][y].doorTop && grid[x][y].doorBot && !grid[x][y].doorLeft && grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::DR;
		}


		//UD
		else if (grid[x][y].doorTop && grid[x][y].doorBot && !grid[x][y].doorLeft && !grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::UD;
		}
		//LR
		else if (!grid[x][y].doorTop && !grid[x][y].doorBot && grid[x][y].doorLeft && grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::LR;
		}


		//U
		else if (grid[x][y].doorTop && !grid[x][y].doorBot && !grid[x][y].doorLeft && !grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::U;
		}
		//D
		else if (!grid[x][y].doorTop && grid[x][y].doorBot && !grid[x][y].doorLeft && !grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::D;
		}
		//L
		else if (!grid[x][y].doorTop && !grid[x][y].doorBot && grid[x][y].doorLeft && !grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::L;
		}
		//R
		else if (!grid[x][y].doorTop && !grid[x][y].doorBot && !grid[x][y].doorLeft && grid[x][y].doorRight)
		{
			grid[x][y].doorsType = RoomDoorType::R;
		}

		else
		{
			LOG("Error: No rooms were generated. Please generate a new dungeon");
			grid[x][y].doorsType = RoomDoorType::NO_ROOMS;
		}
	}
}

void RandomGenerator::PlaceCenter()
{
	//TODO 3: Place the center room and add it to the list of rooms to check

	int centerX = (int)floor(worldSize.x / 2.0f);
	int centerY = (int)floor(worldSize.y / 2.0f);
	iPoint* center = new iPoint(centerX, centerY);

	grid[centerX][centerY].type = 1;
	roomsPlaced++;
	roomsToCheck.Add(center);
}

void RandomGenerator::PlaceRooms()
{
	//TODO 4: Iterate the list of rooms to check and check their neighbours

	ListItem<iPoint*>* currentRoom;
	currentRoom = roomsToCheck.start;

	while (currentRoom != nullptr && roomsPlaced < maximumRooms)
	{
		CheckNeighbours(currentRoom->data);
		
		currentRoom = currentRoom->next;
	}

	LOG("Rooms created");
}

void RandomGenerator::PlaceCorridors()
{
	//TODO 6: Set the rooms' doors and all of their according types

	for (int i = 0; i < worldSize.x; i++)
	{
		for (int j = 0; j < worldSize.y; j++)
		{
			SetRoomDoors(i, j);
			SetRoomDoorTypes(i, j);
		}
	}
}

void RandomGenerator::DrawRooms()
{
	//TODO 13: Iterate the map and draw the according chunk depending on its type in the correct position

	for (int i = 0; i < worldSize.x; i++)
	{
		for (int j = 0; j < worldSize.y; j++)
		{
			int xPlacement = i * 16;
			int yPlacement = j * 16;

			iPoint placement = { xPlacement,yPlacement };

			if (grid[i][j].type == 1)
			{
				switch (grid[i][j].doorsType)
				{
				case UDLR:
					app->map->DrawChunk(app->map->UDLR, placement);
					break;


				case UDL:
					app->map->DrawChunk(app->map->UDL, placement);
					break;
				case UDR:
					app->map->DrawChunk(app->map->UDR, placement);
					break;
				case ULR:
					app->map->DrawChunk(app->map->ULR, placement);
					break;
				case DLR:
					app->map->DrawChunk(app->map->DLR, placement);
					break;


				case UR:
					app->map->DrawChunk(app->map->UR, placement);
					break;
				case UL:
					app->map->DrawChunk(app->map->UL, placement);
					break;
				case DL:
					app->map->DrawChunk(app->map->DL, placement);
					break;
				case DR:
					app->map->DrawChunk(app->map->DR, placement);
					break;


				case UD:
					app->map->DrawChunk(app->map->UD, placement);
					break;
				case LR:
					app->map->DrawChunk(app->map->LR, placement);
					break;


				case U:
					app->map->DrawChunk(app->map->U, placement);
					break;
				case D:
					app->map->DrawChunk(app->map->D, placement);
					break;
				case L:
					app->map->DrawChunk(app->map->L, placement);
					break;
				case R:
					app->map->DrawChunk(app->map->R, placement);
					break;


				case NO_ROOMS:
					LOG("Error: No rooms were generated. Please generate a new dungeon");
					app->map->DrawChunk(app->map->UDLR, placement);
					break;
				default:
					break;
				}
			}
		}
	}
}
