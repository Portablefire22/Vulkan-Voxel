//
// Created by blakey on 08/12/23.
//

#ifndef REGION_H
#define REGION_H
#include "../threadpool/MapSafe.hpp"
#include <map>

#include "Chunk.h"
struct ChunkInformation
{
    bool isEmpty;
    bool isRendered;
};
class Region
{

  public:
    Region(int x, int z);

    chunk::Chunk* getChunk(int yLevel);
    bool createChunk(int yLevel);
    bool isChunkEmpty(int yLevel);
    bool isChunkRendered(int yLevel);
    bool isChunkEmpty(const chunk::Chunk* localChunk);
    bool doesChunkExist(int yLevel) const;
    void setChunkEmpty(const int yLevel, bool isEmpty);
    void setChunkRendered(const int yLevel, bool isEmpty);
    int* getHeightMap();
    int* getBlockHeight(int x, int z);
    bool generateHeightMap();
    bool isGenerated();
    ~Region();
    Region();

  private:
    int HeightMap[CHUNK_SIZE * CHUNK_SIZE] = { 0 };

    MapSafe<int, chunk::Chunk*>* Chunks;
    MapSafe<int, ChunkInformation>* ChunkInfo;
    std::pair<int, int> Position;
    bool _generated = false;
};

#endif // REGION_H
