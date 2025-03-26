#include "MapParser.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

MapParser::MapParser(SDL_Renderer* renderer) : renderer(renderer), tileSize(16) {}

MapParser::~MapParser() {
    for (auto& map : maps) {
        if (map.tileset) SDL_DestroyTexture(map.tileset);
    }
}

// Load a map from file and its associated tileset
// Parameters:
//   mapFile - Path to the map data file
//   tilesetFile - Path to the tileset image
//   isSolid - Flag to mark tiles as solid (collision)
//   isTrap - Flag to mark tiles as traps (damage)
void MapParser::LoadMap(const char* mapFile, const char* tilesetFile, bool isSolid, bool isTrap) {
    // Load tileset
    SDL_Surface* tempSurface = IMG_Load(tilesetFile);
    if (!tempSurface) {
        std::cerr << "Failed to load tileset: " << tilesetFile << " - " << IMG_GetError() << std::endl;
        return;
    }
    // Convert surface to texture
    SDL_Texture* tileset = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!tileset) {
        std::cerr << "Failed to create texture from: " << tilesetFile << std::endl;
        return;
    }

    // Query tileset width for tile calculations
    int tilesetWidth;
    SDL_QueryTexture(tileset, nullptr, nullptr, &tilesetWidth, nullptr);

    // Open and read map file
    std::ifstream file(mapFile);
    if (!file) {
        std::cerr << "Failed to open map file: " << mapFile << std::endl;
        SDL_DestroyTexture(tileset);
        return;
    }

    std::vector<std::vector<int>> mapData;
    std::string line;
    int row = 0;
    
    // Parse map file line by line
    while (std::getline(file, line)) {
        std::vector<int> tileRow;
        std::stringstream ss(line);
        int tile;
        char comma;
        int col = 0;
        
        // Parse each tile value in the row
        while (ss >> tile) {
            tileRow.push_back(tile);
            ss >> comma; // Skip comma separator
            
            // Add tile to collision or trap list if applicable
            if (isSolid && tile != -1) {
                SDL_Rect solidTile = { col * tileSize, row * tileSize, tileSize, tileSize };
                solidTiles.push_back(solidTile);
            }
            else if (isTrap && tile != -1){
                SDL_Rect solidTile = { col * tileSize, row * tileSize, tileSize, tileSize };
                trapTiles.push_back(solidTile);
            }

            col++;
        }
        
        // Update map width if current row is wider
        if (col > m_MapWidth) {
            m_MapWidth = max(m_MapWidth, col * 16);
        }


        mapData.push_back(tileRow);
        row++;
        m_MapHeight = max(m_MapHeight, row * 16); // Update map height
    }
    

    maps.push_back({mapData, tileset, tilesetWidth});
}

void MapParser::Render(int offsetX, int offsetY) {
    for (const auto& mapData : maps) {
        if (!mapData.tileset) continue;

        int tilesPerRow = mapData.tilesetWidth / tileSize;

        for (size_t i = 0; i < mapData.tiles.size(); i++) {
            for (size_t j = 0; j < mapData.tiles[i].size(); j++) {
                int tileIndex = mapData.tiles[i][j];
                if (tileIndex == -1) continue;
                
                // Calculate source rectangle from tileset
                int srcX = (tileIndex % tilesPerRow) * tileSize;
                int srcY = (tileIndex / tilesPerRow) * tileSize;

                SDL_Rect srcRect = { srcX, srcY, tileSize, tileSize };
                SDL_Rect dstRect = { (int)j * tileSize - offsetX, (int)i * tileSize - offsetY, tileSize, tileSize };
                SDL_RenderCopy(renderer, mapData.tileset, &srcRect, &dstRect);
            }
        }
    }
}
