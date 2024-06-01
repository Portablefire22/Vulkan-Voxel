//
// Created by blakey on 25/11/23.
//



#ifndef CHUNK_H
#define CHUNK_H
#pragma once

#define CHUNK_SIZE 32
#define CHUNK_BLOCKS CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE

#define SURFACE_LEVEL 0

//#define WATER_LEVEL ((SURFACE_LEVEL +(CHUNK_SIZE / 4)) * CHUNK_SIZE)
#define WATER_LEVEL (SURFACE_LEVEL + CHUNK_SIZE / 2)
#define HorzRenderDist 6
#define VertRenderDist 5

#include <glm/vec3.hpp>
#include <vector>
#include "../block/Block.h"
#include "../RenderUtils/RenderBlock.h"


class Region;
struct Mesh;

namespace Block {
    class Block;
}

namespace chunk {
class Chunk {
public:
    glm::vec3 ChunkPosition{};
    std::byte Blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = { (std::byte) 0};
    Region* ParentRegion;
    Chunk();
    Chunk(Region* Parent, glm::vec3 chunkPos);
    Mesh GenerateChunkMesh();
    int GetVoxel(glm::vec3& position);
    std::vector<RenderBlock::FACE> ShouldRenderFace(glm::vec3& originalPos);

    bool generateChunk();

    bool operator==(const Chunk& other) const {
        if (this->ChunkPosition.x == other.ChunkPosition.x && this->ChunkPosition.y == other.ChunkPosition.y && this->ChunkPosition.z == other.ChunkPosition.z )
             return true;
        return false;
    }
    struct HashFunction {
        size_t operator()(const Chunk& other) const {
            size_t xHash = std::hash<int>()(other.ChunkPosition.x);
            size_t yHash = std::hash<int>()(other.ChunkPosition.y);
            size_t zHash = std::hash<int>()(other.ChunkPosition.z);
            return xHash ^ yHash ^ zHash;
        }
    };
};
    class ChunkManager {
    public:


    };
    void IndexToVec(int index, glm::vec3* blockPosition);
} // chunk

#endif //CHUNK_H
