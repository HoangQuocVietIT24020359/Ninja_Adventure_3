#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <vector>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

struct MapData {
    std::vector<std::vector<int>> tiles;
    SDL_Texture* tileset;
    int tilesetWidth;
};

class MapParser {
public:
    MapParser(SDL_Renderer* renderer);
    ~MapParser();

    void LoadMap(const char* mapFile, const char* tilesetFile, bool isSolid = false, bool isTrap = false);
    void Render(int offsetX, int offsetY);
    inline int GetMapWidth(){return m_MapWidth;}
    inline int GetMapHeight(){return m_MapHeight;}
    
    std::vector<SDL_Rect> solidTiles;
    std::vector<SDL_Rect> trapTiles;

private:
    SDL_Renderer* renderer;
    int tileSize;
    std::vector<MapData> maps;
    int m_MapWidth, m_MapHeight;
};

#endif // MAPPARSER_H
