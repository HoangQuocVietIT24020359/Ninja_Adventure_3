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

// Load bản đồ, thêm tùy chọn xác định tile va chạm
void MapParser::LoadMap(const char* mapFile, const char* tilesetFile, bool isSolid, bool isTrap) {
    // Load tileset
    SDL_Surface* tempSurface = IMG_Load(tilesetFile);
    if (!tempSurface) {
        std::cerr << "Failed to load tileset: " << tilesetFile << " - " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* tileset = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!tileset) {
        std::cerr << "Failed to create texture from: " << tilesetFile << std::endl;
        return;
    }

    // Lấy chiều rộng thực tế của tileset
    int tilesetWidth;
    SDL_QueryTexture(tileset, nullptr, nullptr, &tilesetWidth, nullptr);

    // Load map file
    std::ifstream file(mapFile);
    if (!file) {
        std::cerr << "Failed to open map file: " << mapFile << std::endl;
        SDL_DestroyTexture(tileset);
        return;
    }

    std::vector<std::vector<int>> mapData;
    std::string line;
    int row = 0;

    while (std::getline(file, line)) {
        std::vector<int> tileRow;
        std::stringstream ss(line);
        int tile;
        char comma;
        int col = 0;

        while (ss >> tile) {
            tileRow.push_back(tile);
            ss >> comma;

            if (isSolid && tile != -1) {
                // Nếu là bản đồ solid, thêm tile vào danh sách va chạm
                SDL_Rect solidTile = { col * tileSize, row * tileSize, tileSize, tileSize };
                solidTiles.push_back(solidTile);
            }
            else if (isTrap && tile != -1){
                SDL_Rect solidTile = { col * tileSize, row * tileSize, tileSize, tileSize };
                trapTiles.push_back(solidTile);
            }

            col++;
        }
        
        if (col > m_MapWidth) {
            m_MapWidth = max(m_MapWidth, col * 16); // Cập nhật chiều rộng lớn nhất
        }


        mapData.push_back(tileRow);
        row++;
        m_MapHeight = max(m_MapHeight, row * 16); // Tổng số dòng chính là chiều cao
    }
    
    // Lưu vào danh sách bản đồ
    maps.push_back({mapData, tileset, tilesetWidth});
}

// Render tất cả các bản đồ đã load
void MapParser::Render(int offsetX, int offsetY) {
    for (const auto& mapData : maps) {
        if (!mapData.tileset) continue;

        int tilesPerRow = mapData.tilesetWidth / tileSize;

        for (size_t i = 0; i < mapData.tiles.size(); i++) {
            for (size_t j = 0; j < mapData.tiles[i].size(); j++) {
                int tileIndex = mapData.tiles[i][j];
                if (tileIndex == -1) continue;

                int srcX = (tileIndex % tilesPerRow) * tileSize;
                int srcY = (tileIndex / tilesPerRow) * tileSize;

                SDL_Rect srcRect = { srcX, srcY, tileSize, tileSize };
                SDL_Rect dstRect = { (int)j * tileSize - offsetX, (int)i * tileSize - offsetY, tileSize, tileSize };
                SDL_RenderCopy(renderer, mapData.tileset, &srcRect, &dstRect);
            }
        }
    }
}
