//
// Created by blakey on 08/12/23.
//
#include <cmath>
#define GLM_ENABLE_EXPERIMENTAL
#include "Region.h"

#include <FastNoise/FastNoise.h>

#include <iostream>
#include <map>
#include <ostream>

Region::Region(int x, int z)
{
    ChunkInfo = new MapSafe<int, ChunkInformation>;
    this->Position = std::make_pair(x, z);
    generateHeightMap();
}

Region::Region() : Region::Region(-69, -1337){}


void
Region::setChunkEmpty(const int yLevel, bool isEmpty)
{
    this->ChunkInfo->at_ptr(yLevel).isEmpty = isEmpty;
}

bool
Region::isChunkEmpty(const int yLevel)
{
  
    if (this->ChunkInfo->contains(yLevel)) {
        return this->ChunkInfo->at(yLevel).isEmpty;
    }
    if (!doesChunkExist(yLevel)) {
        createChunk(yLevel);
    }
    const auto localChunk = getChunk(yLevel);
    const auto blocks = localChunk->Blocks;
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                if (blocks[y][z][x] != static_cast<std::byte>(0)) {
                    this
                      ->ChunkInfo->at_ptr(static_cast<int>(localChunk->ChunkPosition.y))
                      .isEmpty = false;
                    return false;
                }
            }
        }
    }
    this->ChunkInfo->at_ptr(yLevel).isEmpty = true;
    return true;
}

bool
Region::isChunkEmpty(const chunk::Chunk* localChunk)
{
    if (this->ChunkInfo->contains(localChunk->ChunkPosition.y)) {
        return this->ChunkInfo->at(localChunk->ChunkPosition.y).isEmpty;
    }
    const auto blocks = localChunk->Blocks;
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                if (blocks[y][z][x] != static_cast<std::byte>(0)) {
                    this
                      ->ChunkInfo->at_ptr(static_cast<int>(localChunk->ChunkPosition.y)).isEmpty = false;
                  return false;
                }
            }
        }
    }
    this->ChunkInfo->at_ptr(static_cast<int>(localChunk->ChunkPosition.y)).isEmpty =
      true;
    return true;
}

chunk::Chunk*
Region::getChunk(const int yLevel)
{
    if (!doesChunkExist(yLevel)) { // Create the chunk if it doesnt exist
        this->createChunk(yLevel);
    }
    return this->Chunks[yLevel];
}

bool
Region::doesChunkExist(const int yLevel) const
{
    return this->Chunks.contains(yLevel);
}

bool
Region::createChunk(int yLevel)
{
    auto localChunk = new chunk::Chunk(
      this, glm::vec3(this->Position.first, yLevel, this->Position.second));
    localChunk->generateChunk();
    this->Chunks.insert(std::make_pair(yLevel, localChunk));
    return true;
}

bool
Region::generateHeightMap()
{
    try {
        std::vector<float> NoiseArr(CHUNK_SIZE * CHUNK_SIZE);
        const auto fnSimplex = FastNoise::New<FastNoise::OpenSimplex2>();
        const auto fnNoise = FastNoise::New<FastNoise::FractalFBm>();
        fnNoise->SetSource(fnSimplex);
        fnNoise->SetOctaveCount(8);
        fnNoise->GenUniformGrid2D(NoiseArr.data(),
                                  this->Position.first * CHUNK_SIZE,
                                  this->Position.second * CHUNK_SIZE,
                                  CHUNK_SIZE,
                                  CHUNK_SIZE,
                                  0.0005f,
                                  21321);
        for (int i = 0; i < (CHUNK_SIZE * CHUNK_SIZE); i++) {
            this->HeightMap[i] =
              abs(static_cast<int>(round(256 * NoiseArr[i]))) + 2;
        }
        return true;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

int*
Region::getBlockHeight(const int x, const int z)
{
    return &this->HeightMap[x + (z * CHUNK_SIZE)];
}

int*
Region::getHeightMap()
{
    return this->HeightMap;
}


Region::~Region()
{
    for (auto const& [key, val] : this->Chunks) {
        delete (val);
    }
    // delete (ChunkInfo);
}
