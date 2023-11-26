//
// Created by blakey on 25/11/23.
//
#pragma once
#ifndef CHUNK_H
#define CHUNK_H
#include <glm/vec3.hpp>
#include <vector>
#include "../block/Block.h"




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
    explicit chunk(glm::vec3 chunkPos, int height = 16, int width = 16, int depth = 16);
    void generateChunk();
};

} // chunk

#endif //CHUNK_H
