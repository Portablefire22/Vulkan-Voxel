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

};
    chunk generateChunk(glm::vec3 ChunkPos);
    Mesh GenerateChunkMesh(chunk &localChunk);
} // chunk

#endif //CHUNK_H
