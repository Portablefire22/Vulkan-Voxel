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
#include "glm/gtx/hash.hpp"


namespace WorldHandler {

    void World::RenderChunks(VulkanEngine& engine, std::set<chunk::chunk> chunks) {

        for (chunk::chunk localChunk : chunks) {
            RenderObject chunkObject;
            std::string name = glm::to_string(localChunk.data.info.ChunkPosition);
            // Check if the chunk mesh already exists, if not then create it
            if (engine._meshes.contains(glm::to_string(localChunk.data.info.ChunkPosition))) {
                chunkObject.name = name.data();
                chunkObject.mesh = engine.getMesh(name);
                chunkObject.material = engine.getMaterial(name);
                engine._renderables.push_back(chunkObject);
                continue;
            }
            Mesh chunkMesh;
            // If the mesh doesn't exist
            for (Block::Block block : localChunk.data.Blocks) {

            }
            engine.uploadMesh(chunkMesh);
            engine._meshes[name] = chunkMesh;
        }
    }

    std::set<chunk::chunk> World::GetChunksAroundPlayer(Player::Player &player, int horzRenderDistance, int vertRenderDistance) {
        std::set<chunk::chunk> chunksToRender;
        for (int x = -horzRenderDistance; x <= horzRenderDistance; x++) {
            for (int z = -horzRenderDistance; z <= horzRenderDistance; z++) {
                for (int y = -vertRenderDistance; y <= vertRenderDistance; y++) {
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
            int16_t blockToCheckPos = relativeBlockPos.x + ((int)relativeBlockPos.z << 4) + ((int)relativeBlockPos.y << 8);
            // Disgusting code
            switch (static_cast<RenderBlock::FACE>(faceVal)) {
                case RenderBlock::FRONT: // PosX
                    blockToCheckPos++;
                    if (blockToCheckPos % 16 == 0) {
                        // Dont check if it is on the edge of the chunk (for now)
                        addFace = true;
                    }
                    break;
                case RenderBlock::BACK: // NegX
                    blockToCheckPos--;
                    if (blockToCheckPos % 16 == 0) {
                        addFace = true;
                    }
                    break;
                case RenderBlock::LEFT: // Neg Z
                    blockToCheckPos -= 16;
                    if (blockToCheckPos % 256 <= 16) {
                        addFace = true;
                    }
                    break;
                case RenderBlock::RIGHT: // Pos Z
                    blockToCheckPos += 16;
                    if (blockToCheckPos % 256 >= 240) {
                        addFace = true;
                    }
                    break;
                case RenderBlock::TOP: // Pos Y
                    blockToCheckPos += 256;
                    if (blockToCheckPos > 4096) {
                        addFace = true;
                    }
                    break;
                case RenderBlock::BOTTOM: // Neg Y
                    blockToCheckPos -= 256;
                    if (blockToCheckPos < 0) {
                        addFace = true;
                    }
                    break;
                default:
                    break;
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

