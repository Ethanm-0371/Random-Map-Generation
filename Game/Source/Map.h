#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;

	SDL_Rect GetTileRect(int gid) const;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

//TODO 9: Create the data structures needed to read the chunks from the XML file
struct Chunk
{
	iPoint location;
	int width, height;
	uint* data;

	Chunk() : data(NULL)
	{}

	~Chunk()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
	MapTypes type;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

	void DrawChunk(Chunk* chunk, iPoint position);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load();

	iPoint MapToScreen(int x, int y) const;

	iPoint Map::ScreenToMap(int x, int y);

private:

	bool LoadMap(pugi::xml_node mapFile);

	bool LoadTileSet(pugi::xml_node mapFile);

	bool LoadChunk(pugi::xml_node& node, Chunk* chunkToLoad);
	bool LoadAllChunks(pugi::xml_node mapNode);

	TileSet* GetTilesetFromTileId(int gid) const;

public: 

	MapData mapData;

private:

    SString mapFileName;
	SString mapFolder;
    bool mapLoaded;

	//TODO 9: Create the data structures needed to read the chunks from the XML file
	Chunk* UDLR;
	Chunk* LR;
	Chunk* UD;
	Chunk* UDL;
	Chunk* UDR;
	Chunk* ULR;
	Chunk* DLR;
	Chunk* UR;
	Chunk* UL;
	Chunk* DL;
	Chunk* DR;
	Chunk* U;
	Chunk* D;
	Chunk* L;
	Chunk* R;


	friend class RandomGenerator;
};

#endif // __MAP_H__
