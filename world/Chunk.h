//
// Created by blakey on 25/11/23.
//



#ifndef CHUNK_H
#define CHUNK_H
#pragma once

#define CHUNK_SIZE 32
#define CHUNK_BLOCKS CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE

#define SURFACE_LEVEL 64

#define WATER_LEVEL SURFACE_LEVEL + (CHUNK_SIZE / 8)

#define HorzRenderDist 4
#define VertRenderDist 2

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
    struct ChunkData {
        glm::vec3 ChunkPosition;
        std::byte Blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    };
class Chunk {
public:
    ChunkData data{};
    Chunk();
    Chunk(glm::vec3 chunkPos);
    Mesh GenerateChunkMesh();
    int GetVoxel(glm::vec3& position);
    std::vector<RenderBlock::FACE> ShouldRenderFace(glm::vec3& originalPos);

    bool generateChunk();

    bool operator==(const Chunk& other) const {
        if (this->data.ChunkPosition.x == other.data.ChunkPosition.x && this->data.ChunkPosition.y == other.data.ChunkPosition.y && this->data.ChunkPosition.z == other.data.ChunkPosition.z )
             return true;
        return false;
    }
    struct HashFunction {
        size_t operator()(const Chunk& other) const {
            size_t xHash = std::hash<int>()(other.data.ChunkPosition.x);
            size_t yHash = std::hash<int>()(other.data.ChunkPosition.y);
            size_t zHash = std::hash<int>()(other.data.ChunkPosition.z);
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
