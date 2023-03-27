
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{

}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    return ret;
}

void Map::DrawChunk(Chunk* chunk, iPoint position)
{
    //TODO 12: Blit the according tile to their correct position inside the chunk

    for (int x = 0; x < chunk->width; x++)
    {
        for (int y = 0; y < chunk->height; y++)
        {
            int gid = chunk->Get(x, y);

            TileSet* tileset = GetTilesetFromTileId(gid); // (!!) we are using always the first tileset in the list

            SDL_Rect r = tileset->GetTileRect(gid);

            iPoint mapPos; 
            mapPos.x = x + position.x;
            mapPos.y = y + position.y;
            iPoint pos = MapToScreen(mapPos.x, mapPos.y);

            SDL_Texture* tile_texture = tileset->texture;

            app->render->DrawTexture(tile_texture, pos.x, pos.y, &r);
        }
    }
}

//Translates x, y coordinates from map positions to screen coordinates
iPoint Map::MapToScreen(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

//Obtain map positions from screen coordinates
iPoint Map::ScreenToMap(int x, int y)
{
    iPoint ret(0, 0);

    if (mapData.type == MAPTYPE_ORTHOGONAL)
    {
        ret.x = x / mapData.tileWidth;
        ret.y = y / mapData.tileHeight;
    }
    else if (mapData.type == MAPTYPE_ISOMETRIC)
    {
        float halfWidth = mapData.tileWidth * 0.5f;
        float halfHeight = mapData.tileHeight * 0.5f;
        ret.x = int((x / halfWidth + y / halfHeight) / 2);
        ret.y = int((y / halfHeight - x / halfWidth) / 2);
    }
    else
    {
        LOG("Unknown map type");
        ret.x = x; ret.y = y;
    }

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    TileSet* set = NULL;

    ListItem<TileSet*>* item = mapData.tilesets.start;
    while (item != NULL)
    {
        set = item->data;
        if (gid < item->data->firstgid + item->data->tilecount) break;
        
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    return true;
}

// Load new map
bool Map::Load()
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true) ret = LoadMap(mapFileXML);
    
    if (ret == true) ret = LoadTileSet(mapFileXML);

    if (ret == true) ret = LoadAllChunks(mapFileXML.child("map"));
    

    if(ret == true)
    {
       
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) 
        {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        // Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
    }

    mapData.type = MAPTYPE_UNKNOWN;
    if ((SString)map.attribute("orientation").as_string() == "isometric")
    {
        mapData.type = MAPTYPE_ISOMETRIC;
    }

    if ((SString)map.attribute("orientation").as_string() == "orthogonal")
    {
        mapData.type = MAPTYPE_ORTHOGONAL;
    }

    return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

bool Map::LoadChunk(pugi::xml_node& node, Chunk* chunkToLoad)
{
    //TODO 11: Load all of the necessary information into the Chunk data structure

    chunkToLoad->location.x = node.attribute("x").as_int();
    chunkToLoad->location.y = node.attribute("y").as_int();
    chunkToLoad->width = node.attribute("width").as_int();
    chunkToLoad->height = node.attribute("height").as_int();

    //Reserve the memory for the data 
    chunkToLoad->data = new uint[chunkToLoad->width * chunkToLoad->height];
    memset(chunkToLoad->data, 0, chunkToLoad->width * chunkToLoad->height);
    
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("tile"); tile; tile = tile.next_sibling("tile"))
    {
        chunkToLoad->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return true;
}

bool Map::LoadAllChunks(pugi::xml_node mapNode)
{
    bool ret = true;

    //TODO 10: Look for all the chunks in the XML file. For each of them, load the individual chunk to its data structure

    for (pugi::xml_node chunkNode = mapNode.child("layer").child("data").child("chunk"); chunkNode && ret; chunkNode = chunkNode.next_sibling("chunk"))
    {
        //Cross
        if (chunkNode.attribute("x").as_int() == 0 && chunkNode.attribute("y").as_int() == 0)
        {
            UDLR = new Chunk();
            ret = LoadChunk(chunkNode, UDLR);
        }

        //Straights
        if (chunkNode.attribute("x").as_int() == 0 && chunkNode.attribute("y").as_int() == 16)
        {
            LR = new Chunk();
            ret = LoadChunk(chunkNode, LR);
        }
        if (chunkNode.attribute("x").as_int() == 16 && chunkNode.attribute("y").as_int() == 16)
        {
            UD = new Chunk();
            ret = LoadChunk(chunkNode, UD);
        }

        //Ts
        if (chunkNode.attribute("x").as_int() == 0 && chunkNode.attribute("y").as_int() == 32)
        {
            UDL = new Chunk();
            ret = LoadChunk(chunkNode, UDL);
        }
        if (chunkNode.attribute("x").as_int() == 16 && chunkNode.attribute("y").as_int() == 32)
        {
            UDR = new Chunk();
            ret = LoadChunk(chunkNode, UDR);
        }
        if (chunkNode.attribute("x").as_int() == 32 && chunkNode.attribute("y").as_int() == 32)
        {
            ULR = new Chunk();
            ret = LoadChunk(chunkNode, ULR);
        }
        if (chunkNode.attribute("x").as_int() == 48 && chunkNode.attribute("y").as_int() == 32)
        {
            DLR = new Chunk();
            ret = LoadChunk(chunkNode, DLR);
        }

        //Corners
        if (chunkNode.attribute("x").as_int() == 0 && chunkNode.attribute("y").as_int() == 48)
        {
            UR = new Chunk();
            ret = LoadChunk(chunkNode, UR);
        }
        if (chunkNode.attribute("x").as_int() == 16 && chunkNode.attribute("y").as_int() == 48)
        {
            UL = new Chunk();
            ret = LoadChunk(chunkNode, UL);
        }
        if (chunkNode.attribute("x").as_int() == 32 && chunkNode.attribute("y").as_int() == 48)
        {
            DL = new Chunk();
            ret = LoadChunk(chunkNode, DL);
        }
        if (chunkNode.attribute("x").as_int() == 48 && chunkNode.attribute("y").as_int() == 48)
        {
            DR = new Chunk();
            ret = LoadChunk(chunkNode, DR);
        }

        //Dead ends
        if (chunkNode.attribute("x").as_int() == 0 && chunkNode.attribute("y").as_int() == 64)
        {
            U = new Chunk();
            ret = LoadChunk(chunkNode, U);
        }
        if (chunkNode.attribute("x").as_int() == 16 && chunkNode.attribute("y").as_int() == 64)
        {
            D = new Chunk();
            ret = LoadChunk(chunkNode, D);
        }
        if (chunkNode.attribute("x").as_int() == 32 && chunkNode.attribute("y").as_int() == 64)
        {
            L = new Chunk();
            ret = LoadChunk(chunkNode, L);
        }
        if (chunkNode.attribute("x").as_int() == 48 && chunkNode.attribute("y").as_int() == 64)
        {
            R = new Chunk();
            ret = LoadChunk(chunkNode, R);
        }
    }

    return ret;
}
