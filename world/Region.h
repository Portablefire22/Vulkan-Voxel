//
// Created by blakey on 08/12/23.
//

#ifndef REGION_H
#define REGION_H
#include <map>
#include <set>

#include "Chunk.h"


class Region {

    private:
    struct ChunkInformation {
        bool isEmpty;
    };
    int HeightMap[CHUNK_SIZE * CHUNK_SIZE] = {0};

    std::map<int, chunk::Chunk*> Chunks;
    std::pmr::map<int, ChunkInformation> ChunkInfo;
    std::pair<int,int> Position;


    public:
    Region(int x, int z);

    chunk::Chunk* getChunk(int yLevel);
    bool createChunk(int yLevel);
    bool isChunkEmpty(int yLevel);
    bool isChunkEmpty(const chunk::Chunk* localChunk);
    bool doesChunkExist(int yLevel) const;
    void setChunkEmpty(const int yLevel, bool isEmpty);
    int* getHeightMap();
    int* getBlockHeight(int x, int z);
    bool generateHeightMap();
    ~Region();
};



#endif //REGION_H
