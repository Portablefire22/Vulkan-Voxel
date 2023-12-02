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

    void World::RenderChunks(VulkanEngine& engine, std::unordered_set<chunk::chunk, chunk::chunk::HashFunction> chunks) {

        for (chunk::chunk localChunk : chunks) {
            RenderObject chunkObject;
            std::string name = glm::to_string(localChunk.data.info.ChunkPosition);
            // Check if the chunk mesh already exists, if not then create it
            if (!engine._meshes.contains(glm::to_string(localChunk.data.info.ChunkPosition))) {
                Mesh chunkMesh;
                // If the mesh doesn't exist
                for (Block::Block block : localChunk.data.Blocks) {
                    RenderBlock::AddBlockVertices(chunkMesh, CheckBlockFaces(localChunk, block.blockState.Position), block.blockState.Position);
                }
                engine.uploadMesh(chunkMesh);
                engine._meshes[name] = chunkMesh;
            }
            chunkObject.name = name.data();
            chunkObject.mesh = engine.getMesh(name);
            chunkObject.material = engine.getMaterial("grass");

            glm::mat4 translation = glm::translate(glm::mat4{1.0}, glm::vec3((int)localChunk.data.info.ChunkPosition.x << 4, (int)localChunk.data.info.ChunkPosition.y << 4,(int)localChunk.data.info.ChunkPosition.z << 4));
            glm::mat4 scale = glm::scale(glm::mat4{1.0}, glm::vec3(0.5,0.5,0.5));
            chunkObject.transformMatrix = translation * scale;
            engine._renderables.push_back(chunkObject);
        }
    }

    std::unordered_set<chunk::chunk, chunk::chunk::HashFunction> World::GetChunksAroundPlayer(Player::Player &player, int horzRenderDistance, int vertRenderDistance) {
        std::unordered_set<chunk::chunk, chunk::chunk::HashFunction> chunksToRender;
        for (int x = player.Position.x - horzRenderDistance; x <= player.Position.x + horzRenderDistance; x++) {
            for (int z = player.Position.z - horzRenderDistance; z <= player.Position.z + horzRenderDistance; z++) {
                for (int y = player.Position.y - vertRenderDistance; y <= player.Position.y + vertRenderDistance; y++) {
                    std::cout << x << "," << y << "," << z << std::endl;
                    chunksToRender.insert(GetChunk(x,y,z));
                }
            }
        }
        return chunksToRender;
    }

    chunk::chunk World::GetChunk(int ChunkX, int ChunkY, int ChunkZ) {
        chunk::chunk localChunk;
        std::map<int32_t, chunk::chunk> localRegion;
        // Check if the given chunk exists or not
        glm::vec2 chunkVec = {ChunkX, ChunkZ};
        if (WorldMap.contains(chunkVec)) {
            localRegion = WorldMap.at(chunkVec);
            if (localRegion.contains(ChunkY)) {
                return localRegion.at(ChunkY);
            }
            localChunk = chunk::generateChunk({ChunkX, ChunkY, ChunkZ});
            localRegion.insert(std::make_pair(ChunkY, localChunk));
            return localChunk;
        }
        localChunk = chunk::generateChunk({ChunkX, ChunkY, ChunkZ});
        localRegion.insert(std::make_pair(ChunkY, localChunk));
        WorldMap.insert(std::make_pair(chunkVec, localRegion));
        return localChunk;
    }


    std::vector<RenderBlock::FACE> World::CheckBlockFaces(chunk::chunk localChunk, glm::vec3 BlockPos) {
        auto chunkPos = localChunk.data.info.ChunkPosition;
        glm::vec3 relativeBlockPos = {BlockPos.x - chunkPos.x,BlockPos.y - chunkPos.y,BlockPos.z - chunkPos.z};
        std::vector<RenderBlock::FACE> faces;
        for (int faceVal = RenderBlock::FRONT; faceVal <= RenderBlock::BOTTOM; faceVal++) {
            bool addFace = false;
            int16_t blockToCheckPos = std::abs(relativeBlockPos.x) + std::abs(((int)relativeBlockPos.z << 4)) + std::abs(((int)relativeBlockPos.y << 8));
            // Disgusting code

            switch (static_cast<RenderBlock::FACE>(faceVal)) { // TODO FIX THIS LOGIC
                /*
                 * Why did I think this would even be a good idea?
                 * This logic would only apply on chunk boundaries
                 *
                 * Apply this and other logic and it should work
                 */
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
            std::vector<Block::Block> blockVec = localChunk.data.Blocks;
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

