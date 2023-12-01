//
// Created by blakey on 25/11/23.
//

#include "chunk.h"
#include "../main.h"



namespace chunk {


    chunk::chunk() {
        data.info.Width, data.info.Depth, data.info.Height = 16;
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
        for (int x = 0; x < localChunk.data.info.Width; x++) {
            //std::cout << "X:" << x << std::endl;
            for (int y = 0; y < localChunk.data.info.Height ; y++) {
                for (int z = 0; z < localChunk.data.info.Depth ; z++) {
                    Block::Block block = Block::Block(1, glm::vec3{x, y,z}, Block::POSX);
                    localChunk.data.Blocks.push_back(block);
                    //init::app->createBlockVertex({x,y,z});
                }
            }
        }
        return localChunk;
    }

    Mesh GenerateChunkMesh(chunk &localChunk) {
        Mesh chunkMesh{};
        for (Block::Block block : localChunk.data.Blocks) {

        }
    }

} // chunk