//
// Created by blakey on 08/12/23.
//

#include "Region.h"

#include <iostream>
#include <ostream>
#include <map>

#include "FastNoise/FastNoise.h"

Region::Region(int x, int z) {
    this->Position = std::make_pair(x,z);
    generateHeightMap();
}

bool Region::isChunkEmpty(const int yLevel) {
    if (!doesChunkExist(yLevel)) {
        createChunk(yLevel);
    }
    const auto localChunk = getChunk(yLevel);
    const auto blocks = localChunk->Blocks;
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x ++) {
                if (blocks[y][z][x] != static_cast<std::byte>(0)) {
                    this->ChunkInfo[static_cast<int>(localChunk->ChunkPosition.y)].isEmpty = false;
                    return false;
                }
            }
        }
    }
    this->ChunkInfo[yLevel].isEmpty = true;
    return true;
}

bool Region::isChunkEmpty(const chunk::Chunk* localChunk) {
    const auto blocks = localChunk->Blocks;
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x ++) {
                if (blocks[y][z][x] != static_cast<std::byte>(0)) {
                    this->ChunkInfo[static_cast<int>(localChunk->ChunkPosition.y)].isEmpty = false;
                    return false;
                }
            }
        }
    }
    this->ChunkInfo[static_cast<int>(localChunk->ChunkPosition.y)].isEmpty = true;
    return true;
}

chunk::Chunk* Region::getChunk(const int yLevel) {
    if (!doesChunkExist(yLevel)) { // Create the chunk if it doesnt exist
        this->createChunk(yLevel);
    }
    return this->Chunks[yLevel];
}

bool Region::doesChunkExist(const int yLevel) const {
    return this->Chunks.contains(yLevel);
}


bool Region::createChunk(int yLevel) {
    auto localChunk = new chunk::Chunk(this, glm::vec3(this->Position.first, yLevel, this->Position.second));
    localChunk->generateChunk();
    this->Chunks.insert(std::make_pair(yLevel, localChunk));
    return true;
}

bool Region::generateHeightMap() {
    try {
        std::vector<float> NoiseArr(CHUNK_SIZE * CHUNK_SIZE);
        const auto fnNoise = FastNoise::New<FastNoise::OpenSimplex2>();
        fnNoise->GenUniformGrid2D(NoiseArr.data(),  this->Position.first*CHUNK_SIZE, this->Position.second*CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,0.02f, 456163);
        for (int i = 0; i < (CHUNK_SIZE * CHUNK_SIZE); i++) {
            this->HeightMap[i] =  abs(static_cast<int>(round(16 * NoiseArr[i]))) + 2;
        }
        return true;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

int* Region::getBlockHeight(const int x, const int z) {
    return &this->HeightMap[x + (z * CHUNK_SIZE)];
}

int* Region::getHeightMap() {
    return this->HeightMap;
}
