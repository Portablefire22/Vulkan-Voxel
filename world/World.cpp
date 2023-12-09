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
#include <SDL_timer.h>
#include <unordered_set>

#include "glm/gtx/hash.hpp"
#include <noise/noise.h>
#include "../deps/FastNoise2/include/FastNoise/FastNoise.h"


namespace WorldHandler {

    void World::RenderChunks(VulkanEngine& engine, std::vector<chunk::Chunk*>& chunks) {
        for (chunk::Chunk* localChunk : chunks) {
            bool chunkExists = false;
            for (auto t: engine._renderables) {
                if (t.position == localChunk->data.ChunkPosition) {
                    chunkExists = true;
                }
            }
            if (chunkExists) {
                continue;
            }

            RenderObject chunkObject;
            std::string name = glm::to_string(localChunk->data.ChunkPosition);


            // Check if the chunk mesh already exists, if not then create it
            if (!engine._meshes.contains(name)) {
                localChunk->GenerateChunkMesh();
            }
            chunkObject.mesh = engine.getMesh(name);
            if (chunkObject.mesh == nullptr) {
                continue;
            }
            chunkObject.name = name;


            chunkObject.material = engine.getMaterial("grass");

            glm::mat4 translation = glm::translate(glm::mat4{1.0}, localChunk->data.ChunkPosition * (float)CHUNK_SIZE);
            glm::mat4 scale = glm::scale(glm::mat4{1.0}, glm::vec3(1,1,1));
            chunkObject.transformMatrix = translation * scale;
            chunkObject.position = localChunk->data.ChunkPosition;
            engine._renderables.push_back(chunkObject);
        }
    }

    void World::RenderChunk(VulkanEngine& engine, chunk::Chunk* localChunk) {
        RenderObject chunkObject;
        std::string name = glm::to_string(localChunk->data.ChunkPosition);


        // Check if the chunk mesh already exists, if not then create it
        if (!engine._meshes.contains(name)) {
            localChunk->GenerateChunkMesh();
        }
        chunkObject.mesh = engine.getMesh(name);
        if (chunkObject.mesh == nullptr) {
            return;
        }
        chunkObject.name = name;


        chunkObject.material = engine.getMaterial("grass");

        glm::mat4 translation = glm::translate(glm::mat4{1.0}, localChunk->data.ChunkPosition * (float)CHUNK_SIZE);
        glm::mat4 scale = glm::scale(glm::mat4{1.0}, glm::vec3(1,1,1));
        chunkObject.transformMatrix = translation * scale;
        chunkObject.position = localChunk->data.ChunkPosition;
        engine._renderables.push_back(chunkObject);
    }

    std::vector<chunk::Chunk*> World::GetChunksAroundPlayer(VulkanEngine& engine, Player::Player &player, int horzRenderDistance, int vertRenderDistance) {
        std::vector<chunk::Chunk*> chunksToRender;
        for (int z = player.ChunkPosition.z - horzRenderDistance; z <= player.ChunkPosition.z + horzRenderDistance; z++) {
            for (int x = player.ChunkPosition.x - horzRenderDistance; x <= player.ChunkPosition.x + horzRenderDistance; x++) {
                auto tempRegion = GetRegion(x,z);
                for (int y = player.ChunkPosition.y - vertRenderDistance; y <= player.ChunkPosition.y + vertRenderDistance; y++) {
                    if (!tempRegion->isChunkEmpty(y)) {
                        chunksToRender.push_back(tempRegion->getChunk(y));
                    }
                }
            }
        }
        return chunksToRender;
    }

    Region* World::GetRegion(int x, int z) {
        if (!RegionMap.contains(std::make_pair(x,z))) {
            auto tempRegion = Region(x,z);
            RegionMap.insert(std::make_pair(std::make_pair(x,z), tempRegion));
        }
        return &RegionMap.at(std::make_pair(x,z));
    }

}