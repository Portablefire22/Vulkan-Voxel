//
// Created by blakey on 25/11/23.
//



#ifndef CHUNK_H
#define CHUNK_H
#pragma once



#include <glm/vec3.hpp>
#include <vector>
#include "../block/Block.h"


struct Mesh;

namespace Block {
    class Block;
}

namespace chunk {
    struct ChunkInfo {
        int Height;
        int Width;
        int Depth;
        glm::vec3 ChunkPosition;
    };
    struct ChunkData {
        ChunkInfo info;
        std::vector<Block::Block> Blocks;
    };
class chunk {
public:
    ChunkData data;
    chunk();
    chunk(glm::vec3 chunkPos, int height = 16, int width = 16, int depth = 16);

    bool operator==(const chunk& other) const {
        if (this->data.info.ChunkPosition.x == other.data.info.ChunkPosition.x && this->data.info.ChunkPosition.y == other.data.info.ChunkPosition.y && this->data.info.ChunkPosition.z == other.data.info.ChunkPosition.z )
             return true;
        return false;
    }
    struct HashFunction {
        size_t operator()(const chunk& other) const {
            size_t xHash = std::hash<int>()(other.data.info.ChunkPosition.x);
            size_t yHash = std::hash<int>()(other.data.info.ChunkPosition.y);
            size_t zHash = std::hash<int>()(other.data.info.ChunkPosition.z);
            return xHash ^ yHash ^ zHash;
        }
    };
};
    chunk generateChunk(glm::vec3 ChunkPos);
    Mesh GenerateChunkMesh(chunk &localChunk);
} // chunk

#endif //CHUNK_H
