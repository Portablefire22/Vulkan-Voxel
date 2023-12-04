//
// Created by blakey on 01/12/23.
//

#include "World.h"

#include <cstdlib>
#include <set>
#include <glm/vec4.hpp>
#include "../vk_engine.h"
#include "../RenderUtils/RenderBlock.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_query.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <complex>
#include <iostream>
#include <unordered_set>

#include "glm/gtx/hash.hpp"


namespace WorldHandler {

    void World::RenderChunks(VulkanEngine& engine, std::unordered_set<chunk::Chunk, chunk::Chunk::HashFunction>& chunks) {

        for (chunk::Chunk localChunk : chunks) {
            RenderObject chunkObject;
            std::string name = glm::to_string(localChunk.data.info.ChunkPosition);
            // Check if the chunk mesh already exists, if not then create it
            if (!engine._meshes.contains(name)) {
                localChunk.GenerateChunkMesh();
            }
            chunkObject.name = name.data();
            chunkObject.mesh = engine.getMesh(name);
            chunkObject.material = engine.getMaterial("grass");

            glm::mat4 translation = glm::translate(glm::mat4{1.0}, localChunk.data.info.ChunkPosition * 16.0f);
            glm::mat4 scale = glm::scale(glm::mat4{1.0}, glm::vec3(.5,.5,.5));
            chunkObject.transformMatrix = translation * scale;
            engine._renderables.push_back(chunkObject);
        }
    }

    std::unordered_set<chunk::Chunk, chunk::Chunk::HashFunction> World::GetChunksAroundPlayer(VulkanEngine& engine, Player::Player &player, int horzRenderDistance, int vertRenderDistance) {
        std::unordered_set<chunk::Chunk, chunk::Chunk::HashFunction> chunksToRender;
        for (int x = player.Position.x - horzRenderDistance; x <= player.Position.x + horzRenderDistance; x++) {
            for (int z = player.Position.z - horzRenderDistance; z <= player.Position.z + horzRenderDistance; z++) {
                for (int y = player.Position.y - vertRenderDistance; y <= player.Position.y + vertRenderDistance; y++) {
                    chunksToRender.insert(GetChunk(engine, x,y,z));
                }
            }
        }
        return chunksToRender;
    }

    chunk::Chunk World::GetChunk(VulkanEngine& engine, int ChunkX, int ChunkY, int ChunkZ) {
        chunk::Chunk localChunk;
        std::map<int32_t, chunk::Chunk> localRegion;
        // Check if the given chunk exists or not
        glm::vec2 chunkVec = {ChunkX, ChunkZ};
        if (WorldMap.contains(chunkVec)) {
            localRegion = WorldMap.at(chunkVec);
            if (localRegion.contains(ChunkY)) {
                return localRegion.at(ChunkY);
            }
            localChunk = engine._ChunkManager.generateChunk({ChunkX, ChunkY, ChunkZ});
            localRegion.insert(std::make_pair(ChunkY, localChunk));
            return localChunk;
        }
        localChunk = engine._ChunkManager.generateChunk({ChunkX, ChunkY, ChunkZ});
        localRegion.insert(std::make_pair(ChunkY, localChunk));
        WorldMap.insert(std::make_pair(chunkVec, localRegion));
        return localChunk;
    }


} // WorldHandler

/*  Gonna have to pull out some bitshift wizardry I think
 *  X ->
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  1   Z
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  2   V
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  3
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  4
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  5
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  6
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  7
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  8
 *  1 2 3 4 5 6 7 8 9 [10] 11 12 13 14 15 16  9
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  10
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  11
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  12
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  13
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  14
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  15
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  16
 *
 *  Array pos = X + (Z >> 4) + ( Y << 8)
 */

/*
 *std::vector<RenderBlock::FACE> ShouldRenderFace(glm::vec3* originalPos) {
        std::vector<RenderBlock::FACE> faces;
        for (int faceVal = RenderBlock::FRONT; faceVal <= RenderBlock::BOTTOM; faceVal++) {
            bool addFace = false;
            // Convert 3D space to an index in the byte array
            switch (static_cast<RenderBlock::FACE>(faceVal)) { // TODO FIX THIS LOGIC
                case RenderBlock::FRONT: // PosX
                    // On a chunk border
                    if (blockToCheckPos % 16 == 15) {
                        addFace = true;
                        break;
                    }
                    blockToCheckPos++;
                    break;
                case RenderBlock::BACK: // NegX
                    if (blockToCheckPos % 16 == 0) {
                        addFace = true;
                        break;
                    }
                    blockToCheckPos--;
                    break;
                case RenderBlock::LEFT: // Neg Z
                    if (blockToCheckPos % 256 <= 15) {
                        addFace = true;
                        break;
                    }
                    blockToCheckPos -= 16;
                    break;
                case RenderBlock::RIGHT: // Pos Z
                    if (blockToCheckPos % 256 >= 240) {
                        addFace = true;
                        break;
                    }
                    blockToCheckPos += 16;
                    break;
                case RenderBlock::TOP: // Pos Y Works
                    if (blockToCheckPos >= 3840) {
                        addFace = true;
                        break;
                    }
                    blockToCheckPos += 256;
                    break;
                case RenderBlock::BOTTOM: // Neg Y
                    if (blockToCheckPos <= 256) {
                        addFace = true;
                        break;
                    }
                    blockToCheckPos -= 256;
                    break;
                default:
                    break;
            }
            //std::cout << localChunk.data.Blocks[blockToCheckPos].blockState.BlockId << std::endl;
            std::vector<Block::Block> blockVec = localChunk->data.Blocks;
            if (!addFace) {
                if (blockVec.size() < blockToCheckPos || blockToCheckPos <= 0) {
                    addFace = true;
                }
                if (!addFace && blockVec[blockToCheckPos].blockState.BlockId != 1) { // If it is within the vector
                    std::cout << "SIZE: " << blockVec.size() << " CHECKING: " << blockToCheckPos - 1 << " FOUND ID: " << blockVec[blockToCheckPos - 1].blockState.BlockId << std::endl;
                    addFace = true;
                }
            }
            if (addFace) {
                faces.push_back(static_cast<RenderBlock::FACE>(faceVal));
            }
        }
        return faces;
    }*/
