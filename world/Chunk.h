//
// Created by blakey on 25/11/23.
//



#ifndef CHUNK_H
#define CHUNK_H
#pragma once

#define CHUNK_SIZE 32
#define CHUNK_BLOCKS CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE
#define WATER_LEVEL (2 * (CHUNK_SIZE / 16))

#define HorzRenderDist 4
#define VertRenderDist 2

#include <glm/vec3.hpp>
#include <vector>
#include "../block/Block.h"
#include "../RenderUtils/RenderBlock.h"


struct Mesh;

namespace Block {
    class Block;
}

namespace chunk {
    struct ChunkInfo { // Chunks will never be differing sizes
        glm::vec3 ChunkPosition;
    };
    struct ChunkData {
        ChunkInfo info;
        std::byte Blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
        bool isSurface;
    };
class Chunk {
public:
    ChunkData data{};
    Chunk();
    Chunk(glm::vec3 chunkPos);
    Mesh GenerateChunkMesh();
    int GetVoxel(glm::vec3& position);
    std::vector<RenderBlock::FACE> ShouldRenderFace(glm::vec3& originalPos);

    bool operator==(const Chunk& other) const {
        if (this->data.info.ChunkPosition.x == other.data.info.ChunkPosition.x && this->data.info.ChunkPosition.y == other.data.info.ChunkPosition.y && this->data.info.ChunkPosition.z == other.data.info.ChunkPosition.z )
             return true;
        return false;
    }
    struct HashFunction {
        size_t operator()(const Chunk& other) const {
            size_t xHash = std::hash<int>()(other.data.info.ChunkPosition.x);
            size_t yHash = std::hash<int>()(other.data.info.ChunkPosition.y);
            size_t zHash = std::hash<int>()(other.data.info.ChunkPosition.z);
            return xHash ^ yHash ^ zHash;
        }
    };
};
    class ChunkManager {
    public:
        Chunk generateChunk(glm::vec3 ChunkPos);

    };
    void IndexToVec(int index, glm::vec3* blockPosition);
} // chunk

#endif //CHUNK_H
