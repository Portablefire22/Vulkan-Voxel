//
// Created by blakey on 25/11/23.
//

#include "chunk.h"

#include <iostream>

#include "../main.h"



namespace chunk {


    chunk::chunk() {
        data.info.Width = 16;
        data.info.Depth = 16;
        data.info.Height = 16;
        data.info.ChunkPosition = {0,0,0};
    }

    chunk::chunk(glm::vec3 chunkPos, int height, int width, int depth) {
        //std::cout << "Created: " << height << "|" << width << "|" << depth << std::endl;
        //std::cout << "@: " << chunkPos.x << "|" << chunkPos.y << "|" << chunkPos.z << std::endl;
        std::vector<Block::Block> blocks;
        data = ChunkData {
            ChunkInfo{
                height,
                width,
                depth,
                chunkPos
            },
            blocks,
        };
    }

    chunk generateChunk(glm::vec3 ChunkPos) {
        // ChunkPos not currently used but will be required for when noise is used
        chunk localChunk;
        localChunk.data.info.ChunkPosition = ChunkPos;
        for (int y = 0; y < localChunk.data.info.Height ; y++) {
            std::cout << " X ->" << std::endl;
            for (int z = 0; z < localChunk.data.info.Depth ; z++) {
                for (int x = 0; x < localChunk.data.info.Width; x++) {
                    Block::Block block = Block::Block(1, glm::vec3{x + ChunkPos.x, y + ChunkPos.y,z + ChunkPos.z }, Block::POSX);
                    localChunk.data.Blocks.push_back(block);
                    //init::app->createBlockVertex({x,y,z});
                    std::cout << x <<  " ";
                }
                std::cout << " Z " << std::endl;
            }
            std::cout << std::endl << std::endl << std::endl;
        }
        return localChunk;
    }

    Mesh GenerateChunkMesh(chunk &localChunk) {
        Mesh chunkMesh{};
        for (Block::Block block : localChunk.data.Blocks) {

        }
    }

} // chunk