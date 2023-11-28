//
// Created by blakey on 25/11/23.
//

#include "chunk.h"
#include "../main.h"



namespace chunk {

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

    void chunk::generateChunk() {
        //std::cout << "Generating : " << data.info.Width << "," << data.info.Height << "," << data.info.Depth << std::endl;
        //std::cout << "Generating this: " << this->data.info.Width << "," << this->data.info.Height << "," << this->data.info.Depth << std::endl;
        for (int x = 0; x < (*this).data.info.Width; x++) {
            //std::cout << "X:" << x << std::endl;
            for (int y = 0; y < this->data.info.Height ; y++) {
                for (int z = 0; z < this->data.info.Depth ; z++) {
                    Block::Block block = Block::Block(1, glm::vec3{x, y,z}, Block::POSX);
                    this->data.Blocks.push_back(block);
                    //init::app->createBlockVertex({x,y,z});

                }
            }
        }
    }
} // chunk